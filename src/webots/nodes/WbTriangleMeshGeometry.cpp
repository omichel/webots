// Copyright 1996-2021 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "WbTriangleMeshGeometry.hpp"

#include "WbBoundingSphere.hpp"
#include "WbField.hpp"
#include "WbMatter.hpp"
#include "WbNodeUtilities.hpp"
#include "WbOdeGeomData.hpp"
#include "WbRay.hpp"
#include "WbResizeManipulator.hpp"
#include "WbSimulationState.hpp"
#include "WbTransform.hpp"
#include "WbTriangleMesh.hpp"
#include "WbWorld.hpp"
#include "WbWrenMeshBuffers.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include <wren/node.h>

#include <ode/ode.h>

WbTriangleMeshMap WbTriangleMeshGeometry::cTriangleMeshMap;

void WbTriangleMeshGeometry::init() {
  mTrimeshData = NULL;
  mTriangleMesh = NULL;
  mScaledCoordinatesNeedUpdate = true;
  mCorrectSolidMass = true;
  mIsOdeDataApplied = false;
  mNormalsMesh = NULL;
  mNormalsMaterial = NULL;
  mNormalsRenderable = NULL;
}

WbTriangleMeshGeometry::WbTriangleMeshGeometry(const QString &modelName, WbTokenizer *tokenizer) :
  WbGeometry(modelName, tokenizer) {
  init();
}

WbTriangleMeshGeometry::WbTriangleMeshGeometry(const WbTriangleMeshGeometry &other) : WbGeometry(other) {
  init();
}

WbTriangleMeshGeometry::WbTriangleMeshGeometry(const WbNode &other) : WbGeometry(other) {
  init();
}

WbTriangleMeshGeometry::~WbTriangleMeshGeometry() {
  wr_static_mesh_delete(mWrenMesh);
  wr_static_mesh_delete(mNormalsMesh);

  deleteWrenRenderable();

  if (mTriangleMesh) {
    WbTriangleMeshCache::releaseTriangleMesh(this);
    clearTrimeshResources();
  }
}

void WbTriangleMeshGeometry::preFinalize() {
  if (isPreFinalizedCalled())
    return;

  WbGeometry::preFinalize();

  mMeshKey.set(this);
  WbTriangleMeshCache::useTriangleMesh(this);
}

WbTriangleMeshCache::TriangleMeshInfo WbTriangleMeshGeometry::createTriangleMesh() {
  delete mTriangleMesh;
  mTriangleMesh = new WbTriangleMesh();
  updateTriangleMesh(false);

  return WbTriangleMeshCache::TriangleMeshInfo(mTriangleMesh);
}

void WbTriangleMeshGeometry::clearTrimeshResources() {
  if (mTrimeshData) {
    dGeomTriMeshDataDestroy(mTrimeshData);
    mTrimeshData = NULL;
  }
}

void WbTriangleMeshGeometry::createWrenObjects() {
  if (WbNodeUtilities::findContainingProto(this))
    updateTriangleMesh(false);

  foreach (QString warning, mTriangleMesh->warnings())
    parsingWarn(warning);

  if (!mTriangleMeshError.isEmpty())
    parsingWarn(tr("Cannot create %1 because: \"%2\".").arg(nodeModelName()).arg(mTriangleMeshError));

  WbGeometry::createWrenObjects();

  buildWrenMesh(false);

  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::optionalRenderingChanged, this,
          &WbTriangleMeshGeometry::updateOptionalRendering);
  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::lineScaleChanged, this,
          &WbTriangleMeshGeometry::updateNormalsRepresentation);

  const WbShape *shape = dynamic_cast<const WbShape *>(parentNode());
  if (shape && shape->isCastShadowsEnabled() && 3 * mTriangleMesh->numberOfTriangles() > maxIndexNumberToCastShadows()) {
    warn(tr("Too many triangles (%1) in mesh: unable to cast shadows, please reduce "
            "the number of triangles below %2 or set Shape.castShadows to "
            "FALSE to disable this warning.")
           .arg(mTriangleMesh->numberOfTriangles())
           .arg((int)(maxIndexNumberToCastShadows() / 3)));
  }
  emit wrenObjectsCreated();
}

void WbTriangleMeshGeometry::setResizeManipulatorDimensions() {
  WbVector3 scale(1.0f, 1.0f, 1.0f);

  WbTransform *transform = upperTransform();
  if (transform)
    scale *= transform->matrix().scale();

  resizeManipulator()->updateHandleScale(scale.ptr());
  updateResizeHandlesSize();
}

