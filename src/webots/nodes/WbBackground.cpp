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

#include "WbBackground.hpp"

#include "WbApplication.hpp"
#include "WbApplicationInfo.hpp"
#include "WbDownloader.hpp"
#include "WbField.hpp"
#include "WbFieldChecker.hpp"
#include "WbGroup.hpp"
#include "WbMFColor.hpp"
#include "WbMFString.hpp"
#include "WbMathsUtilities.hpp"
#include "WbNodeOperations.hpp"
#include "WbPreferences.hpp"
#include "WbSFNode.hpp"
#include "WbStandardPaths.hpp"
#include "WbUrl.hpp"
#include "WbViewpoint.hpp"
#include "WbWorld.hpp"
#include "WbWrenOpenGlContext.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include <wren/gl_state.h>
#include <wren/material.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/scene.h>
#include <wren/shader_program.h>
#include <wren/static_mesh.h>
#include <wren/texture_cubemap.h>
#include <wren/texture_cubemap_baker.h>
#include <wren/transform.h>
#include <wren/viewport.h>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtGui/QImage>
#include <QtGui/QImageReader>

#define STB_IMAGE_IMPLEMENTATION  // needed for include to work properly
#include <stb_image.h>

QList<WbBackground *> WbBackground::cBackgroundList;

static const QString gDirections[6] = {"right", "left", "top", "bottom", "front", "back"};

static const QString gUrlNames(int i) {
  return gDirections[i] + "Url";
}

static const QString gIrradianceUrlNames(int i) {
  return gDirections[i] + "IrradianceUrl";
}

static int gCoordinateSystemSwap(int i) {
  static const int enu_swap[] = {5, 4, 0, 1, 3, 2};
  if (WbWorld::instance()->worldInfo()->coordinateSystem() == "ENU")
    return enu_swap[i];
  else  // "NUE" or "EUN"
    return i;
}

static int gCoordinateSystemRotate(int i) {
  static const int enu_rotate[] = {90, -90, 0, 180, -90, -90};
  if (WbWorld::instance()->worldInfo()->coordinateSystem() == "ENU")
    return enu_rotate[i];
  else  // "NUE" or "EUN"
    return 0;
}

void WbBackground::init() {
  mSkyColor = findMFColor("skyColor");
  mLuminosity = findSFDouble("luminosity");
  for (int i = 0; i < 6; ++i) {
    mUrlFields[i] = findMFString(gUrlNames(i));
    mIrradianceUrlFields[i] = findMFString(gIrradianceUrlNames(i));
    mTexture[i] = NULL;
    mIrradianceTexture[i] = NULL;
  }
  for (int i = 0; i < 12; ++i)
    mDownloader[i] = NULL;
  mSkyboxShaderProgram = NULL;
  mSkyboxRenderable = NULL;
  mSkyboxMaterial = NULL;
  mSkyboxTransform = NULL;
  mSkyboxMesh = NULL;

  mHdrClearShaderProgram = NULL;
  mHdrClearRenderable = NULL;
  mHdrClearMaterial = NULL;
  mHdrClearTransform = NULL;
  mHdrClearMesh = NULL;

  mCubeMapTexture = NULL;
  mIrradianceCubeTexture = NULL;

  mTextureHasAlpha = false;
  mTextureSize = 0;
  mIrradianceWidth = 0;
  mIrradianceHeight = 0;
}

WbBackground::WbBackground(WbTokenizer *tokenizer) : WbBaseNode("Background", tokenizer) {
  init();
  if (tokenizer == NULL)
    mSkyColor->setItem(0, WbRgb(0.15, 0.45, 1), false);
}

WbBackground::WbBackground(const WbBackground &other) : WbBaseNode(other) {
  init();
}

WbBackground::WbBackground(const WbNode &other) : WbBaseNode(other) {
  init();
}

