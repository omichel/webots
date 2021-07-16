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

#include "WbPaintTexture.hpp"

#include "WbAppearance.hpp"
#include "WbBoundingSphere.hpp"
#include "WbGeometry.hpp"
#include "WbImageTexture.hpp"
#include "WbIndexedFaceSet.hpp"
#include "WbLog.hpp"
#include "WbMathsUtilities.hpp"
#include "WbPbrAppearance.hpp"
#include "WbPerformanceLog.hpp"
#include "WbRgb.hpp"
#include "WbShape.hpp"
#include "WbWorld.hpp"
#include "WbWrenRenderingContext.hpp"

#include <wren/drawable_texture.h>
#include <wren/material.h>
#include <wren/texture.h>

static QList<WbPaintTexture *> gPaintTextures;
static QList<WbPaintTexture *> gEvaporationTextures;

// Min & max texture sizes (when no texture is attached to material, size has to be computed)
static const WbVector2 MAX_TEXTURE_SIZE(1024, 1024);
static const WbVector2 MIN_TEXTURE_SIZE(32, 32);

WbPaintTexture *WbPaintTexture::findPaintTexture(const WbShape *shape) {
  foreach (WbPaintTexture *texture, gPaintTextures) {
    if (texture->mShape == shape)
      return texture;
  }

  return NULL;
}

WbPaintTexture *WbPaintTexture::paintTexture(const WbShape *shape) {
  WbPaintTexture *paintTexture = findPaintTexture(shape);
  if (paintTexture)
    return paintTexture;  // already exists

  // check if it is possible to paint on the shape
  if (isPaintable(shape))
    return new WbPaintTexture(shape);

  return NULL;
}

bool WbPaintTexture::isPaintable(const WbShape *shape) {
  return shape->geometry();
}

void WbPaintTexture::init() {
  cleanup();
}

void WbPaintTexture::cleanup() {
  QList<WbPaintTexture *>::iterator it = gPaintTextures.begin();
  while (it != gPaintTextures.end()) {
    delete *it;
    ++it;
  }

  gPaintTextures.clear();
  gEvaporationTextures.clear();
}

void WbPaintTexture::clearAllTextures() {
  foreach (WbPaintTexture *const paintTexture, gPaintTextures)
    paintTexture->clearTexture();
  gEvaporationTextures.clear();
}

void WbPaintTexture::clearTexture() {
  int size = mTextureSize.x() * mTextureSize.y();

  for (int i = 0; i < size; i++) {
    int k = 4 * i;
    mData[k] = 1.0f;
    mData[++k] = 1.0f;
    mData[++k] = 1.0f;
    mData[++k] = 0.0f;
  }
  wr_drawable_texture_clear(mTexture);

  if (mEvaporation)
    memset(mEvaporation, 0, size * sizeof(double));
}

WbPaintTexture::WbPaintTexture(const WbShape *shape) : mShape(shape), mEvaporation(NULL) {
  mTexture = wr_drawable_texture_new();

  // add painting layer
  WbAppearance *appearance = shape->appearance();
  WbPbrAppearance *pbrAppearance = shape->pbrAppearance();
  if (appearance && appearance->texture() && appearance->texture()->wrenTexture())
    mTextureSize = computeTextureSize(appearance->texture()->width(), appearance->texture()->height());
  else if (pbrAppearance && pbrAppearance->baseColorMap() && pbrAppearance->baseColorMap()->wrenTexture())
    mTextureSize = computeTextureSize(pbrAppearance->baseColorMap()->width(), pbrAppearance->baseColorMap()->height());
  else {
    const WbVector2 size = computeDefaultTextureSize();
    mTextureSize = computeTextureSize(size.x(), size.y());
  }

  wr_texture_set_size(WR_TEXTURE(mTexture), mTextureSize.x(), mTextureSize.y());
  wr_texture_setup(WR_TEXTURE(mTexture));

  const int penTextureIndex = shape->wrenMaterial()->type == WR_MATERIAL_PHONG ? 1 : 8;

  wr_material_set_texture(shape->wrenMaterial(), WR_TEXTURE(mTexture), penTextureIndex);
  wr_material_set_texture_anisotropy(shape->wrenMaterial(), 0, penTextureIndex);
  wr_material_set_texture_enable_interpolation(shape->wrenMaterial(), false, penTextureIndex);
  wr_material_set_texture_enable_mip_maps(shape->wrenMaterial(), false, penTextureIndex);

  // initialize paint texture
  int size = mTextureSize.x() * mTextureSize.y();
  mData = new float[4 * size];
  for (int i = 0; i < size; i++) {
    int k = 4 * i;
    mData[k] = 1.0f;
    mData[++k] = 1.0f;
    mData[++k] = 1.0f;
    mData[++k] = 0.0f;
  }

  // initialize evaporations
  if (WbWorld::instance()->worldInfo()->inkEvaporation() > 0.0) {
    mEvaporation = new double[size];
    memset(mEvaporation, 0, size * sizeof(double));
  }

  gPaintTextures.append(this);

  connect(shape, &WbShape::wrenMaterialChanged, this, &WbPaintTexture::restoreWrenTexture);
}