void WbTriangleMeshGeometry::deleteWrenRenderable() {
  if (mNormalsMaterial)
    wr_material_delete(mNormalsMaterial);
  mNormalsMaterial = NULL;
  if (mNormalsRenderable)
    wr_node_delete(WR_NODE(mNormalsRenderable));
  mNormalsRenderable = NULL;

  WbGeometry::deleteWrenRenderable();
}

void WbTriangleMeshGeometry::buildWrenMesh(bool updateCache) {
  if (updateCache) {
    WbTriangleMeshCache::releaseTriangleMesh(this);
    mMeshKey.set(this);
    WbTriangleMeshCache::useTriangleMesh(this);
  }

  const bool resizeManipulator = mResizeManipulator && mResizeManipulator->isAttached();
  deleteWrenRenderable();

  wr_static_mesh_delete(mWrenMesh);
  mWrenMesh = NULL;

  if (!mTriangleMesh->isValid())
    return;

  const bool createOutlineMesh = isInBoundingObject();

  WbGeometry::computeWrenRenderable();

  // Reattach resize manipulator
  if (mResizeManipulator) {
    mResizeManipulator->attachTo(mWrenScaleTransform);
    if (resizeManipulator)
      mResizeManipulator->show();
  }

  // normals representation
  mNormalsMaterial = wr_phong_material_new();
  wr_material_set_default_program(mNormalsMaterial, WbWrenShaders::lineSetShader());
  wr_phong_material_set_color_per_vertex(mNormalsMaterial, true);
  wr_phong_material_set_transparency(mNormalsMaterial, 0.4f);

  mNormalsRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mNormalsRenderable, false);
  wr_renderable_set_receive_shadows(mNormalsRenderable, false);
  wr_renderable_set_material(mNormalsRenderable, mNormalsMaterial, NULL);
  wr_renderable_set_visibility_flags(mNormalsRenderable, WbWrenRenderingContext::VF_NORMALS);
  wr_renderable_set_drawing_mode(mNormalsRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
  wr_transform_attach_child(wrenNode(), WR_NODE(mNormalsRenderable));

  // Restore pickable state
  setPickable(isPickable());

  WbWrenMeshBuffers *buffers = WbGeometry::createMeshBuffers(estimateVertexCount(), estimateIndexCount());
  buildGeomIntoBuffers(buffers, WbMatrix4(), !mTriangleMesh->areTextureCoordinatesValid());

  mWrenMesh = wr_static_mesh_new(buffers->verticesCount(), buffers->indicesCount(), buffers->vertexBuffer(),
                                 buffers->normalBuffer(), buffers->texCoordBuffer(), buffers->unwrappedTexCoordBuffer(),
                                 buffers->indexBuffer(), createOutlineMesh);

  delete buffers;

  wr_renderable_set_mesh(mWrenRenderable, WR_MESH(mWrenMesh));
  updateNormalsRepresentation();
}