WbBackground::~WbBackground() {
  const bool firstInstanceDeleted = isFirstInstance();

  cBackgroundList.removeAll(this);

  destroySkyBox();

  if (firstInstanceDeleted) {
    WbBackground *newFirstInstance = firstInstance();
    if (newFirstInstance == NULL)
      // reset to default
      applyColorToWren(WbRgb());
    else
      // activate next Background node
      newFirstInstance->activate();
  }

  if (!WbWorld::instance()->isCleaning())
    emit WbWrenRenderingContext::instance()->backgroundColorChanged();

  wr_scene_set_hdr_clear_quad(wr_scene_get_instance(), NULL);
  // Delete skybox
  // Shader program is not deleted, a singleton instance is kept in WbWrenShaders
  wr_node_delete(WR_NODE(mSkyboxRenderable));

  if (mSkyboxMaterial)
    wr_material_delete(mSkyboxMaterial);

  wr_node_delete(WR_NODE(mSkyboxTransform));
  wr_static_mesh_delete(mSkyboxMesh);

  // Delete skybox
  // Shader program is not deleted, a singleton instance is kept in WbWrenShaders
  wr_node_delete(WR_NODE(mHdrClearRenderable));
  mHdrClearRenderable = NULL;
  wr_scene_set_hdr_clear_quad(wr_scene_get_instance(), mHdrClearRenderable);

  if (mHdrClearMaterial)
    wr_material_delete(mHdrClearMaterial);

  wr_node_delete(WR_NODE(mHdrClearTransform));
  wr_static_mesh_delete(mHdrClearMesh);

  for (int i = 0; i < 6; i++) {
    delete mTexture[i];
    if (mIrradianceTexture[i])
      stbi_image_free(mIrradianceTexture[i]);
  }
}

void WbBackground::downloadAsset(const QString &url, int index, bool postpone) {
  if (!WbUrl::isWeb(url))
    return;
  if (index < 6) {
    delete mTexture[index];
    mTexture[index] = NULL;
  } else {
    stbi_image_free(mIrradianceTexture[index - 6]);
    mIrradianceTexture[index - 6] = NULL;
  }
  delete mDownloader[index];
  mDownloader[index] = new WbDownloader(this);
  mDownloader[index]->setIsBackground(true);
  if (postpone)
    connect(mDownloader[index], &WbDownloader::complete, this, &WbBackground::downloadUpdate);

  mDownloader[index]->download(QUrl(url));
}

void WbBackground::downloadAssets() {
  for (size_t i = 0; i < 6; i++) {
    if (mUrlFields[i]->size())
      downloadAsset(mUrlFields[i]->item(0), i, false);
    if (mIrradianceUrlFields[i]->size())
      downloadAsset(mIrradianceUrlFields[i]->item(0), i + 6, false);
  }
}

void WbBackground::downloadUpdate() {
  // we need that all downloads are complete before proceeding with the update of the cube map
  for (int i = 0; i < 12; i++)
    if (mDownloader[i] && !mDownloader[i]->hasFinished())
      return;
  updateCubemap();
  WbWorld::instance()->viewpoint()->emit refreshRequired();
}

void WbBackground::preFinalize() {
  WbBaseNode::preFinalize();
  cBackgroundList << this;
}

void WbBackground::postFinalize() {
  WbBaseNode::postFinalize();

  if (isFirstInstance())
    activate();
  else
    parsingWarn(tr("Only one Background node is allowed. The current node won't be taken into account."));
}

void WbBackground::activate() {
  if (!areWrenObjectsInitialized())
    createWrenObjects();

  connect(mLuminosity, &WbSFDouble::changed, this, &WbBackground::updateLuminosity);
  connect(mSkyColor, &WbMFColor::changed, this, &WbBackground::updateColor);
  for (int i = 0; i < 6; ++i) {
    connect(mUrlFields[i], &WbMFString::changed, this, &WbBackground::updateCubemap);
    connect(mIrradianceUrlFields[i], &WbMFString::changed, this, &WbBackground::updateCubemap);
  }

  updateColor();

  updateCubemap();
}

