/**
 * @author Florian Feuerstein
 * @date 22.07.2016
 *
 */

#pragma once

#include <gl/glew.h>
#include <string>
#include <vector>

namespace g3d
{

// TODO: make this base class and create 2dtexture, cubemap, rendertexture?!
// TODO: how to handle mipmaps in constructors?

class Texture
{
public:
  /**
   * Loads and creates a R/RG/RGB/RGBA 8 bit image and generates mipmaps and
   * sets the texture to high quality.
   */
  Texture(std::string const & textureFileName);

  /**
   * Creates a R/RG/RGB/RGBA 8 bit image and generates mipmaps and sets the
   * texture to high quality.
   */
  Texture(unsigned int width, unsigned int height, unsigned int channels,
          std::vector<unsigned char> const & data);

  /**
   * Creates a R/RG/RGB/RGBA 16 bit floating point image and generates mipmaps
   * and sets the texture to high quality.
   */
  Texture(unsigned int width, unsigned int height, unsigned int channels,
          std::vector<float> const & data);

  /**
   * Creates any texture without data, sets it to low quality and without
   * mipmaps.
   */
  Texture(unsigned int width, unsigned int height, GLenum internalFormat);

  ~Texture(); // TODO: virtual?

  unsigned int getWidth() const;

  unsigned int getHeight() const;

  void use() const; // TODO: really const? shader can change data

  GLuint getHandle() const;

  void repeat();

  void mirroredRepeat();

  void clampToBorder();

  void clampToEdge();

  void lowQuality();

  void mediumQuality();

  void highQuality();

protected:
  Texture(Texture const &) = delete;
  Texture & operator=(Texture const &) = delete;

  GLuint mTextureHandle;

  unsigned int mWidth;

  unsigned int mHeight;

  GLenum mInternalFormat;
};

} // namespace g3d
