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

#include "Scene.hpp"

#include "Camera.hpp"
#include "ColorUtils.hpp"
#include "Config.hpp"
#include "Constants.hpp"
#include "ContainerUtils.hpp"
#include "Debug.hpp"
#include "DirectionalLight.hpp"
#include "GlState.hpp"
#include "GlUser.hpp"
#include "Id.hpp"
#include "Material.hpp"
#include "Node.hpp"
#include "Overlay.hpp"
#include "PbrMaterial.hpp"
#include "PhongMaterial.hpp"
#include "PointLight.hpp"
#include "Renderable.hpp"
#include "ShaderProgram.hpp"
#include "ShadowVolumeCaster.hpp"
#include "SpotLight.hpp"
#include "Transform.hpp"
#include "UniformBuffer.hpp"
#include "Viewport.hpp"

#include <wren/renderable.h>
#include <wren/scene.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <algorithm>
#include <memory>

#define INVERSE_LOG2 1.442695

namespace wren {

  Scene *Scene::cInstance = NULL;

  Scene *Scene::instance() {
    if (!Scene::cInstance)
      Scene::cInstance = new Scene();

    return Scene::cInstance;
  }

  void Scene::init() {
    if (glstate::isInitialized())
      return;

    glstate::init();
  }

  void Scene::applyPendingUpdates() {
    // Apply OpenGL state changes
    GlUser::applyGl();
  }

  void Scene::reset() {
    IdPhongMaterial::resetCounter();
    IdPbrMaterial::resetCounter();
    IdMesh::resetCounter();

    mDirectionalLightsActive.clear();
    mPointLightsActive.clear();
    mSpotLightsActive.clear();

    mDirectionalLightsInactive.clear();
    mPointLightsInactive.clear();
    mSpotLightsInactive.clear();

    // Apply modifications before reset
    GlUser::applyGl();

    debug::printCacheContents();

    // Check for leaks
    assert(!PhongMaterial::cachedItemCount());
    assert(!PbrMaterial::cachedItemCount());
    assert(!StaticMesh::cachedItemCount());
    assert(!Texture2d::cachedItemCount());
  }

  void Scene::getMainBuffer(int width, int height, unsigned int format, unsigned int data_type, unsigned int buffer_type,
                            void *buffer) {
    assert(buffer_type == GL_FRONT || buffer_type == GL_BACK);
    glstate::bindFrameBuffer(0);
    glReadBuffer(buffer_type);
    glReadPixels(0, 0, width, height, format, data_type, buffer);
  }

  void Scene::initFrameCapture(int pixelBufferCount, unsigned int *pixelBufferIds, int frameSize) {
    mPixelBufferCount = pixelBufferCount;
    mPixelBufferIds = pixelBufferIds;
    glGenBuffers(mPixelBufferCount, mPixelBufferIds);
    for (int i = 0; i < mPixelBufferCount; ++i) {
      glBindBuffer(GL_PIXEL_PACK_BUFFER, mPixelBufferIds[i]);
      glBufferData(GL_PIXEL_PACK_BUFFER, frameSize, NULL, GL_STREAM_READ);
    }
  }

  void Scene::bindPixelBuffer(int buffer) { glBindBuffer(GL_PIXEL_PACK_BUFFER, buffer); }

  void *Scene::mapPixelBuffer(unsigned int accessMode) {
#ifdef __EMSCRIPTEN__
    return NULL;
#else
    return glMapBuffer(GL_PIXEL_PACK_BUFFER, accessMode);
#endif
  }
  void Scene::unMapPixelBuffer() { glUnmapBuffer(GL_PIXEL_PACK_BUFFER); }

  void Scene::terminateFrameCapture() {
    glDeleteBuffers(mPixelBufferCount, mPixelBufferIds);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    mPixelBufferCount = 0;
    mPixelBufferIds = NULL;
  }

  void Scene::addLight(LightNode *light) {
    switch (light->type()) {
      case LightNode::TYPE_DIRECTIONAL:
        if (light->on())
          mDirectionalLightsActive.push_back(reinterpret_cast<DirectionalLight *>(light));
        else
          mDirectionalLightsInactive.push_back(reinterpret_cast<DirectionalLight *>(light));
        break;
      case LightNode::TYPE_POINT:
        if (light->on())
          mPointLightsActive.push_back(reinterpret_cast<PointLight *>(light));
        else
          mPointLightsInactive.push_back(reinterpret_cast<PointLight *>(light));
        break;
      case LightNode::TYPE_SPOT:
        if (light->on())
          mSpotLightsActive.push_back(reinterpret_cast<SpotLight *>(light));
        else
          mSpotLightsInactive.push_back(reinterpret_cast<SpotLight *>(light));
        break;
      default:
        assert(false);
    }
  }