void WbBackground::createWrenObjects() {
  WbBaseNode::createWrenObjects();

  mSkyboxShaderProgram = WbWrenShaders::skyboxShader();
  mSkyboxMaterial = wr_phong_material_new();
  mSkyboxRenderable = wr_renderable_new();
  mSkyboxMesh = wr_static_mesh_unit_box_new(false);

  wr_material_set_default_program(mSkyboxMaterial, mSkyboxShaderProgram);
  wr_renderable_set_cast_shadows(mSkyboxRenderable, false);
  wr_renderable_set_receive_shadows(mSkyboxRenderable, false);
  wr_renderable_set_mesh(mSkyboxRenderable, WR_MESH(mSkyboxMesh));
  wr_renderable_set_material(mSkyboxRenderable, mSkyboxMaterial, NULL);
  wr_renderable_set_drawing_mode(mSkyboxRenderable, WR_RENDERABLE_DRAWING_MODE_TRIANGLES);
  wr_renderable_set_face_culling(mSkyboxRenderable, false);

  mSkyboxTransform = wr_transform_new();
  wr_transform_attach_child(mSkyboxTransform, WR_NODE(mSkyboxRenderable));

  mHdrClearShaderProgram = WbWrenShaders::hdrClearShader();
  mHdrClearMaterial = wr_phong_material_new();
  mHdrClearRenderable = wr_renderable_new();
  mHdrClearMesh = wr_static_mesh_quad_new();

  wr_material_set_default_program(mHdrClearMaterial, mHdrClearShaderProgram);
  wr_renderable_set_cast_shadows(mHdrClearRenderable, false);
  wr_renderable_set_receive_shadows(mHdrClearRenderable, false);
  wr_renderable_set_mesh(mHdrClearRenderable, WR_MESH(mHdrClearMesh));
  wr_renderable_set_material(mHdrClearRenderable, mHdrClearMaterial, NULL);
  wr_renderable_set_drawing_mode(mHdrClearRenderable, WR_RENDERABLE_DRAWING_MODE_TRIANGLES);

  mHdrClearTransform = wr_transform_new();
  wr_transform_attach_child(mHdrClearTransform, WR_NODE(mHdrClearRenderable));

  if (isFirstInstance())
    applyColorToWren(skyColor());
}

void WbBackground::destroySkyBox() {
  wr_scene_set_skybox(wr_scene_get_instance(), NULL);

  if (mSkyboxMaterial)
    wr_material_set_texture_cubemap(mSkyboxMaterial, NULL, 0);

  if (mCubeMapTexture) {
    wr_texture_delete(WR_TEXTURE(mCubeMapTexture));
    mCubeMapTexture = NULL;
  }

  if (mIrradianceCubeTexture) {
    wr_texture_delete(WR_TEXTURE(mIrradianceCubeTexture));
    mIrradianceCubeTexture = NULL;
  }
}

void WbBackground::updateColor() {
  if (WbFieldChecker::resetMultipleColorIfInvalid(this, mSkyColor))
    return;

  if (areWrenObjectsInitialized())
    applyColorToWren(skyColor());

  emit WbWrenRenderingContext::instance()->backgroundColorChanged();
}

void WbBackground::updateCubemap() {
  if (areWrenObjectsInitialized()) {
    // if some textures are to be downloaded again (changed from the scene tree or supervisor)
    // we should postpone the applySkyBoxToWren
    bool postpone = false;
    int urlCount = 0;
    int irradianceUrlCount = 0;
    for (int i = 0; i < 6; i++) {
      if (mUrlFields[i]->size())
        urlCount++;
      if (mIrradianceUrlFields[i]->size())
        irradianceUrlCount++;
    }
    const bool hasCompleteBackground = urlCount == 6;
    if (isPostFinalizedCalled()) {
      for (int i = 0; i < 6; i++) {
        if (hasCompleteBackground) {
          const QString &url = mUrlFields[i]->item(0);
          if (WbUrl::isWeb(url)) {
            if (mDownloader[i] == NULL) {
              downloadAsset(url, i, true);
              postpone = true;
            }
          } else {
            delete mTexture[i];
            mTexture[i] = 0;
          }
        }
        if (mIrradianceUrlFields[i]->size() > 0) {
          const QString &irradianceUrl = mIrradianceUrlFields[i]->item(0);
          if (WbUrl::isWeb(irradianceUrl)) {
            if (mDownloader[i + 6] == NULL) {
              downloadAsset(irradianceUrl, i + 6, true);
              postpone = true;
            }
          } else {
            stbi_image_free(mIrradianceTexture[i]);
            mIrradianceTexture[i] = NULL;
          }
        }
      }
    }
    if (!postpone) {
      bool destroy = false;
      if (irradianceUrlCount > 0 && irradianceUrlCount < 6) {
        warn(tr("Incomplete irradiance cubemap"));
        destroy = true;
      }
      if (!hasCompleteBackground) {
        if (urlCount > 0) {
          warn(tr("Incomplete background cubemap"));
          destroy = true;
        }
      } else
        for (int i = 0; i < 6; i++)
          if (!loadTexture(i)) {
            destroy = true;
            break;
          }
      for (int i = 0; i < 6; i++)
        if (!loadIrradianceTexture(i)) {
          destroy = true;
          break;
        }
      if (destroy) {
        destroySkyBox();
        applyColorToWren(skyColor());
        emit WbWrenRenderingContext::instance()->backgroundColorChanged();
      } else if (hasCompleteBackground || urlCount == 0)
        applySkyBoxToWren();
    }
  }
}