void WbTriangleMeshGeometry::buildGeomIntoBuffers(WbWrenMeshBuffers *buffers, const WbMatrix4 &m,
                                                  bool generateUserTexCoords) const {
  assert(mTriangleMesh->isValid());
  const WbMatrix3 rm = m.extracted3x3Matrix();
  const int n = mTriangleMesh->numberOfTriangles();

  int start = buffers->vertexIndex() / 3;
  float *vBuf = buffers->vertexBuffer();
  if (vBuf) {
    int i = buffers->vertexIndex();
    for (int t = 0; t < n; ++t) {    // foreach triangle
      for (int v = 0; v < 3; ++v) {  // foreach vertex
        WbWrenMeshBuffers::writeCoordinates(mTriangleMesh->vertex(t, v, 0), mTriangleMesh->vertex(t, v, 1),
                                            mTriangleMesh->vertex(t, v, 2), m, vBuf, i);
        i += 3;
      }
    }
  }
  float *nBuf = buffers->normalBuffer();
  if (nBuf) {
    int i = buffers->vertexIndex();
    for (int t = 0; t < n; ++t) {    // foreach triangle
      for (int v = 0; v < 3; ++v) {  // foreach vertex
        WbWrenMeshBuffers::writeNormal(mTriangleMesh->normal(t, v, 0), mTriangleMesh->normal(t, v, 1),
                                       mTriangleMesh->normal(t, v, 2), rm, nBuf, i);
        i += 3;
      }
    }
  }
  float *tBuf = buffers->texCoordBuffer();
  float *utBuf = buffers->unwrappedTexCoordBuffer();
  if (tBuf) {
    int i = start * buffers->texCoordSetsCount() * 2;
    for (int t = 0; t < n; ++t) {    // foreach triangle
      for (int v = 0; v < 3; ++v) {  // foreach vertex
        tBuf[i] = mTriangleMesh->textureCoordinate(t, v, 0);
        tBuf[i + 1] = mTriangleMesh->textureCoordinate(t, v, 1);

        if (generateUserTexCoords) {
          utBuf[i] = mTriangleMesh->nonRecursiveTextureCoordinate(t, v, 0);
          utBuf[i + 1] = mTriangleMesh->nonRecursiveTextureCoordinate(t, v, 1);
        } else {
          utBuf[i] = mTriangleMesh->textureCoordinate(t, v, 0);
          utBuf[i + 1] = mTriangleMesh->textureCoordinate(t, v, 1);
        }
        i += 2;
      }
    }
  }
  unsigned int *iBuf = buffers->indexBuffer();
  if (iBuf) {
    start = buffers->vertexIndex() / 3;
    int i = buffers->index();
    for (int t = 0; t < n; ++t) {  // foreach triangle
      for (int v = 0; v < 3; ++v)  // foreach vertex
        iBuf[i++] = start + mTriangleMesh->index(t, v);
    }
    buffers->setIndex(i);
  }
  buffers->setVertexIndex(buffers->vertexIndex() + estimateVertexCount() * 3);
}

int WbTriangleMeshGeometry::estimateVertexCount(bool isOutlineMesh) const {
  assert(mTriangleMesh->isValid());
  return 3 * mTriangleMesh->numberOfTriangles();
}

int WbTriangleMeshGeometry::estimateIndexCount(bool isOutlineMesh) const {
  assert(mTriangleMesh->isValid());
  return 3 * mTriangleMesh->numberOfTriangles();
}

/////////////////
// ODE objects //
/////////////////

// works only for meshes made up of triangles
dGeomID WbTriangleMeshGeometry::createOdeGeom(dSpaceID space) {
  if (!isPreFinalizedCalled())  // needed because preFinalize comes after insertion and insertion triggers ODE dGeom creation
    preFinalize();

  if (!mTriangleMesh->isValid()) {
    clearTrimeshResources();  // delete trimesh resources if any
    return NULL;
  }

  if (WbNodeUtilities::findUpperMatter(this)->nodeType() == WB_NODE_FLUID)
    checkFluidBoundingObjectOrientation();

  setOdeTrimeshData();

  dGeomID g = dCreateTriMesh(space, mTrimeshData, NULL, NULL, NULL);
  assert(g);
  return g;
}

void WbTriangleMeshGeometry::setOdeTrimeshData() {
  assert(mTriangleMesh->isValid());
  clearTrimeshResources();  // delete trimesh resources if any
  const int n = mTriangleMesh->numberOfVertices();
  const int nt = mTriangleMesh->numberOfTriangles();
  mTrimeshData = dGeomTriMeshDataCreate();
  mScaledCoordinatesNeedUpdate = true;
  updateScaledCoordinates();
  dGeomTriMeshDataBuildDouble(mTrimeshData, mTriangleMesh->scaledCoordinatesData(), 3 * sizeof(double), n,
                              mTriangleMesh->indicesData(), 3 * nt, 3 * sizeof(int));
}

// works only for meshes made up of triangles
void WbTriangleMeshGeometry::applyToOdeData(bool correctSolidMass) {
  mCorrectSolidMass = correctSolidMass;

  if (mTriangleMesh->isValid() == false)
    return;

  setOdeTrimeshData();
  if (mOdeGeom == NULL) {
    if (areOdeObjectsCreated())
      emit validTriangleMeshGeometryInserted();

    return;
  }

  assert(mTriangleMesh->isValid());
  assert(dGeomGetClass(mOdeGeom) == dTriMeshClass);

  dGeomTriMeshSetData(mOdeGeom, mTrimeshData);

  WbOdeGeomData *const odeGeomData = static_cast<WbOdeGeomData *>(dGeomGetData(mOdeGeom));
  assert(odeGeomData);
  odeGeomData->setLastChangeTime(WbSimulationState::instance()->time());

  if (mCorrectSolidMass)
    applyToOdeMass();

  mIsOdeDataApplied = true;
}