  void Scene::removeLight(LightNode *light) {
    switch (light->type()) {
      case LightNode::TYPE_DIRECTIONAL:
        if (light->on())
          containerutils::removeElementFromVector(mDirectionalLightsActive, reinterpret_cast<DirectionalLight *>(light));
        else
          containerutils::removeElementFromVector(mDirectionalLightsInactive, reinterpret_cast<DirectionalLight *>(light));
        break;
      case LightNode::TYPE_POINT:
        if (light->on())
          containerutils::removeElementFromVector(mPointLightsActive, reinterpret_cast<PointLight *>(light));
        else
          containerutils::removeElementFromVector(mPointLightsInactive, reinterpret_cast<PointLight *>(light));
        break;
      case LightNode::TYPE_SPOT:
        if (light->on())
          containerutils::removeElementFromVector(mSpotLightsActive, reinterpret_cast<SpotLight *>(light));
        else
          containerutils::removeElementFromVector(mSpotLightsInactive, reinterpret_cast<SpotLight *>(light));
        break;
      default:
        assert(false);
    }
  }

  void Scene::setFog(WrSceneFogType fogType, WrSceneFogDepthType depthType, const glm::vec4 &color, float density, float start,
                     float end) {
    mIsFogDirty = true;
    mFog.mMode = glm::vec2(fogType, depthType);
    mFog.mColor = colorutils::srgbToLinear(color);
    // To speed up computation of a floating point exponent, we use the fact that e^(x) = 2^(x/log_e(2))
    // The operation is faster under the assumption that exp2() is faster than exp()
    mFog.mParams = glm::vec4(density * INVERSE_LOG2, density * density * INVERSE_LOG2, end, 1.0f / glm::abs(end - start));
  }

  void Scene::setSkybox(Renderable *renderable) {
    mSkybox = renderable;
    if (mSkybox) {
      mSkybox->defaultMaterial()->setEffectiveProgram(Material::MATERIAL_PROGRAM_DEFAULT);
      mSkybox->setEffectiveMaterial(mSkybox->defaultMaterial());
    }
  }

  void Scene::setHdrClearQuad(Renderable *renderable) {
    mHdrClearQuad = renderable;
    if (mHdrClearQuad) {
      mHdrClearQuad->defaultMaterial()->setEffectiveProgram(Material::MATERIAL_PROGRAM_DEFAULT);
      mHdrClearQuad->setEffectiveMaterial(mHdrClearQuad->defaultMaterial());
    }
  }

  int Scene::computeNodeCount() const { return 1 + mRoot->computeChildCount(); }
  void Scene::printSceneTree() { debug::printSceneTree(); }

  void Scene::render(bool culling) {
    assert(glstate::isInitialized());

    ++mFrameCounter;
    DEBUG("\nScene::render: total nodes=" << Scene::computeNodeCount() << ", frame=" << mFrameCounter);

    // debug::printCacheContents();
    // debug::printSceneTree();

    renderToViewports({mMainViewport}, culling);
  }

  void Scene::renderToViewports(std::vector<Viewport *> viewports, bool culling) {
    assert(glstate::isInitialized());

    DEBUG("Notify frame listeners...");

    for (auto listener : mListeners)
      listener();

    DEBUG("\nScene::renderToViewports: viewports.size()=" << viewports.size());

    prepareRender();

    for (Viewport *viewport : viewports) {
      mCurrentViewport = viewport;

      glstate::setDefaultState();
      mCurrentViewport->updateUniforms();
      mCurrentViewport->bind();
      mCurrentViewport->clear();

      // Simplified rendering in case use material is set (for example when picking)
      if (Renderable::useMaterial()) {
        DEBUG("Rendering using material \"" << Renderable::useMaterial() << "\"");
        for (int i = 0; i < WR_RENDERABLE_DRAWING_ORDER_COUNT; ++i) {
          if (!mRenderQueues[i].size())
            continue;

          RenderQueueIterator firstInvisibleRenderable =
            partitionByVisibility(mRenderQueues[i].begin(), mRenderQueues[i].end());
          RenderQueueIterator firstCulledRenderable =
            partitionByViewability(mRenderQueues[i].begin(), firstInvisibleRenderable);

          // Only render Renderables that have the optional material currently in use
          RenderQueueIterator firstWithoutUseMaterial = partitionByUseMaterial(mRenderQueues[i].begin(), firstCulledRenderable);
          if (firstWithoutUseMaterial == mRenderQueues[i].begin())
            continue;

          if (mClearDepth && i > 0)
            glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

          sortRenderQueueByState(mRenderQueues[i].begin(), firstWithoutUseMaterial);
          renderDefault(mRenderQueues[i].begin(), firstWithoutUseMaterial, i > 0);
          mCurrentViewport->applyPostProcessing();
        }
      } else {
        renderToViewport(culling);
        if (mCurrentViewport == mMainViewport && mCurrentViewport->frameBuffer()) {
          glstate::bindDrawFrameBuffer(0);
          mCurrentViewport->frameBuffer()->blit(0, true, false, false, 0, 0, 0, 0, 0, 0,
                                                mCurrentViewport->width() * mCurrentViewport->pixelRatio(),
                                                mCurrentViewport->height() * mCurrentViewport->pixelRatio());
        }
      }

      mCurrentViewport->finishRender();
    }

    glstate::checkError();
  }