void WbBackground::updateLuminosity() {
  if (WbFieldChecker::resetDoubleIfNegative(this, mLuminosity, 1.0))
    return;

  emit luminosityChanged();
}

void WbBackground::applyColorToWren(const WbRgb &color) {
  const float value[] = {static_cast<float>(color.red()), static_cast<float>(color.green()), static_cast<float>(color.blue())};
  wr_viewport_set_clear_color_rgb(wr_scene_get_viewport(wr_scene_get_instance()), value);
  if (areWrenObjectsInitialized()) {
    // use wren's set_diffuse to transform to linear color space
    wr_phong_material_set_diffuse(mHdrClearMaterial, value);

    // de-gamma correct
    float hdrColor[] = {powf(value[0], 2.2), powf(value[1], 2.2), powf(value[2], 2.2)};

    // reverse tone map
    const float exposure = WbWorld::instance()->viewpoint()->exposure()->value();
    for (int i = 0; i < 3; ++i)
      hdrColor[i] = -log(1.000000001 - hdrColor[i]) / exposure;

    wr_phong_material_set_linear_diffuse(mHdrClearMaterial, hdrColor);
    wr_scene_set_hdr_clear_quad(wr_scene_get_instance(), mHdrClearRenderable);
  }
}

bool WbBackground::loadTexture(int i) {
  if (mTexture[i])
    return true;
  const int urlFieldIndex = gCoordinateSystemSwap(i);
  QString url;
  QIODevice *device;

  if (mDownloader[urlFieldIndex]) {
    url = mUrlFields[urlFieldIndex]->item(0);
    if (!mDownloader[urlFieldIndex]->error().isEmpty()) {
      warn(tr("Cannot retrieve '%1': %2").arg(url).arg(mDownloader[urlFieldIndex]->error()));
      delete mDownloader[urlFieldIndex];
      mDownloader[urlFieldIndex] = NULL;
      return false;
    }
    assert(mDownloader[urlFieldIndex]->device());
    device = mDownloader[urlFieldIndex]->device();
  } else {
    if (mUrlFields[urlFieldIndex]->size() == 0)
      return false;
    url = WbUrl::computePath(this, QString("%1Url").arg(gDirections[i]), mUrlFields[urlFieldIndex]->item(0), false);
    if (url == WbUrl::missingTexture() || url.isEmpty()) {
      warn(tr("Texture not found: '%1'").arg(mUrlFields[urlFieldIndex]->item(0)));
      return false;
    }
    device = new QFile(url);
    if (!device->open(QIODevice::ReadOnly)) {
      warn(tr("Cannot open texture file: '%1'").arg(url));
      delete device;
      return false;
    }
  }
  QImageReader imageReader(device);
  QSize textureSize = imageReader.size();

  if (textureSize.width() != textureSize.height()) {
    warn(tr("The %1Url '%2' is not a square image (its width doesn't equal its height).").arg(gDirections[i], url));
    if (!mDownloader[urlFieldIndex])
      delete device;
    return false;
  }
  for (int j = 0; j < 6; j++)
    if (mTexture[j]) {
      if (textureSize.width() == mTextureSize)
        break;
      else {
        warn(tr("Texture dimension mismatch between %1Url and %2Url.").arg(gDirections[i], gDirections[j]));
        if (!mDownloader[urlFieldIndex])
          delete device;
        return false;
      }
    }
  mTextureSize = textureSize.width();
  mTexture[i] = new QImage;
  if (!imageReader.read(mTexture[i])) {
    warn(tr("Cannot load texture '%1': %2.").arg(imageReader.fileName()).arg(imageReader.errorString()));
    if (!mDownloader[urlFieldIndex])
      delete device;
    return false;
  }

  for (int j = 0; j < 6; j++) {
    if (mTexture[j] && j != i) {
      if (mTexture[i]->hasAlphaChannel() == mTextureHasAlpha)
        break;
      warn(tr("Alpha channel mismatch with %1Url.").arg(gDirections[i]));
      delete mTexture[i];
      mTexture[i] = NULL;
      if (!mDownloader[urlFieldIndex])
        delete device;
      return false;
    }
  }
  mTextureHasAlpha = mTexture[i]->hasAlphaChannel();
  if (mTexture[i]->format() != QImage::Format_ARGB32) {
    QImage tmp = mTexture[i]->convertToFormat(QImage::Format_ARGB32);
    mTexture[i]->swap(tmp);
  }
  const int rotate = gCoordinateSystemRotate(i);
  // FIXME: this texture rotation should be performed by OpenGL or in the shader to get a better performance
  if (rotate != 0) {
    QPoint center = mTexture[i]->rect().center();
    QTransform matrix;
    matrix.translate(center.x(), center.y());
    matrix.rotate(rotate);
    QImage tmp = mTexture[i]->transformed(matrix);
    mTexture[i]->swap(tmp);
  }
  if (mDownloader[urlFieldIndex]) {
    delete mDownloader[urlFieldIndex];
    mDownloader[urlFieldIndex] = NULL;
  } else {
    device->close();
    delete device;
  }
  return true;
}