void WbTriangleMeshGeometry::updateOdeData() {
  if (mIsOdeDataApplied)
    applyToOdeData(mCorrectSolidMass);
}

bool WbTriangleMeshGeometry::isSuitableForInsertionInBoundingObject(bool warning) const {
  return true;
}

bool WbTriangleMeshGeometry::isAValidBoundingObject(bool checkOde, bool warning) const {
  return mTriangleMesh->isValid() && WbGeometry::isAValidBoundingObject(checkOde, warning);
}
/////////////////
// Ray tracing //
/////////////////

WbVector2 WbTriangleMeshGeometry::nonRecursiveTextureSizeFactor() const {
  if (mTriangleMesh->areTextureCoordinatesValid())
    // user-specified mapping
    return WbVector2(1, 1);

  // default
  return WbVector2(4, 2);
}

bool WbTriangleMeshGeometry::pickUVCoordinate(WbVector2 &uv, const WbRay &ray, int textureCoordSet) const {
  WbVector3 localCollisionPoint;
  int t;
  bool collisionExists = computeLocalCollisionPoint(localCollisionPoint, t, ray);
  if (!collisionExists)
    return false;

  WbVector3 v0(mTriangleMesh->vertex(t, 0, 0), mTriangleMesh->vertex(t, 0, 1), mTriangleMesh->vertex(t, 0, 2));
  WbVector3 v1(mTriangleMesh->vertex(t, 1, 0), mTriangleMesh->vertex(t, 1, 1), mTriangleMesh->vertex(t, 1, 2));
  WbVector3 v2(mTriangleMesh->vertex(t, 2, 0), mTriangleMesh->vertex(t, 2, 1), mTriangleMesh->vertex(t, 2, 2));

  const WbTransform *const transform = upperTransform();
  if (transform) {
    const WbMatrix4 &m = transform->matrix();
    v0 = m * v0;
    v1 = m * v1;
    v2 = m * v2;
  }

  double u, v;
  ray.intersects(v0, v1, v2, false, u, v);
  WbVector2 tc0, tc1, tc2;
  if (textureCoordSet == 0 || mTriangleMesh->areTextureCoordinatesValid()) {
    tc0.setXy(mTriangleMesh->textureCoordinate(t, 0, 0), mTriangleMesh->textureCoordinate(t, 0, 1));
    tc1.setXy(mTriangleMesh->textureCoordinate(t, 1, 0), mTriangleMesh->textureCoordinate(t, 1, 1));
    tc2.setXy(mTriangleMesh->textureCoordinate(t, 2, 0), mTriangleMesh->textureCoordinate(t, 2, 1));
  } else {  // textureCoordSet == 1 and no user-specified mapping
    tc0.setXy(mTriangleMesh->nonRecursiveTextureCoordinate(t, 0, 0), mTriangleMesh->nonRecursiveTextureCoordinate(t, 0, 1));
    tc1.setXy(mTriangleMesh->nonRecursiveTextureCoordinate(t, 1, 0), mTriangleMesh->nonRecursiveTextureCoordinate(t, 1, 1));
    tc2.setXy(mTriangleMesh->nonRecursiveTextureCoordinate(t, 2, 0), mTriangleMesh->nonRecursiveTextureCoordinate(t, 2, 1));
  }
  uv = (1 - u - v) * tc0 + u * tc1 + v * tc2;

  return true;
}

double WbTriangleMeshGeometry::computeDistance(const WbRay &ray) const {
  WbVector3 localCollisionPoint;
  int triangleIndex;
  return computeLocalCollisionPoint(localCollisionPoint, triangleIndex, ray);
}