  void Scene::enqueueRenderable(Renderable *renderable) {
    mRenderQueues[renderable->drawingOrder()].push_back(renderable);

    if (!renderable->isTranslucent() && renderable->castShadows() && renderable->shadowVolumeCaster() &&
        renderable->drawingOrder() == WR_RENDERABLE_DRAWING_ORDER_MAIN &&
        renderable->drawingMode() == WR_RENDERABLE_DRAWING_MODE_TRIANGLES)
      mShadowVolumeQueue.push_back(renderable->shadowVolumeCaster());
  }

  void Scene::removeFrameListener(void (*listener)()) {
    mListeners.erase(std::remove(mListeners.begin(), mListeners.end(), listener), mListeners.end());
  }

  Scene::Scene() :
    mFrameCounter(0),
    mRoot(NULL),
    mMainViewport(NULL),
    mCurrentViewport(NULL),
    mRenderQueues(WR_RENDERABLE_DRAWING_ORDER_COUNT),
    mSkybox(NULL),
    mHdrClearQuad(NULL),
    mFogProgram(NULL),
    mShadowVolumeProgram(NULL),
    mRenderSkybox(true),
    mHdrClear(true),
    mTranslucence(true),
    mClearDepth(true),
    mPixelBufferCount(0),
    mPixelBufferIds(NULL) {
    mRoot = Transform::createTransform();
    mMainViewport = Viewport::createViewport();
    mMainViewport->setCamera(Camera::createCamera());
    setFog(WR_SCENE_FOG_TYPE_NONE);
  }

  Scene::~Scene() {
    // Cleanup static meshes used for AABBs & bounding spheres draw
    config::cleanup();

    reset();

    if (mMainViewport) {
      Node::deleteNode(mMainViewport->camera());
      Viewport::deleteViewport(mMainViewport);
    }

    if (mRoot)
      Node::deleteNode(mRoot);
  }

  void Scene::prepareRender() {
    // Perform accumulated OpenGL state changes
    GlUser::applyGl();

    // Update the scene tree and enqueue Renderables
    for (RenderQueue &renderQueue : mRenderQueues)
      renderQueue.clear();

    mShadowVolumeQueue.clear();

    mRoot->updateFromParent();

    // cppcheck-suppress reademptycontainer
    DEBUG("Number of shadow-casting Renderables: " << mShadowVolumeQueue.size());

    // Update fog uniform buffer
    updateFogUniformBuffer();
  }

