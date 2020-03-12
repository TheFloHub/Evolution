#include "Graphics3d/Assets/Texture.h"
#include "Graphics3d/GlInfo.h"
#include "Graphics3d/Importer/stb_image.h"

#include <iostream>

g3d::Texture::Texture(std::string const & textureFileName)
    : mTextureHandle(0), mWidth(0), mHeight(0), mInternalFormat(9999)
{
  int width = 0;
  int height = 0;
  int channels = 0;
  stbi_set_flip_vertically_on_load(true);
  unsigned char * image =
      stbi_load(textureFileName.c_str(), &width, &height, &channels, 0);
  if (image != nullptr)
  {
    mWidth = (unsigned int)width;
    mHeight = (unsigned int)height;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mTextureHandle);
    glBindTexture(GL_TEXTURE_2D, mTextureHandle);
    CHECKGLERROR();
    if (channels == 1)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
                   GL_UNSIGNED_BYTE, image);
      mInternalFormat = GL_RED;
      CHECKGLERROR();
    }
    else if (channels == 2)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG,
                   GL_UNSIGNED_BYTE, image);
      mInternalFormat = GL_RG;
      CHECKGLERROR();
    }
    else if (channels == 3)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, image);
      mInternalFormat = GL_RGB;
      CHECKGLERROR();
    }
    else if (channels == 4)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, image);
      mInternalFormat = GL_RGBA;
      CHECKGLERROR();
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    CHECKGLERROR();
    repeat();
    highQuality();
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  else
  {
    std::cout << "Failed to load texture " << textureFileName.c_str()
              << std::endl;
  }
  stbi_image_free(image);
}

g3d::Texture::Texture(unsigned int width, unsigned int height,
                      unsigned int channels,
                      std::vector<unsigned char> const & data)
    : mTextureHandle(0), mWidth(width), mHeight(height), mInternalFormat(9999)
{
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &mTextureHandle);
  glBindTexture(GL_TEXTURE_2D, mTextureHandle);
  if (channels == 1)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
                 GL_UNSIGNED_BYTE, data.data());
    mInternalFormat = GL_RED;
  }
  else if (channels == 2)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG,
                 GL_UNSIGNED_BYTE, data.data());
    mInternalFormat = GL_RG;
  }
  else if (channels == 3)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data.data());
    mInternalFormat = GL_RGB;
  }
  else if (channels == 4)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data.data());
    mInternalFormat = GL_RGBA;
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  repeat();
  highQuality();
  glBindTexture(GL_TEXTURE_2D, 0);
}

g3d::Texture::Texture(unsigned int width, unsigned int height,
                      unsigned int channels, std::vector<float> const & data)
    : mTextureHandle(0), mWidth(width), mHeight(height), mInternalFormat(9999)
{
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &mTextureHandle);
  glBindTexture(GL_TEXTURE_2D, mTextureHandle);
  if (channels == 1)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED, GL_FLOAT,
                 data.data());
    mInternalFormat = GL_R16F;
    CHECKGLERROR();
  }
  else if (channels == 2)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT,
                 data.data());
    mInternalFormat = GL_RG16F;
    CHECKGLERROR();
  }
  else if (channels == 3)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB,
                 GL_FLOAT, data.data());
    mInternalFormat = GL_RGB16F;
    CHECKGLERROR();
  }
  else if (channels == 4)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA,
                 GL_FLOAT, data.data());
    mInternalFormat = GL_RGBA16F;
    CHECKGLERROR();
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  CHECKGLERROR();
  repeat();
  CHECKGLERROR();
  highQuality();
  CHECKGLERROR();
  glBindTexture(GL_TEXTURE_2D, 0);
}

g3d::Texture::Texture(unsigned int width, unsigned int height,
                      GLenum internalFormat)
    : mTextureHandle(0), mWidth(width), mHeight(height),
      mInternalFormat(internalFormat)
{
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &mTextureHandle);
  glBindTexture(GL_TEXTURE_2D, mTextureHandle);
  // TODO: maybe use glTexImage2D here which makes it possible to resize the
  // image later but its forever fixed with glTexStorage2D
  glTexStorage2D(GL_TEXTURE_2D, 1, mInternalFormat, mWidth, mHeight);
  CHECKGLERROR();

  clampToBorder();
  CHECKGLERROR();
  lowQuality();
  CHECKGLERROR();
  glBindTexture(GL_TEXTURE_2D, 0);
}

g3d::Texture::~Texture()
{
  if (mTextureHandle != 0)
    glDeleteTextures(1, &mTextureHandle);
}

void g3d::Texture::use() const { glBindTexture(GL_TEXTURE_2D, mTextureHandle); }

void g3d::Texture::repeat()
{
  glBindTexture(GL_TEXTURE_2D, mTextureHandle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void g3d::Texture::mirroredRepeat()
{
  glBindTexture(GL_TEXTURE_2D, mTextureHandle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void g3d::Texture::clampToBorder()
{
  glBindTexture(GL_TEXTURE_2D, mTextureHandle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  static float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void g3d::Texture::clampToEdge()
{
  glBindTexture(GL_TEXTURE_2D, mTextureHandle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void g3d::Texture::lowQuality()
{
  glBindTexture(GL_TEXTURE_2D, mTextureHandle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void g3d::Texture::mediumQuality()
{
  glBindTexture(GL_TEXTURE_2D, mTextureHandle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void g3d::Texture::highQuality()
{
  glBindTexture(GL_TEXTURE_2D, mTextureHandle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int g3d::Texture::getWidth() const { return mWidth; }

unsigned int g3d::Texture::getHeight() const { return mHeight; }

GLuint g3d::Texture::getHandle() const { return mTextureHandle; }
