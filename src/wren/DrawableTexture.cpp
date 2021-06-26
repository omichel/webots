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

#include "DrawableTexture.hpp"
#include "GlState.hpp"

#include <wren/drawable_texture.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace wren {

  void DrawableTexture::clear() {
    for (int i = 0; i < width() * height(); ++i)
      mData[i] = mClearColor;

    mDirty = true;
    mDirtyMinX = 0;
    mDirtyMaxX = width();
    mDirtyMinY = 0;
    mDirtyMaxY = height();
  }

  void DrawableTexture::drawPixel(int x, int y) {
    assert(x >= 0 && x < width());
    assert(y >= 0 && y < height());
    updateDirtyRect(x, y);
    mData[y * width() + x] = mColor;
    mDirty = true;
  }

  void DrawableTexture::drawText(const char *text, int x, int y) {
    if (x >= width() || y >= height())
      return;  // out of the uppest bounds
    int cx = x;
    int cy = y;
    int l = strlen(text);

    wchar_t *wcharText = new wchar_t[l + 1];
#ifdef _WIN32  // mbstowcs doesn't work properly on Windows
    l = MultiByteToWideChar(CP_UTF8, 0, text, -1, wcharText, l + 1) - 1;
#else
    // cppcheck-suppress uninitdata
    l = mbstowcs(wcharText, text, l + 1);
#endif

    int fontSize = mFont->fontSize();
    for (int i = 0; i < l; i++) {
      if (text[i] == L'\n') {  // carriage return
        cy += mFont->verticalSpace();
        cx = x;
      } else {  // draw char
        int size = fontSize;
        if (cx + fontSize >= 0 && cx < width() && cy + fontSize >= 0 && cy < height()) {  // if char is visible
          size = drawChar(wcharText[i], cx, cy);
        }
        cx += size;
      }
    }
    delete[] wcharText;
  }

  void DrawableTexture::setSize(int width, int height) {
    int oldWidth = Texture::width();
    int oldHeight = Texture::height();

    if (width == oldWidth && height == oldHeight)
      return;

    Texture::setSize(width, height);

    int *newData = new int[width * height];

    // Check if texture has already been modified
    if (mData) {
      // Copy content of old image into new (crop if new is smaller, filled with clear color if bigger)
      int oldSize = oldWidth * oldHeight;
      for (int i = 0; i < width * height; ++i) {
        if (i >= oldSize)
          newData[i] = mClearColor;
        else
          newData[i] = mData[i];
      }

      delete[] mData;
      mData = newData;

      // Replace old texture if texture setup was already made
      if (mGlName) {
        Texture::bind(DEFAULT_USAGE_PARAMS);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, newData);
        mHaveMipMapsBeenGenerated = false;
      }
    } else {
      mData = newData;
      clear();
    }
  }

  DrawableTexture::DrawableTexture() :
    mFont(NULL),
    mFontAntiAliasing(true),
    mPremultipliedAlpha(false),
    mData(NULL),
    mDirty(true),
    mDirtyMinX(0),
    mDirtyMaxX(0),
    mDirtyMinY(0),
    mDirtyMaxY(0),
    mGlName(0) {
    mClearColor = 0x0;    // transparent
    mColor = 0xFFFFFFFF;  // white
  }

  DrawableTexture::~DrawableTexture() { delete[] mData; }

  void DrawableTexture::updateDirtyRect(int x, int y) {
    if (x < mDirtyMinX)
      mDirtyMinX = x;

    if (x > mDirtyMaxX - 1)
      mDirtyMaxX = x + 1;

    if (y < mDirtyMinY)
      mDirtyMinY = y;

    if (y > mDirtyMaxY - 1)
      mDirtyMaxY = y + 1;

    assert(mDirtyMinX >= 0 && mDirtyMaxX <= width());
    assert(mDirtyMinY >= 0 && mDirtyMaxY <= height());
  }

  int DrawableTexture::drawChar(unsigned long c, int baselineX, int baselineY) {
    int characterWidth, characterHeight, verticalOffset, horizontalOffset, transparencyFactor, horizontalAdvance, pitch;
    unsigned char *buffer = mFont->generateCharBuffer(c, mFontAntiAliasing, &characterWidth, &characterHeight, &verticalOffset,
                                                      &horizontalOffset, &transparencyFactor, &horizontalAdvance, &pitch);
    if (c == L' ')
      return horizontalAdvance;

    assert(buffer);
    if (!buffer)
      return 0;

    const int x = baselineX + horizontalOffset;
    const int y = baselineY - verticalOffset + mFont->verticalSpace() + mFont->descender();

    const int width = Texture::width();
    const int height = Texture::height();

    const unsigned char originalOpacity = mColor >> 24;

    for (int j = 0; j < characterHeight; ++j) {
      if (y + j >= height || y + j < 0)
        continue;
      const int bufferOffset = j * characterWidth;
      for (int i = 0; i < characterWidth; ++i) {
        if (x + i >= width || x + i < 0)
          continue;

        if (mFontAntiAliasing) {
          const unsigned char pixelValue = buffer[bufferOffset + i];
          if (pixelValue > 0) {
            const int savedColor = mColor;

            const float pixelAlpha = pixelValue / static_cast<float>(transparencyFactor);
            unsigned char alpha = originalOpacity * pixelAlpha;
            if (mPremultipliedAlpha) {
              mColor = (alpha << 24) | (static_cast<int>(((mColor >> 16) & 0xFF) * pixelAlpha) << 16) |
                       (static_cast<int>(((mColor >> 8) & 0xFF) * pixelAlpha) << 8) |
                       static_cast<int>((mColor & 0xFF) * pixelAlpha);
            } else
              mColor = (mColor & 0x00FFFFFF) | (alpha << 24);

            drawPixel(x + i, height - y - j);

            mColor = savedColor;
          }
        } else {
          // get bit from current byte corresponding to current pixel
          const unsigned char *row = &buffer[pitch * j];
          const unsigned char cValue = row[i >> 3];
          if ((cValue & (128 >> (i & 7))) != 0)
            drawPixel(x + i, height - y - j);
        }
      }
    }

    return horizontalAdvance;
  }

  int DrawableTexture::toInt(const float *color) const {
    if (mPremultipliedAlpha) {
      return (static_cast<int>(255 * color[3]) << 24) | (static_cast<int>(255 * (color[2] * color[3])) << 16) |
             (static_cast<int>(255 * (color[1] * color[3])) << 8) | (static_cast<int>(255 * (color[0] * color[3])));
    } else {
      return (static_cast<int>(255 * color[3]) << 24) | (static_cast<int>(255 * color[2]) << 16) |
             (static_cast<int>(255 * color[1]) << 8) | (static_cast<int>(255 * color[0]));
    }
  }

  void DrawableTexture::prepareGl() {
    assert(!mGlName);
    assert(mData);

    if (textureUnit() < 0)
      setTextureUnit(0);

    mGlName = generateNewTexture();

    if (textureUnit() < 0)
      setTextureUnit(0);

    Texture::bind(Texture::DEFAULT_USAGE_PARAMS);

    // Some texture parameters (format, internal format & data type) are fixed since the class behaviour relies on them
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, mData);

    mDirty = false;
    resetDirtyRect();
  }

  void DrawableTexture::bind(const Texture::UsageParams &params) {
    Texture::bind(params);

    // Update texture if necessary
    if (mDirty) {
      // Copy texture subportion that needs to be updated
      const int width = mDirtyMaxX - mDirtyMinX;
      const int height = mDirtyMaxY - mDirtyMinY;
      int *data = new int[width * height];
      for (int y = mDirtyMinY; y < mDirtyMaxY; ++y) {
        const int index1 = (y - mDirtyMinY) * width;
        const int index2 = y * Texture::width();
        memcpy(&data[index1], &mData[index2], width * sizeof(int));
      }

      glTexSubImage2D(GL_TEXTURE_2D, 0, mDirtyMinX, mDirtyMinY, width, height, GL_BGRA, GL_UNSIGNED_BYTE, data);
      if (params.mAreMipMapsEnabled)
        generateMipMaps(true);

      delete[] data;
      mDirty = false;
      resetDirtyRect();
    }
  }

}  // namespace wren