  void Scene::renderToViewport(bool culling) {
    DEBUG("Scene::renderToViewport, viewport = " << mCurrentViewport);

    LightNode::updateUniforms();

    // hdr-clear first, irrespective of winding order
    if (mHdrClear && mHdrClearQuad && mCurrentViewport->isSkyboxEnabled()) {
      DEBUG("performing HDR clear!");

      glstate::setBlend(false);
      glstate::setDepthClamp(false);
      glstate::setDepthMask(false);
      glstate::setDepthTest(false);
      glstate::setStencilTest(false);
      glstate::setColorMask(true, true, true, true);
      glstate::setFrontFace(GL_CCW);

      mHdrClearQuad->render();

      if (mCurrentViewport->camera()->flipY())
        glstate::setFrontFace(GL_CW);
    }

    // Render skybox first
    if (mSkybox && mRenderSkybox && mCurrentViewport->isSkyboxEnabled()) {
      DEBUG("Rendering skybox");

      glstate::setBlend(false);

// GL_DEPTH_CLAMP is not available in webgl, so it is a way to work around
// It must be here and not in glstate::setDepthClamp because we need to access the camera
#ifdef __EMSCRIPTEN__
      float near = mCurrentViewport->camera()->nearDistance();
      mCurrentViewport->camera()->setNear(0.05);
      float far = mCurrentViewport->camera()->farDistance();
      mCurrentViewport->camera()->setFar(1000000.0f);
      mCurrentViewport->camera()->updateUniforms();
#endif
      glstate::setDepthClamp(true);
      glstate::setDepthMask(false);
      glstate::setDepthTest(true);
      glstate::setDepthFunc(GL_LESS);
      glstate::setStencilTest(false);
      glstate::setColorMask(true, true, true, true);

      mSkybox->render();

#ifdef __EMSCRIPTEN__
      mCurrentViewport->camera()->setNear(near);
      mCurrentViewport->camera()->setFar(far);
      mCurrentViewport->camera()->updateUniforms();
#endif
    }

    RenderQueue *renderQueue = &mRenderQueues[WR_RENDERABLE_DRAWING_ORDER_MAIN];
    DEBUG("Rendering queue 0, number of Renderables: " << renderQueue->size());

    RenderQueueIterator firstInvisibleRenderable = partitionByVisibility(renderQueue->begin(), renderQueue->end());
    DEBUG("Number of visible Renderables: " << firstInvisibleRenderable - renderQueue->begin());

    RenderQueueIterator firstCulledRenderable =
      culling ? partitionByViewability(renderQueue->begin(), firstInvisibleRenderable) : renderQueue->end();
    DEBUG("Number of non-culled Renderables: " << firstCulledRenderable - renderQueue->begin());

    RenderQueueIterator firstOpaqueRenderable = renderQueue->begin();
    if (mTranslucence)
      firstOpaqueRenderable = partitionByTranslucency(renderQueue->begin(), firstCulledRenderable);

    DEBUG("Number of opaque Renderables: " << firstCulledRenderable - firstOpaqueRenderable);
    DEBUG("Number of translucent & z-sorted Renderables: " << firstOpaqueRenderable - renderQueue->begin());

    sortRenderQueueByState(firstOpaqueRenderable, firstCulledRenderable);
    sortRenderQueueByDistance(renderQueue->begin(), firstOpaqueRenderable);

    DEBUG("Rendering opaque renderables");
    if (config::areShadowsEnabled() && mCurrentViewport->areShadowsEnabled() && LightNode::activeLightsCastingShadows() > 0) {
      DEBUG("Rendering ambient and emissive illumination");

      RenderQueueIterator firstWithoutStencilProgram = partitionByStencilProgram(firstOpaqueRenderable, firstCulledRenderable);
      RenderQueueIterator firstShadowReceiver = partitionByShadowReceiving(firstOpaqueRenderable, firstWithoutStencilProgram);

      renderStencilAmbientEmissive(firstOpaqueRenderable, firstWithoutStencilProgram);

      mCurrentViewport->applyAmbientOcclusion();

      for (size_t i = 0; i < mDirectionalLightsActive.size(); ++i) {
        mLightRenderable.mActiveLights = glm::ivec4(i, -1, -1, -1);
        glstate::uniformBuffer(WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHT_RENDERABLE)->writeValue(&mLightRenderable);

        renderStencilPerLight(mDirectionalLightsActive[i], firstOpaqueRenderable, firstShadowReceiver,
                              firstWithoutStencilProgram);
      }

      for (size_t i = 0; i < mPointLightsActive.size(); ++i) {
        mLightRenderable.mActiveLights = glm::ivec4(-1, i, -1, -1);
        glstate::uniformBuffer(WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHT_RENDERABLE)->writeValue(&mLightRenderable);

        renderStencilPerLight(mPointLightsActive[i], firstOpaqueRenderable, firstShadowReceiver, firstWithoutStencilProgram);
      }

      for (size_t i = 0; i < mSpotLightsActive.size(); ++i) {
        mLightRenderable.mActiveLights = glm::ivec4(-1, -1, i, -1);
        glstate::uniformBuffer(WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHT_RENDERABLE)->writeValue(&mLightRenderable);

        renderStencilPerLight(mSpotLightsActive[i], firstOpaqueRenderable, firstShadowReceiver, firstWithoutStencilProgram);
      }

      if (mFog.mMode.x > 0.0f) {
        DEBUG("Rendering fog");
        renderStencilFog(firstOpaqueRenderable, firstWithoutStencilProgram);
      }

      // Render renderables which don't have stencil shadow shaders
      renderStencilWithoutProgram(firstWithoutStencilProgram, firstCulledRenderable);
    } else {
      renderDefault(firstOpaqueRenderable, firstCulledRenderable);
      mCurrentViewport->applyAmbientOcclusion();
    }

    if (firstOpaqueRenderable - renderQueue->begin()) {
      DEBUG("Rendering translucent renderables");
      renderTranslucent(renderQueue->begin(), firstOpaqueRenderable);
    }

    // Draw bounding volumes if enabled
    if (config::showBoundingSpheres()) {
      DEBUG("Drawing bounding spheres");
      for (auto it = renderQueue->begin(); it < renderQueue->end(); ++it)
        config::drawBoundingSphere((*it)->boundingSphere());
    }

    if (config::showAabbs()) {
      DEBUG("Drawing AABBs");
      for (auto it = renderQueue->begin(); it < renderQueue->end(); ++it)
        config::drawAabb((*it)->aabb());
    }

    // we want multiple effects to overwrite the viewport's post-processing effect framebuffer so we can't use GL_LESS or from
    // the second effect onwards the effects won't get written to the viewport's framebuffer
    glstate::setDepthFunc(GL_LEQUAL);

    // Render post-processing queue (preserving depth)
    mCurrentViewport->applyPostProcessing();

    // Draw Renderables not part of main queue
    for (int i = WR_RENDERABLE_DRAWING_ORDER_MAIN + 1; i < WR_RENDERABLE_DRAWING_ORDER_COUNT; ++i) {
      if (!mRenderQueues[i].size())
        continue;

      if (mClearDepth)
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      renderQueue = &mRenderQueues[i];
      DEBUG("Rendering queue " << i << ", number of Renderables: " << renderQueue->size());

      firstInvisibleRenderable = partitionByVisibility(renderQueue->begin(), renderQueue->end());
      firstCulledRenderable =
        culling ? partitionByViewability(renderQueue->begin(), firstInvisibleRenderable) : renderQueue->end();
      firstOpaqueRenderable = renderQueue->begin();

      if (mTranslucence)
        firstOpaqueRenderable = partitionByTranslucency(renderQueue->begin(), firstCulledRenderable);

      sortRenderQueueByState(firstOpaqueRenderable, firstCulledRenderable);
      sortRenderQueueByDistance(renderQueue->begin(), firstOpaqueRenderable);

      if (firstCulledRenderable - firstOpaqueRenderable) {
        DEBUG("Rendering opaque renderables");
        renderDefault(firstOpaqueRenderable, firstCulledRenderable, true);
      }

      if (firstOpaqueRenderable - renderQueue->begin()) {
        DEBUG("Rendering translucent renderables");
        renderTranslucent(renderQueue->begin(), firstOpaqueRenderable, true);
      }
    }
    mCurrentViewport->applyAntiAliasing();
    mCurrentViewport->drawOverlays();
  }