WbPaintTexture::~WbPaintTexture() {
  wr_texture_delete(WR_TEXTURE(mTexture));
  delete[] mData;
  if (mEvaporation)
    delete[] mEvaporation;
}

void WbPaintTexture::prePhysicsStep(double ms) {
  WbWorldInfo *wi = WbWorld::instance()->worldInfo();
  double ie = wi->inkEvaporation();

  if (ie) {
    double factor = exp(-ie * ms / 1000.0);
    for (int i = gEvaporationTextures.size() - 1; i >= 0; --i) {
      bool isDone = true;
      gEvaporationTextures[i]->evaporateInk(factor, isDone);
      if (isDone)
        gEvaporationTextures.removeAt(i);
    }
  }
}

void WbPaintTexture::restoreWrenTexture() {
  wr_material_set_texture(mShape->wrenMaterial(), WR_TEXTURE(mTexture), 1);
}

void WbPaintTexture::evaporateInk(double evaporationFactor, bool &isDone) {
  assert(mEvaporation);

  const int w = mTextureSize.x();
  const int h = mTextureSize.y();

  isDone = true;
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const int n = y * w + x;
      const int m = 4 * n;
      if (mEvaporation[n] > 0.001) {
        mEvaporation[n] *= evaporationFactor;
        mData[m + 3] = static_cast<float>(mEvaporation[n]);
        wr_drawable_texture_set_color(mTexture, &mData[m]);
        wr_drawable_texture_draw_pixel(mTexture, x, y);
        isDone = false;
      }
    }
  }
}

