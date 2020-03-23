#include "Graphics/Texture.h"
#include "GL/glew.h"
#include "GlInfo.h"
#include "Importer/stb_image.h"
#include <iostream>

namespace evo
{

Texture::Texture(std::string const & textureFileName)
{
  int width = 0;
  int height = 0;
  int channels = 0;
  stbi_set_flip_vertically_on_load(true);
  unsigned char * image =
      stbi_load(textureFileName.c_str(), &width, &height, &channels, 0);
  if (image != nullptr)
  {
    m_width = static_cast<uint32_t>(width);
    m_height = static_cast<uint32_t>(height);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_2D, m_handle);
    CHECKGLERROR();
    if (channels == 1)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
                   GL_UNSIGNED_BYTE, image);
      m_internalFormat = GL_RED;
      CHECKGLERROR();
    }
    else if (channels == 2)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG,
                   GL_UNSIGNED_BYTE, image);
      m_internalFormat = GL_RG;
      CHECKGLERROR();
    }
    else if (channels == 3)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, image);
      m_internalFormat = GL_RGB;
      CHECKGLERROR();
    }
    else if (channels == 4)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, image);
      m_internalFormat = GL_RGBA;
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

Texture::~Texture()
{
  if (m_handle != 0)
    glDeleteTextures(1, &m_handle);
}

void Texture::use() const { glBindTexture(GL_TEXTURE_2D, m_handle); }

void Texture::repeat()
{
  glBindTexture(GL_TEXTURE_2D, m_handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::mirroredRepeat()
{
  glBindTexture(GL_TEXTURE_2D, m_handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::clampToBorder()
{
  glBindTexture(GL_TEXTURE_2D, m_handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  static float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::clampToEdge()
{
  glBindTexture(GL_TEXTURE_2D, m_handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::lowQuality()
{
  glBindTexture(GL_TEXTURE_2D, m_handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::mediumQuality()
{
  glBindTexture(GL_TEXTURE_2D, m_handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::highQuality()
{
  glBindTexture(GL_TEXTURE_2D, m_handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::getWidth() const { return m_width; }

unsigned int Texture::getHeight() const { return m_height; }

GLuint Texture::getHandle() const { return m_handle; }

} // namespace evo