  void Scene::updateFogUniformBuffer() {
    if (!mIsFogDirty)
      return;

    glstate::uniformBuffer(WR_GLSL_LAYOUT_UNIFORM_BUFFER_FOG)->writeValue(&mFog);

    mIsFogDirty = false;
  }

  Scene::RenderQueueIterator Scene::partitionByVisibility(RenderQueueIterator first, RenderQueueIterator last) {
    return std::partition(
      first, last, [this](const Renderable *r) -> bool { return mCurrentViewport->visibilityMask() & r->visibilityFlags(); });
  }

  Scene::RenderQueueIterator Scene::partitionByViewability(RenderQueueIterator first, RenderQueueIterator last) {
    return std::partition(first, last, [this](Renderable *r) -> bool {
      return !r->sceneCulling() || mCurrentViewport->camera()->isAabbVisible(r->aabb());
    });
  }

  Scene::RenderQueueIterator Scene::partitionByTranslucency(RenderQueueIterator first, RenderQueueIterator last) {
    return std::partition(first, last, [](const Renderable *r) -> bool { return r->isTranslucent(); });
  }

  Scene::RenderQueueIterator Scene::partitionByUseMaterial(RenderQueueIterator first, RenderQueueIterator last) {
    std::string useMaterial(Renderable::useMaterial());

    return std::partition(first, last,
                          [&useMaterial](const Renderable *r) -> bool { return r->optionalMaterial(useMaterial); });
  }

  Scene::RenderQueueIterator Scene::partitionByStencilProgram(RenderQueueIterator first, RenderQueueIterator last) {
    return std::partition(first, last,
                          [](const Renderable *r) -> bool { return r->effectiveMaterial()->stencilAmbientEmissiveProgram(); });
  }

  Scene::RenderQueueIterator Scene::partitionByShadowReceiving(RenderQueueIterator first, RenderQueueIterator last) {
    return std::partition(first, last, [](const Renderable *r) -> bool { return !r->receiveShadows(); });
  }

  Scene::RenderQueueIterator Scene::partitionByZOrder(RenderQueueIterator first, RenderQueueIterator last) {
    return std::partition(first, last, [](const Renderable *r) -> bool { return r->zSortedRendering(); });
  }

  Scene::ShadowVolumeIterator Scene::partitionShadowsByVisibility(ShadowVolumeIterator first, ShadowVolumeIterator last,
                                                                  LightNode *light) {
    return std::partition(first, last, [this, &light](ShadowVolumeCaster *shadowVolume) -> bool {
      return mCurrentViewport->camera()->isAabbVisible(shadowVolume->aabb(light));
    });
  }

  void Scene::sortRenderQueueByState(RenderQueueIterator first, RenderQueueIterator last) {
    std::sort(first, last, [](const Renderable *a, const Renderable *b) -> bool { return a->sortingId() > b->sortingId(); });
  }

  void Scene::sortRenderQueueByDistance(RenderQueueIterator first, RenderQueueIterator last) {
    for (auto it = first; it < last; ++it)
      (*it)->recomputeBoundingSphereInViewSpace(mCurrentViewport->camera()->view());

    std::sort(first, last, [](const Renderable *a, const Renderable *b) -> bool {
      const float aDistance =
        a->isInViewSpace() ? fabs(a->parent()->position()[2]) : glm::length2(a->boundingSphereInViewSpace().mCenter);
      const float bDistance =
        b->isInViewSpace() ? fabs(b->parent()->position()[2]) : glm::length2(b->boundingSphereInViewSpace().mCenter);
      return aDistance > bDistance;
    });
  }

  void Scene::renderDefault(RenderQueueIterator first, RenderQueueIterator last, bool disableDepthTest) {
    glstate::setBlend(false);
    glstate::setDepthClamp(false);
    glstate::setDepthMask(true);
    glstate::setDepthTest(!disableDepthTest);
    glstate::setDepthFunc(GL_LESS);
    glstate::setStencilTest(false);
    glstate::setCullFace(true);
    glstate::setColorMask(true, true, true, true);

    for (auto it = first; it < last; ++it) {
      assert((*it)->defaultMaterial());

      (*it)->effectiveMaterial()->setEffectiveProgram(Material::MATERIAL_PROGRAM_DEFAULT);
      (*it)->render();
    }
  }

