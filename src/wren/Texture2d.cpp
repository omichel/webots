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

#include "Texture2d.hpp"

#include "Debug.hpp"
#include "GlState.hpp"

#include <wren/texture_2d.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <cstring>

namespace wren {

  std::unordered_map<cache::Key, cache::Texture2dData> Texture2d::cCache;

  Texture2d *Texture2d::copyFromCache(const std::string &filePath) {
    Texture2d *texture = NULL;

    const cache::Key key(cache::sipHash13c(filePath.data(), filePath.size()));
    auto it = Texture2d::cCache.find(key);
    if (it != Texture2d::cCache.end()) {
      texture = new Texture2d();
      texture->mCacheKey = key;
      texture->mCacheData = &(it->second);
      ++texture->mCacheData->mNumUsers;

      texture->mWidth = texture->mCacheData->mWidth;
      texture->mHeight = texture->mCacheData->mHeight;
      texture->mIsTranslucent = texture->mCacheData->mIsTranslucent;
      texture->mIsCachePersistent = texture->mCacheData->mIsCachePersistent;
      texture->mFilePath = filePath;

      texture->setTextureUnit(0);
    }

    return texture;
  }

  size_t Texture2d::cachedItemCount() {
    size_t nonPersistentCount = 0;
    for (std::pair<cache::Key, cache::Texture2dData> cachedTexture : cCache)
      if (!cachedTexture.second.mIsCachePersistent)
        ++nonPersistentCount;
    return nonPersistentCount;
  }

  void Texture2d::setup() {
    assert(mWidth);
    assert(mHeight);
    assert(mCacheKey.mHash == 0);

    // if no file path was supplied, supply a unique string
    if (!mFilePath.size()) {
      static int uniqueCounter = 0;
      mFilePath = "Texture2d_" + std::to_string(uniqueCounter++);
    }

    mCacheKey = cache::Key(cache::sipHash13c(mFilePath.data(), mFilePath.size()));
    auto it = Texture2d::cCache.find(mCacheKey);
    if (it != Texture2d::cCache.end()) {
      mCacheData = &(it->second);
      ++mCacheData->mNumUsers;
      setTextureUnit(0);
    } else {
      Texture2d::cCache.emplace(mCacheKey, cache::Texture2dData(*this));
      mCacheData = &(Texture2d::cCache.at(mCacheKey));
      Texture::setup();
    }
  }

  Texture2d::Texture2d() : mData(NULL), mIsCachePersistent(false), mCacheData(NULL) {}

  void Texture2d::prepareGl() {
    assert(mCacheData);
    assert(!mCacheData->mGlName);

    // DEBUG("Texture2d::prepareGl, this=" << this << ", mWidth=" << mWidth << ", mHeight=" << mHeight << ", mFilePath=" <<
    // mFilePath);

    mCacheData->mGlName = generateNewTexture();

    if (textureUnit() < 0)
      setTextureUnit(0);

    bind(Texture::DEFAULT_USAGE_PARAMS);

    if (!mData) {
      char *data = Texture::generateBlackData(mGlFormatParams, mWidth, mHeight);
      glTexImage2D(GL_TEXTURE_2D, 0, mGlFormatParams.mInternalFormat, mWidth, mHeight, 0, mGlFormatParams.mFormat,
                   mGlFormatParams.mDataType, data);
      delete[] data;
    } else
      glTexImage2D(GL_TEXTURE_2D, 0, mGlFormatParams.mInternalFormat, mWidth, mHeight, 0, mGlFormatParams.mFormat,
                   mGlFormatParams.mDataType, mData);

    mData = NULL;
  }

  void Texture2d::cleanupGl() {
    if (!mCacheData)
      return;

    --mCacheData->mNumUsers;

    if (!mCacheData->mNumUsers) {
      Texture::cleanupGl();
      Texture2d::cCache.erase(mCacheKey);
    } else
      release();

    mCacheKey = cache::Key();
    mCacheData = NULL;
  }

}  // namespace wren

// C interface implementation
static size_t maxLength = 4096;  // Max. path length for linux as defined in limits.h

WrTexture2d *wr_texture_2d_new() {
  return reinterpret_cast<WrTexture2d *>(wren::Texture2d::createTexture2d());
}

WrTexture2d *wr_texture_2d_copy_from_cache(const char *file_path) {
  std::string s(file_path, std::min(std::strlen(file_path), maxLength));
  return reinterpret_cast<WrTexture2d *>(wren::Texture2d::copyFromCache(s));
}

void wr_texture_2d_set_data(WrTexture2d *texture, const char *data) {
  reinterpret_cast<wren::Texture2d *>(texture)->setData(data);
}

void wr_texture_2d_set_file_path(WrTexture2d *texture, const char *file_path) {
  std::string s(file_path, std::min(std::strlen(file_path), maxLength));
  reinterpret_cast<wren::Texture2d *>(texture)->setFilePath(s);
}

void wr_texture_2d_set_cache_persistency(WrTexture2d *texture, bool is_persistent) {
  reinterpret_cast<wren::Texture2d *>(texture)->setCachePersistency(is_persistent);
}