bool WbBackground::loadIrradianceTexture(int i) {
  if (mIrradianceTexture[i])
    return true;
  const int j = gCoordinateSystemSwap(i);
  if (mIrradianceUrlFields[j]->size() == 0)
    return true;
  const int k = j + 6;
  QIODevice *device = mDownloader[k] ? mDownloader[k]->device() : NULL;
  bool shouldDelete = false;
  int components;
  if (device) {
    if (!mDownloader[k]->error().isEmpty()) {
      warn(tr("Cannot download %1IrradianceUrl: %2").arg(gDirections[1], mDownloader[k]->error()));
      delete mDownloader[k];
      mDownloader[k] = NULL;
      return false;
    }
  } else {
    const QString url =
      WbUrl::computePath(this, QString("%1IrradianceUrl").arg(gDirections[i]), mIrradianceUrlFields[j]->item(0), false);
    if (url.isEmpty()) {
      warn(tr("%1IrradianceUrl not found: '%2'").arg(gDirections[i], mUrlFields[i]->item(0)));
      return false;
    }
    device = new QFile(url);
    shouldDelete = true;
    if (!device->open(QIODevice::ReadOnly)) {
      warn(tr("Cannot open HDR texture file: '%1'").arg(mIrradianceUrlFields[j]->item(0)));
      delete device;
      return false;
    }
  }
  const QByteArray content = device->readAll();
  if (shouldDelete) {
    device->close();
    delete device;
  } else {
    delete mDownloader[k];
    mDownloader[k] = NULL;
  }
  float *data = stbi_loadf_from_memory((const unsigned char *)content.constData(), content.size(), &mIrradianceWidth,
                                       &mIrradianceHeight, &components, 0);
  const int rotate = gCoordinateSystemRotate(i);
  // FIXME: this texture rotation should be performed by OpenGL or in the shader to get a better performance
  if (rotate != 0) {
    float *rotated = (float *)stbi__malloc(sizeof(float) * mIrradianceWidth * mIrradianceHeight * components);
    if (rotate == 90) {
      for (int x = 0; x < mIrradianceWidth; x++) {
        for (int y = 0; y < mIrradianceHeight; y++) {
          const int u = y * mIrradianceWidth * components + x * components;
          const int v = (mIrradianceWidth - 1 - x) * mIrradianceWidth * components + y * components;
          for (int c = 0; c < components; c++)
            rotated[u + c] = data[v + c];
        }
      }
      const int swap = mIrradianceWidth;
      mIrradianceWidth = mIrradianceHeight;
      mIrradianceHeight = swap;
    } else if (rotate == -90) {
      for (int x = 0; x < mIrradianceWidth; x++) {
        for (int y = 0; y < mIrradianceHeight; y++) {
          const int u = y * mIrradianceWidth * components + x * components;
          const int v = x * mIrradianceWidth * components + (mIrradianceHeight - 1 - y) * components;
          for (int c = 0; c < components; c++)
            rotated[u + c] = data[v + c];
        }
      }
      const int swap = mIrradianceWidth;
      mIrradianceWidth = mIrradianceHeight;
      mIrradianceHeight = swap;
    } else if (rotate == 180) {
      for (int x = 0; x < mIrradianceWidth; x++) {
        for (int y = 0; y < mIrradianceHeight; y++) {
          const int u = y * mIrradianceWidth * components + x * components;
          const int v = (mIrradianceHeight - 1 - y) * mIrradianceWidth * components + (mIrradianceWidth - 1 - x) * components;
          for (int c = 0; c < components; c++)
            rotated[u + c] = data[v + c];
        }
      }
    }
    stbi_image_free(data);
    data = rotated;
  }
  mIrradianceTexture[i] = data;
  return true;
}