double WbTriangleMeshGeometry::computeLocalCollisionPoint(WbVector3 &point, int &triangleIndex, const WbRay &ray) const {
  if (!mTriangleMesh)
    return false;

  WbRay localRay(ray);
  WbTransform *transform = upperTransform();
  if (transform) {
    localRay.setDirection(ray.direction() * transform->matrix());
    WbVector3 origin = transform->matrix().pseudoInversed(ray.origin());
    origin /= absoluteScale();
    localRay.setOrigin(origin);
    localRay.normalize();
  }

  int nTriangles = mTriangleMesh->numberOfTriangles();
  double closestDistance = std::numeric_limits<double>::infinity();
  bool found = false;
  updateScaledCoordinates();
  for (int t = 0; t < nTriangles; ++t) {
    WbVector4 v0(mTriangleMesh->scaledVertex(t, 0, 0), mTriangleMesh->scaledVertex(t, 0, 1),
                 mTriangleMesh->scaledVertex(t, 0, 2), 1.0);
    WbVector4 v1(mTriangleMesh->scaledVertex(t, 1, 0), mTriangleMesh->scaledVertex(t, 1, 1),
                 mTriangleMesh->scaledVertex(t, 1, 2), 1.0);
    WbVector4 v2(mTriangleMesh->scaledVertex(t, 2, 0), mTriangleMesh->scaledVertex(t, 2, 1),
                 mTriangleMesh->scaledVertex(t, 2, 2), 1.0);

    double u, v;
    std::pair<bool, double> result = localRay.intersects(v0.toVector3(), v1.toVector3(), v2.toVector3(), true, u, v);
    if (result.first && result.second > 0.0 && result.second < closestDistance) {
      found = true;
      closestDistance = result.second;
      triangleIndex = t;
    }
  }

  if (found) {
    point = localRay.origin() + closestDistance * localRay.direction();
    return closestDistance;
  }

  return -1;
}

void WbTriangleMeshGeometry::recomputeBoundingSphere() const {
  assert(mBoundingSphere);
  mBoundingSphere->empty();
  if (mTriangleMesh->numberOfTriangles() == 0)
    return;

  // Ritter's bounding sphere approximation:
  // 1. Pick a point x from P, search a point y in P, which has the largest distance from x;
  // 2. Search a point z in P, which has the largest distance from y. set up an
  //    initial sphere B, with its centre as the midpoint of y and z, the radius as
  //    half of the distance between y and z;
  // 3. If all points in P are within sphere B, then we get a bounding sphere.
  //    Otherwise, let p be a point outside the sphere, construct a new sphere covering
  //    both point p and previous sphere. Repeat this step until all points are covered.
  // Note that steps 1. and 2. help in computing a better fitting (smaller) sphere by
  // estimating the center of the final sphere and thus reducing the bias due to the enclosed
  // vertices order.
  const int nbTriangles = mTriangleMesh->numberOfTriangles();
  WbVector3 p2(mTriangleMesh->vertex(0, 0, 0), mTriangleMesh->vertex(0, 0, 1), mTriangleMesh->vertex(0, 0, 2));
  WbVector3 p1;
  double maxDistance;  // squared distance
  for (int i = 0; i < 2; ++i) {
    maxDistance = 0.0;
    p1 = p2;
    for (int t = 0; t < nbTriangles; ++t) {
      for (int v = 0; v < 3; ++v) {
        const WbVector3 point(mTriangleMesh->vertex(t, v, 0), mTriangleMesh->vertex(t, v, 1), mTriangleMesh->vertex(t, v, 2));
        const double d = p1.distance2(point);
        if (d > maxDistance) {
          maxDistance = d;
          p2 = point;
        }
      }
    }
  }
  mBoundingSphere->set((p2 + p1) * 0.5, sqrt(maxDistance) * 0.5);

  for (int t = 0; t < nbTriangles; ++t) {
    for (int v = 0; v < 3; ++v) {
      const WbVector3 point(mTriangleMesh->vertex(t, v, 0), mTriangleMesh->vertex(t, v, 1), mTriangleMesh->vertex(t, v, 2));
      mBoundingSphere->enclose(point);
    }
  }
}

void WbTriangleMeshGeometry::updateScaledCoordinates() const {
  if (mScaledCoordinatesNeedUpdate) {
    const WbVector3 &s = absoluteScale();
    mTriangleMesh->updateScaledCoordinates(s.x(), s.y(), s.z());
    mScaledCoordinatesNeedUpdate = false;
    return;
  }
}

void WbTriangleMeshGeometry::setScaleNeedUpdate() {
  mScaledCoordinatesNeedUpdate = true;
}

void WbTriangleMeshGeometry::updateOptionalRendering(int option) {
  if (option != WbWrenRenderingContext::VF_NORMALS)
    return;

  updateNormalsRepresentation();
}