void WbPaintTexture::paint(const WbRay &ray, float leadSize, const WbRgb &color, float density) {
  WbVector2 uv;
  bool validCollision = mShape->geometry()->pickUVCoordinate(uv, ray, 1);
  if (!validCollision)
    return;  // no valid collision

  if (uv.x() > 1.0 || uv.x() < 0.0 || uv.y() > 1.0 || uv.y() < 0.0)
    return;  // outside

  const int w = mTextureSize.x();
  const int h = mTextureSize.y();
  const int x = uv.x() * w;
  const int y = uv.y() * h;

  WbBoundingSphere *bs = mShape->geometry()->boundingSphere();
  bs->recomputeIfNeeded();
  int size = static_cast<int>(leadSize * mOriginalTextureSize.x() / bs->radius());
  if (size < 1)
    size = 1;

  const int halfSize = size / 2;

  int ox = x - halfSize;
  int oy = y - halfSize;
  if (ox < 0)
    ox = 0;

  if (oy < 0)
    oy = 0;

  int sx = x + halfSize;
  int sy = y + halfSize;
  if (sx >= w)
    sx = w - 1;

  if (sy >= h)
    sy = h - 1;

  if (mEvaporation && !gEvaporationTextures.contains(this))
    gEvaporationTextures.append(this);

  const int halfSize2 = (halfSize * halfSize);
  for (int ty = oy; ty <= sy; ++ty) {
    int rowOffset = ty * w;
    int dataIndex = (rowOffset + ox) << 2;
    int circleCondition = halfSize2 - (ty - y) * (ty - y);
    for (int tx = ox; tx <= sx; ++tx) {
      if ((tx - x) * (tx - x) <= circleCondition) {
        const float previousDensity = mData[dataIndex + 3];
        const float oldDensityRatio = density + previousDensity < 1e-15 ? 0.0f : previousDensity / (density + previousDensity);
        mData[dataIndex] = oldDensityRatio * mData[dataIndex] + (1.0f - oldDensityRatio) * color.blue();
        mData[dataIndex + 1] = oldDensityRatio * mData[dataIndex + 1] + (1.0f - oldDensityRatio) * color.green();
        mData[dataIndex + 2] = oldDensityRatio * mData[dataIndex + 2] + (1.0f - oldDensityRatio) * color.red();
        mData[dataIndex + 3] += density;
        if (mData[dataIndex + 3] > 1.0f)
          mData[dataIndex + 3] = 1.0f;

        if (mEvaporation) {
          mEvaporation[rowOffset + tx] += (double)density;
          if (mEvaporation[rowOffset + tx] > 1.0f)
            mEvaporation[rowOffset + tx] = 1.0f;
        }

        wr_drawable_texture_set_color(mTexture, &mData[dataIndex]);
        wr_drawable_texture_draw_pixel(mTexture, tx, ty);
      }

      dataIndex += 4;  // compute index of the next pixel
    }
  }
}

void WbPaintTexture::pickColor(const WbVector2 &uv, WbRgb &pickedColor, float *pickedDensity) const {
  const int w = mTextureSize.x();
  const int h = mTextureSize.y();
  int x = uv.x() * w;
  int y = uv.y() * h;
  while (x < 0)
    x += w;
  while (y < 0)
    y += h;
  while (x >= w)
    x -= w;
  while (y >= h)
    y -= h;

  const int index = (y * w + x) * 4;
  pickedColor.setValue(mData[index + 2], mData[index + 1], mData[index]);
  if (pickedDensity != NULL)
    *pickedDensity = mData[index + 3];
}

WbVector2 WbPaintTexture::computeTextureSize(int imageTextureWidth, int imageTextureHeight) {
  const int width = imageTextureWidth;
  const int height = imageTextureHeight;
  mOriginalTextureSize.setXy(imageTextureWidth, imageTextureHeight);

  const WbVector2 sizeFactor = mShape->geometry()->nonRecursiveTextureSizeFactor();
  return WbVector2(width * sizeFactor.x(), height * sizeFactor.y());
}

WbVector2 WbPaintTexture::computeDefaultTextureSize() {
  // Get max scale factor
  const WbVector3 scale = mShape->geometry()->absoluteScale();
  double maxScale = scale.x() > scale.y() ? scale.x() : scale.y();
  if (scale.z() > maxScale)
    maxScale = scale.z();

  // Compute size based on bounding sphere radius
  // If the sphere radius is greater than 10 (arbitrary value), then we use the max size
  const float sphereRadius = maxScale * mShape->geometry()->boundingSphere()->radius();
  const WbVector2 size =
    sphereRadius > 10.0 ? MAX_TEXTURE_SIZE : MIN_TEXTURE_SIZE + (MAX_TEXTURE_SIZE - MIN_TEXTURE_SIZE) * sphereRadius / 10.0;

  // Return closest power of two dimensions
  const WbVector2 nextPowerOf2Size(WbMathsUtilities::nextPowerOf2(size.x()), WbMathsUtilities::nextPowerOf2(size.y()));
  const WbVector2 previousPowerOf2Size = nextPowerOf2Size / 2;

  if (abs(nextPowerOf2Size.x() - size.x()) > abs(previousPowerOf2Size.x() - size.x()))
    return previousPowerOf2Size;
  else
    return nextPowerOf2Size;
}