// C interface implementation
WrDrawableTexture *wr_drawable_texture_new() {
  return reinterpret_cast<WrDrawableTexture *>(wren::DrawableTexture::createDrawableTexture());
}

void wr_drawable_texture_set_font(WrDrawableTexture *texture, WrFont *font) {
  reinterpret_cast<wren::DrawableTexture *>(texture)->setFont(reinterpret_cast<wren::Font *>(font));
}

void wr_drawable_texture_set_color(WrDrawableTexture *texture, float *color) {
  reinterpret_cast<wren::DrawableTexture *>(texture)->setColor(color);
}

void wr_drawable_texture_set_antialasing(WrDrawableTexture *texture, bool enabled) {
  reinterpret_cast<wren::DrawableTexture *>(texture)->setAntialiasing(enabled);
}

void wr_drawable_texture_set_use_premultiplied_alpha(WrDrawableTexture *texture, bool premultipliedAlpha) {
  reinterpret_cast<wren::DrawableTexture *>(texture)->setUsePremultipliedAlpha(premultipliedAlpha);
}

void wr_drawable_texture_draw_pixel(WrDrawableTexture *texture, int x, int y) {
  reinterpret_cast<wren::DrawableTexture *>(texture)->drawPixel(x, y);
}

void wr_drawable_texture_draw_text(WrDrawableTexture *texture, const char *text, int x, int y) {
  reinterpret_cast<wren::DrawableTexture *>(texture)->drawText(text, x, y);
}

void wr_drawable_texture_clear(WrDrawableTexture *texture) {
  reinterpret_cast<wren::DrawableTexture *>(texture)->clear();
}