void WbBackground::applySkyBoxToWren() {
  destroySkyBox();

  WbWrenOpenGlContext::makeWrenCurrent();

  // 1. Load the background if present
  if (mTexture[0]) {
    mCubeMapTexture = wr_texture_cubemap_new();
    wr_texture_set_internal_format(WR_TEXTURE(mCubeMapTexture), WR_TEXTURE_INTERNAL_FORMAT_RGBA8);

    for (int i = 0; i < 6; i++)
      wr_texture_cubemap_set_data(mCubeMapTexture, reinterpret_cast<const char *>(mTexture[i]->bits()),
                                  static_cast<WrTextureOrientation>(i));

    wr_texture_set_size(WR_TEXTURE(mCubeMapTexture), mTexture[0]->width(), mTexture[0]->height());
    wr_texture_setup(WR_TEXTURE(mCubeMapTexture));
    wr_material_set_texture_cubemap(mSkyboxMaterial, mCubeMapTexture, 0);
    wr_material_set_texture_cubemap_wrap_r(mSkyboxMaterial, WR_TEXTURE_WRAP_MODE_CLAMP_TO_EDGE, 0);
    wr_material_set_texture_cubemap_wrap_s(mSkyboxMaterial, WR_TEXTURE_WRAP_MODE_CLAMP_TO_EDGE, 0);
    wr_material_set_texture_cubemap_wrap_t(mSkyboxMaterial, WR_TEXTURE_WRAP_MODE_CLAMP_TO_EDGE, 0);
    wr_scene_set_skybox(wr_scene_get_instance(), mSkyboxRenderable);
  }

  // 2. Load the irradiance map
  WrTextureCubeMap *cm;
  bool missing = false;
  for (int i = 0; i < 6; i++)
    if (mIrradianceTexture[i] == NULL) {
      missing = true;
      break;
    }
  if (missing) {  // If missing, bake a small irradiance map to have the right colors (reflections won't be good in that case)
    int size;
    if (mCubeMapTexture) {  // if a cubemap is available, use it
      cm = mCubeMapTexture;
      size = 64;
    } else {  // otherwise, use a small uniform texture with the color of the sky
      cm = wr_texture_cubemap_new();
      size = 2;
      const int size2 = size * size;
      wr_texture_set_internal_format(WR_TEXTURE(cm), WR_TEXTURE_INTERNAL_FORMAT_RGBA8);
      unsigned int data[size2];
      const WbRgb &c = skyColor();
      unsigned int color = c.redByte() * 0x10000 + c.greenByte() * 0x100 + c.blueByte();
      for (int i = 0; i < size2; i++)
        data[i] = color;
      for (int i = 0; i < 6; i++)
        wr_texture_cubemap_set_data(cm, reinterpret_cast<const char *>(data), static_cast<WrTextureOrientation>(i));
      wr_texture_set_size(WR_TEXTURE(cm), size, size);
      wr_texture_setup(WR_TEXTURE(cm));
    }
    mIrradianceCubeTexture =
      wr_texture_cubemap_bake_specular_irradiance(cm, WbWrenShaders::iblSpecularIrradianceBakingShader(), size);
    if (!mCubeMapTexture)
      wr_texture_delete(WR_TEXTURE(cm));
  } else {
    cm = wr_texture_cubemap_new();
    wr_texture_set_internal_format(WR_TEXTURE(cm), WR_TEXTURE_INTERNAL_FORMAT_RGB32F);
    for (int i = 0; i < 6; i++)
      wr_texture_cubemap_set_data(cm, reinterpret_cast<const char *>(mIrradianceTexture[i]),
                                  static_cast<WrTextureOrientation>(i));
    wr_texture_set_size(WR_TEXTURE(cm), mIrradianceWidth, mIrradianceHeight);
    wr_texture_set_texture_unit(WR_TEXTURE(cm), 13);
    wr_texture_setup(WR_TEXTURE(cm));
    mIrradianceCubeTexture =
      wr_texture_cubemap_bake_specular_irradiance(cm, WbWrenShaders::iblSpecularIrradianceBakingShader(), mIrradianceWidth);
    wr_texture_delete(WR_TEXTURE(cm));
  }
  wr_texture_cubemap_disable_automatic_mip_map_generation(mIrradianceCubeTexture);

  WbWrenOpenGlContext::doneWren();

  emit cubemapChanged();
}