void WbTriangleMeshGeometry::updateNormalsRepresentation() {
  if (mNormalsMesh) {
    wr_static_mesh_delete(mNormalsMesh);
    mNormalsMesh = NULL;
    wr_renderable_set_mesh(mNormalsRenderable, NULL);
  }

  if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_NORMALS) && mTriangleMesh) {
    QVector<float> vertices;
    QVector<float> colors;
    const int n = mTriangleMesh->numberOfTriangles();
    const double linescale = WbWorld::instance()->worldInfo()->lineScale();
    for (int t = 0; t < n; ++t) {    // foreach triangle
      for (int v = 0; v < 3; ++v) {  // foreach vertex
        const double x = mTriangleMesh->vertex(t, v, 0);
        const double y = mTriangleMesh->vertex(t, v, 1);
        const double z = mTriangleMesh->vertex(t, v, 2);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
        vertices.push_back(x + linescale * mTriangleMesh->normal(t, v, 0));
        vertices.push_back(y + linescale * mTriangleMesh->normal(t, v, 1));
        vertices.push_back(z + linescale * mTriangleMesh->normal(t, v, 2));

        float color[3] = {1.0, 0.0, 0.0};
        if (mTriangleMesh->isNormalCreased(t, v))
          color[1] = 1.0;
        else
          color[2] = 1.0;

        for (int i = 0; i < 2; ++i) {
          colors.append(color[0]);
          colors.append(color[1]);
          colors.append(color[2]);
        }
      }
    }

    if (vertices.size() > 0) {
      mNormalsMesh = wr_static_mesh_line_set_new(vertices.size() / 3, vertices.data(), colors.data());
      wr_renderable_set_mesh(mNormalsRenderable, WR_MESH(mNormalsMesh));
    }
  }
}

/////////////////////////////////////////////////////////////
//  WREN related methods for resizing by pulling handles   //
/////////////////////////////////////////////////////////////

double WbTriangleMeshGeometry::max(int coordinate) const {
  return mTriangleMesh->max(coordinate);
}

double WbTriangleMeshGeometry::min(int coordinate) const {
  return mTriangleMesh->min(coordinate);
}

bool WbTriangleMeshGeometry::exportNodeHeader(WbVrmlWriter &writer) const {
  if (!writer.isX3d())
    return WbGeometry::exportNodeHeader(writer);

  // reduce the number of exported TriangleMeshGeometrys by automatically
  // using a def-use based on the mesh hash
  writer << "<" << x3dName() << " id=\'n" << QString::number(uniqueId()) << "\'";
  if (writer.indexedFaceSetDefMap().contains(mMeshKey.mHash)) {
    writer << " USE=\'" + writer.indexedFaceSetDefMap().value(mMeshKey.mHash) + "\'></" + x3dName() + ">";
    return true;
  }

  if (cTriangleMeshMap.at(mMeshKey).mNumUsers > 1)
    writer.indexedFaceSetDefMap().insert(mMeshKey.mHash, QString::number(uniqueId()));
  return false;
}