  static bool affectedByLight(Renderable *renderable, LightNode *light) {
    bool visible = true;
    // Light culling
    if (light->type() != LightNode::TYPE_DIRECTIONAL) {
      PositionalLight *positionalLight = static_cast<PositionalLight *>(light);
      const primitive::Sphere &boundingSphere = renderable->boundingSphere();
      const float distance = glm::distance(boundingSphere.mCenter, positionalLight->position());
      const float radius = positionalLight->radius() + boundingSphere.mRadius;
      // Check if light is too far away
      visible = (distance <= radius &&
                 positionalLight->attenuationConstant() +
                     distance * (positionalLight->attenuationLinear() + distance * positionalLight->attenuationQuadratic()) <
                   255.0f);
    }
    return visible;
  }

  void Scene::renderStencilPerLight(LightNode *light, RenderQueueIterator first, RenderQueueIterator firstShadowReceiver,
                                    RenderQueueIterator last) {
    if (light->castShadows()) {
      assert(mShadowVolumeProgram);
      mShadowVolumeProgram->bind();

      Camera *camera = mCurrentViewport->camera();
      const primitive::Plane farPlane = camera->frustum().plane(Frustum::FRUSTUM_PLANE_FAR);

      const primitive::Aabb &cameraAabb = camera->aabb();
      glm::vec3 cameraToLightInv;
      if (light->type() != LightNode::TYPE_DIRECTIONAL) {
        PositionalLight *positionalLight = static_cast<PositionalLight *>(light);
        cameraToLightInv = 1.0f / glm::normalize(positionalLight->position() - camera->position());
      } else {
        DirectionalLight *directionalLight = static_cast<DirectionalLight *>(light);
        cameraToLightInv = 1.0f / -directionalLight->direction();
      }

      ShadowVolumeIterator firstInvisibleShadowVolume =
        partitionShadowsByVisibility(mShadowVolumeQueue.begin(), mShadowVolumeQueue.end(), light);
      for (ShadowVolumeIterator it = mShadowVolumeQueue.begin(); it < firstInvisibleShadowVolume; ++it) {
        const primitive::Aabb renderableAabb = (*it)->renderable()->aabb();

        // Check if the renderable is affected by current light
        if (!affectedByLight((*it)->renderable(), light) || !primitive::isAabbAbovePlane(farPlane, renderableAabb))
          continue;

        // Use depth fail if camera stands in the shadow volume
        if (primitive::aabbCollision(cameraAabb, (*it)->aabb(light)) ||
            primitive::rayIntersectAabb(camera->position(), cameraToLightInv, renderableAabb, false))
          renderStencilShadowVolumesDepthFail(*it, light);
        else
          renderStencilShadowVolumesDepthPass(*it, light);

        if (config::showShadowAabbs()) {
          config::drawAabb((*it)->aabb(light));
          mShadowVolumeProgram->bind();
        }
      }

      if (first != firstShadowReceiver)
        renderStencilDiffuseSpecular(first, firstShadowReceiver, light, false);
      renderStencilDiffuseSpecular(firstShadowReceiver, last, light);

      glClear(GL_STENCIL_BUFFER_BIT);
    } else
      renderStencilDiffuseSpecular(first, last, light, false);
  }