WbRgb WbBackground::skyColor() const {
  return (mSkyColor->size() > 0 ? mSkyColor->item(0) : WbRgb());
}

void WbBackground::exportNodeFields(WbVrmlWriter &writer) const {
  if (writer.isWebots()) {
    WbBaseNode::exportNodeFields(writer);
    return;
  }

  findField("skyColor", true)->write(writer);
  findField("luminosity", true)->write(writer);

  QString backgroundFileNames[6];
  for (int i = 0; i < 6; ++i) {
    if (mUrlFields[i]->size() == 0)
      continue;
    QString imagePath = mUrlFields[i]->value()[0];
    if (imagePath.indexOf("http") == 0)
      backgroundFileNames[i] = imagePath;
    else if (imagePath.indexOf("webots://") == 0)
      backgroundFileNames[i] = imagePath.replace("webots://", "https://raw.githubusercontent.com/" + WbApplicationInfo::repo() +
                                                                "/" + WbApplicationInfo::branch() + "/");
    else {
      const QString &url = WbUrl::computePath(this, "textureBaseName", mUrlFields[i]->item(0), false);
      const QFileInfo &cubeInfo(url);
      if (writer.isWritingToFile())
        backgroundFileNames[i] =
          WbUrl::exportTexture(this, url, url, writer.relativeTexturesPath() + cubeInfo.dir().dirName() + "/", writer);
      else
        backgroundFileNames[i] = writer.relativeTexturesPath() + cubeInfo.dir().dirName() + "/" + cubeInfo.fileName();
      writer.addTextureToList(backgroundFileNames[i], url);
    }
  }

  QString irradianceFileNames[6];
  for (int i = 0; i < 6; ++i) {
    if (mIrradianceUrlFields[i]->size() == 0)
      continue;

    QString irradiancePath = mIrradianceUrlFields[i]->value()[0];
    if (irradiancePath.indexOf("http") == 0)
      irradianceFileNames[i] = mIrradianceUrlFields[i]->value()[0];
    else if (irradiancePath.indexOf("webots://") == 0)
      irradianceFileNames[i] =
        irradiancePath.replace("webots://", "https://raw.githubusercontent.com/" + WbApplicationInfo::repo() + "/" +
                                              WbApplicationInfo::branch() + "/");
    else {
      const QString &url = WbUrl::computePath(this, "textureBaseName", mIrradianceUrlFields[i]->item(0), false);
      const QFileInfo &cubeInfo(url);
      if (writer.isWritingToFile())
        irradianceFileNames[i] =
          WbUrl::exportTexture(this, url, url, writer.relativeTexturesPath() + cubeInfo.dir().dirName() + "/", writer);
      else
        irradianceFileNames[i] = writer.relativeTexturesPath() + cubeInfo.dir().dirName() + "/" + cubeInfo.fileName();
      writer.addTextureToList(irradianceFileNames[i], url);
    }
  }

  if (writer.isX3d()) {
    writer << " ";
    for (int i = 0; i < 6; ++i) {
      if (!backgroundFileNames[i].isEmpty())
        writer << gUrlNames(i) << "='\"" << backgroundFileNames[i] << "\"' ";
      if (!irradianceFileNames[i].isEmpty())
        writer << gIrradianceUrlNames(i) << "='\"" << irradianceFileNames[i] << "\"' ";
    }
  } else if (writer.isVrml()) {
    for (int i = 0; i < 6; ++i) {
      if (!backgroundFileNames[i].isEmpty()) {
        writer.indent();
        writer << gUrlNames(i) << " [ \"" << backgroundFileNames[i] << "\" ]\n";
      }
      if (!irradianceFileNames[i].isEmpty()) {
        writer.indent();
        writer << gIrradianceUrlNames(i) << " [ \"" << irradianceFileNames[i] << "\" ]\n";
      }
    }
  } else
    WbNode::exportNodeFields(writer);
}