void WbTriangleMeshGeometry::exportNodeContents(WbVrmlWriter &writer) const {
  // before exporting the vertex, normal and texture coordinates, we
  // need to remove duplicates from the arrays to save space in the
  // saved file and adapt the indexes consequently

  // export the original loaded mesh if we're not writing to X3D
  if (!writer.isX3d()) {
    WbNode::exportNodeContents(writer);
    return;
  }

  // To avoid differences due to normal computations export the computed triangle mesh.
  const int n = mTriangleMesh->numberOfTriangles();
  const int n3 = n * 3;
  int *const coordIndex = new int[n3];
  int *const normalIndex = new int[n3];
  int *const texCoordIndex = new int[n3];
  double *const vertex = new double[n * 9];
  double *const normal = new double[n * 9];
  double *const texture = new double[n * 6];
  int indexCount = 0;
  int vertexCount = 0;
  int normalCount = 0;
  int textureCount = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 3; ++j) {
      const double x = mTriangleMesh->vertex(i, j, 0);
      const double y = mTriangleMesh->vertex(i, j, 1);
      const double z = mTriangleMesh->vertex(i, j, 2);
      bool found = false;
      for (int l = 0; l < vertexCount; ++l) {
        const int k = 3 * l;
        if (vertex[k] == x && vertex[k + 1] == y && vertex[k + 2] == z) {
          coordIndex[indexCount] = l;
          found = true;
          break;
        }
      }
      if (!found) {
        const int v = 3 * vertexCount;
        vertex[v] = x;
        vertex[v + 1] = y;
        vertex[v + 2] = z;
        coordIndex[indexCount] = vertexCount;
        ++vertexCount;
      }
      const double nx = mTriangleMesh->normal(i, j, 0);
      const double ny = mTriangleMesh->normal(i, j, 1);
      const double nz = mTriangleMesh->normal(i, j, 2);
      found = false;
      for (int l = 0; l < normalCount; ++l) {
        const int k = 3 * l;
        if (normal[k] == nx && normal[k + 1] == ny && normal[k + 2] == nz) {
          normalIndex[indexCount] = l;
          found = true;
          break;
        }
      }
      if (!found) {
        const int v = 3 * normalCount;
        normal[v] = nx;
        normal[v + 1] = ny;
        normal[v + 2] = nz;
        normalIndex[indexCount] = normalCount;
        ++normalCount;
      }

      const double tu = mTriangleMesh->textureCoordinate(i, j, 0);
      const double tv = mTriangleMesh->textureCoordinate(i, j, 1);
      found = false;
      for (int l = 0; l < textureCount; ++l) {
        const int k = 2 * l;
        if (texture[k] == tu && texture[k + 1] == tv) {
          texCoordIndex[indexCount] = l;
          found = true;
          break;
        }
      }
      if (!found) {
        const int v = 2 * textureCount;
        texture[v] = tu;
        texture[v + 1] = tv;
        texCoordIndex[indexCount] = textureCount;
        ++textureCount;
      }
      ++indexCount;
    }
  }

  const WbField *solidField = findField("solid", true);
  if (solidField)
    solidField->write(writer);

  if (mTriangleMesh && !mTriangleMesh->areTextureCoordinatesValid())
    // notify three.js if a default mapping is used to prevent issue https://github.com/cyberbotics/webots/issues/752
    writer << " defaultMapping=\'true\'";

  writer << " coordIndex=\'";

  for (int i = 0; i < indexCount; ++i) {
    if (i != 0) {
      writer << " ";
      if (i % 3 == 0)
        writer << "-1 ";
    }
    writer << coordIndex[i];
  }

  writer << " -1\'";
  writer << " normalIndex=\'";
  for (int i = 0; i < indexCount; ++i) {
    if (i != 0) {
      writer << " ";
      if (i % 3 == 0)
        writer << "-1 ";
    }
    writer << normalIndex[i];
  }
  writer << " -1\'";

  writer << " texCoordIndex=\'";
  for (int i = 0; i < indexCount; ++i) {
    if (i != 0) {
      writer << " ";
      if (i % 3 == 0)
        writer << "-1 ";
    }
    writer << texCoordIndex[i];
  }
  writer << " -1\'";

  writer << ">";  // end of fields, beginning of nodes

  writer << "<Coordinate point=\'";
  const int precision = 4;
  for (int i = 0; i < vertexCount; ++i) {
    if (i != 0)
      writer << ", ";
    const int j = 3 * i;
    writer << QString::number(vertex[j], 'f', precision)
           << " "  // write with limited precision to reduce the size of the X3D/HTML file
           << QString::number(vertex[j + 1], 'f', precision) << " " << QString::number(vertex[j + 2], 'f', precision);
  }

  writer << "\'></Coordinate>";

  writer << "<Normal vector=\'";
  for (int i = 0; i < normalCount; ++i) {
    if (i != 0)
      writer << ", ";
    const int j = 3 * i;
    writer << QString::number(normal[j], 'f', precision) << " " << QString::number(normal[j + 1], 'f', precision) << " "
           << QString::number(normal[j + 2], 'f', precision);
  }
  writer << "\'></Normal>";

  writer << "<TextureCoordinate point=\'";
  for (int i = 0; i < textureCount; ++i) {
    if (i != 0)
      writer << ", ";
    const int j = 2 * i;
    writer << QString::number(texture[j], 'f', precision) << " " << QString::number(1.0 - texture[j + 1], 'f', precision);
  }
  writer << "\'></TextureCoordinate>";

  delete[] coordIndex;
  delete[] normalIndex;
  delete[] texCoordIndex;
  delete[] vertex;
  delete[] normal;
  delete[] texture;
}

////////////////////////
// Friction Direction //
////////////////////////

WbVector3 WbTriangleMeshGeometry::computeFrictionDirection(const WbVector3 &normal) const {
  parsingWarn(tr("A %1 is used in a Bounding object using an asymmetric friction. %1 does not support "
                 "asymmetric friction")
                .arg(nodeModelName()));
  return WbVector3(0, 0, 0);
}