  void Scene::renderStencilShadowVolumesDepthPass(ShadowVolumeCaster *shadowVolume, LightNode *light) {
    glstate::setDepthClamp(true);
    glstate::setDepthMask(false);
    glstate::setDepthTest(true);
    glstate::setDepthFunc(GL_LESS);
    glstate::setStencilTest(true);
    glstate::setStencilFunc(GL_ALWAYS, 0, ~0);
    glstate::setStencilOpFront(GL_KEEP, GL_KEEP, GL_INCR_WRAP);
    glstate::setStencilOpBack(GL_KEEP, GL_KEEP, GL_DECR_WRAP);
    glstate::setCullFace(false);
    glstate::setColorMask(false, false, false, false);

    // Compute silhouette without caps
    shadowVolume->computeSilhouette(light, false);

    glUniformMatrix4fv(mShadowVolumeProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM), 1, false,
                       glm::value_ptr(shadowVolume->renderable()->parent()->matrix()));
    shadowVolume->renderSides(light);
  }

  void Scene::renderStencilShadowVolumesDepthFail(ShadowVolumeCaster *shadowVolume, LightNode *light) {
    glstate::setDepthClamp(true);
    glstate::setDepthMask(false);
    glstate::setDepthTest(true);
    glstate::setDepthFunc(GL_GEQUAL);
    glstate::setStencilTest(true);
    glstate::setStencilFunc(GL_ALWAYS, 0, ~0);
    glstate::setStencilOpFront(GL_KEEP, GL_KEEP, GL_DECR_WRAP);
    glstate::setStencilOpBack(GL_KEEP, GL_KEEP, GL_INCR_WRAP);
    glstate::setCullFace(false);
    glstate::setColorMask(false, false, false, false);

    // Compute silhouette with caps
    shadowVolume->computeSilhouette(light, true);

    glUniformMatrix4fv(mShadowVolumeProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM), 1, false,
                       glm::value_ptr(shadowVolume->renderable()->parent()->matrix()));
    shadowVolume->renderSides(light);

    shadowVolume->renderCaps(light);
  }

  void Scene::renderStencilAmbientEmissive(RenderQueueIterator first, RenderQueueIterator last) {
    glstate::setBlend(false);
    glstate::setDepthClamp(false);
    glstate::setDepthMask(true);
    glstate::setDepthTest(true);
    glstate::setDepthFunc(GL_LESS);
    glstate::setStencilTest(false);
    glstate::setCullFace(true);
    glstate::setColorMask(true, true, true, true);

    for (auto it = first; it < last; ++it) {
      assert((*it)->effectiveMaterial()->stencilAmbientEmissiveProgram());

      (*it)->effectiveMaterial()->setEffectiveProgram(Material::MATERIAL_PROGRAM_STENCIL_AMBIENT_EMISSIVE);
      (*it)->render();
    }
  }

  void Scene::renderStencilDiffuseSpecular(RenderQueueIterator first, RenderQueueIterator last, LightNode *light,
                                           bool applyShadows) {
    glstate::setBlend(true);
    glstate::setBlendEquation(GL_FUNC_ADD);
    glstate::setBlendFunc(GL_ONE, GL_ONE);
    glstate::setDepthClamp(false);
    glstate::setDepthMask(false);
    glstate::setDepthTest(true);
    glstate::setDepthFunc(GL_LEQUAL);
    glstate::setCullFace(true);
    glstate::setColorMask(true, true, true, true);

    if (applyShadows) {
      glstate::setStencilTest(true);
      glstate::setStencilFunc(GL_EQUAL, 0, ~0);
      glstate::setStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    } else
      glstate::setStencilTest(false);

    for (auto it = first; it < last; ++it) {
      if (!affectedByLight(*it, light))
        continue;

      assert((*it)->effectiveMaterial()->stencilDiffuseSpecularProgram());

      (*it)->effectiveMaterial()->setEffectiveProgram(Material::MATERIAL_PROGRAM_STENCIL_DIFFUSE_SPECULAR);
      (*it)->render();
    }
  }

  void Scene::renderStencilFog(RenderQueueIterator first, RenderQueueIterator last) {
    glstate::setBlend(true);
    glstate::setBlendEquation(GL_FUNC_ADD);
    glstate::setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glstate::setDepthClamp(false);
    glstate::setDepthMask(false);
    glstate::setDepthTest(true);
    glstate::setDepthFunc(GL_LEQUAL);
    glstate::setStencilTest(false);
    glstate::setCullFace(true);
    glstate::setColorMask(true, true, true, true);

    for (auto it = first; it < last; ++it)
      (*it)->renderWithoutMaterial(mFogProgram);
  }

  void Scene::renderStencilWithoutProgram(RenderQueueIterator first, RenderQueueIterator last) {
    glstate::setBlend(true);
    glstate::setBlendEquation(GL_FUNC_ADD);
    glstate::setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glstate::setDepthClamp(false);
    glstate::setDepthMask(true);
    glstate::setDepthTest(true);
    glstate::setDepthFunc(GL_LESS);
    glstate::setStencilTest(false);
    glstate::setCullFace(true);
    glstate::setColorMask(true, true, true, true);

    for (auto it = first; it < last; ++it) {
      assert((*it)->effectiveMaterial());

      (*it)->effectiveMaterial()->setEffectiveProgram(Material::MATERIAL_PROGRAM_DEFAULT);
      (*it)->render();
    }
  }

  void Scene::renderTranslucent(RenderQueueIterator first, RenderQueueIterator last, bool disableDepthTest) {
    glstate::setBlend(true);
    glstate::setBlendEquation(GL_FUNC_ADD);
    glstate::setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glstate::setDepthClamp(false);
    glstate::setDepthMask(false);
    glstate::setDepthTest(!disableDepthTest);
    glstate::setDepthFunc(GL_LESS);
    glstate::setStencilTest(false);
    glstate::setCullFace(true);
    glstate::setColorMask(true, true, true, true);

    for (auto it = first; it < last; ++it) {
      if (!(*it)->effectiveMaterial())
        continue;

      (*it)->effectiveMaterial()->setEffectiveProgram(Material::MATERIAL_PROGRAM_DEFAULT);
      (*it)->render();
    }
  }

}  // namespace wren

// C interface implementation
WrScene *wr_scene_get_instance() {
  return reinterpret_cast<WrScene *>(wren::Scene::instance());
}

void wr_scene_destroy() {
  wren::Scene::destroy();
}

void wr_scene_init(WrScene *scene) {
  reinterpret_cast<wren::Scene *>(scene)->init();
}

void wr_scene_apply_pending_updates(WrScene *scene) {
  reinterpret_cast<wren::Scene *>(scene)->applyPendingUpdates();
}

void wr_scene_get_main_buffer(WrScene *scene, int width, int height, unsigned int format, unsigned int data_type,
                              unsigned int buffer_type, void *buffer) {
  reinterpret_cast<wren::Scene *>(scene)->getMainBuffer(width, height, format, data_type, buffer_type, buffer);
}

void wr_scene_init_frame_capture(WrScene *scene, int pixel_buffer_count, unsigned int *pixel_buffer_ids, int frame_size) {
  reinterpret_cast<wren::Scene *>(scene)->initFrameCapture(pixel_buffer_count, pixel_buffer_ids, frame_size);
}

void wr_scene_bind_pixel_buffer(WrScene *scene, int buffer) {
  reinterpret_cast<wren::Scene *>(scene)->bindPixelBuffer(buffer);
}

void *wr_scene_map_pixel_buffer(WrScene *scene, unsigned int access_mode) {
  return reinterpret_cast<wren::Scene *>(scene)->mapPixelBuffer(access_mode);
}

void wr_scene_unmap_pixel_buffer(WrScene *scene) {
  reinterpret_cast<wren::Scene *>(scene)->unMapPixelBuffer();
}

void wr_scene_terminate_frame_capture(WrScene *scene) {
  reinterpret_cast<wren::Scene *>(scene)->terminateFrameCapture();
}

void wr_scene_render(WrScene *scene, const char *material_name, bool culling) {
  if (material_name)
    wren::Renderable::setUseMaterial(material_name);

  reinterpret_cast<wren::Scene *>(scene)->render(culling);

  wren::Renderable::setUseMaterial(NULL);
}

void wr_scene_render_to_viewports(WrScene *scene, int count, WrViewport **viewports, const char *material_name, bool culling) {
  if (material_name)
    wren::Renderable::setUseMaterial(material_name);

  wren::Viewport **start = reinterpret_cast<wren::Viewport **>(viewports);
  std::vector<wren::Viewport *> viewportsVector(start, start + count);
  reinterpret_cast<wren::Scene *>(scene)->renderToViewports(viewportsVector, culling);

  wren::Renderable::setUseMaterial(NULL);
}

void wr_scene_reset(WrScene *scene) {
  reinterpret_cast<wren::Scene *>(scene)->reset();
}

void wr_scene_set_ambient_light(const float *ambient_light) {
  wren::LightNode::setAmbientLight(glm::make_vec3(ambient_light));
}

int wr_scene_get_active_spot_light_count(WrScene *scene) {
  return reinterpret_cast<wren::Scene *>(scene)->spotLights().size();
}

int wr_scene_get_active_point_light_count(WrScene *scene) {
  return reinterpret_cast<wren::Scene *>(scene)->pointLights().size();
}

int wr_scene_get_active_directional_light_count(WrScene *scene) {
  return reinterpret_cast<wren::Scene *>(scene)->directionalLights().size();
}

void wr_scene_set_fog(WrScene *scene, WrSceneFogType fogType, WrSceneFogDepthType depthType, const float *color, float density,
                      float start, float end) {
  reinterpret_cast<wren::Scene *>(scene)->setFog(
    fogType, depthType, color != NULL ? glm::vec4(color[0], color[1], color[2], 1.0f) : wren::gVec4Ones, density, start, end);
}

void wr_scene_set_skybox(WrScene *scene, WrRenderable *renderable) {
  reinterpret_cast<wren::Scene *>(scene)->setSkybox(reinterpret_cast<wren::Renderable *>(renderable));
}

void wr_scene_set_hdr_clear_quad(WrScene *scene, WrRenderable *renderable) {
  reinterpret_cast<wren::Scene *>(scene)->setHdrClearQuad(reinterpret_cast<wren::Renderable *>(renderable));
}

void wr_scene_set_fog_program(WrScene *scene, WrShaderProgram *program) {
  reinterpret_cast<wren::Scene *>(scene)->setFogProgram(reinterpret_cast<wren::ShaderProgram *>(program));
}

void wr_scene_set_shadow_volume_program(WrScene *scene, WrShaderProgram *program) {
  reinterpret_cast<wren::Scene *>(scene)->setShadowVolumeProgram(reinterpret_cast<wren::ShaderProgram *>(program));
}

void wr_scene_enable_depth_reset(WrScene *scene, bool enable) {
  reinterpret_cast<wren::Scene *>(scene)->enableDepthReset(enable);
}

void wr_scene_enable_skybox(WrScene *scene, bool enable) {
  reinterpret_cast<wren::Scene *>(scene)->enableSkybox(enable);
}

void wr_scene_enable_hdr_clear(WrScene *scene, bool enable) {
  reinterpret_cast<wren::Scene *>(scene)->enableHdrClear(enable);
}

void wr_scene_enable_translucence(WrScene *scene, bool enable) {
  reinterpret_cast<wren::Scene *>(scene)->enableTranslucence(enable);
}

int wr_scene_compute_node_count(WrScene *scene) {
  return reinterpret_cast<wren::Scene *>(scene)->computeNodeCount();
}

WrTransform *wr_scene_get_root(WrScene *scene) {
  return reinterpret_cast<WrTransform *>(reinterpret_cast<wren::Scene *>(scene)->root());
}

WrViewport *wr_scene_get_viewport(WrScene *scene) {
  return reinterpret_cast<WrViewport *>(reinterpret_cast<wren::Scene *>(scene)->mainViewport());
}

void wr_scene_add_frame_listener(WrScene *scene, void (*listener)()) {
  reinterpret_cast<wren::Scene *>(scene)->addFrameListener(listener);
}

void wr_scene_remove_frame_listener(WrScene *scene, void (*listener)()) {
  reinterpret_cast<wren::Scene *>(scene)->removeFrameListener(listener);
}
