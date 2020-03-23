#pragma once

#include <string>
#include <vector>

namespace evo
{

// TODO: make this base class and create 2dtexture, cubemap, rendertexture?!
// TODO: how to handle mipmaps in constructors?

class Texture final
{
public:
  /**
   * Loads and creates a R/RG/RGB/RGBA 8 bit image and generates mipmaps and
   * sets the texture to high quality.
   */
  Texture(std::string const & textureFileName);

  Texture(Texture const &) = delete;

  Texture & operator=(Texture const &) = delete;

  ~Texture();

  uint32_t getWidth() const;

  uint32_t getHeight() const;

  void use() const; // TODO: really const? shader can change data

  unsigned int getHandle() const;

  void repeat();

  void mirroredRepeat();

  void clampToBorder();

  void clampToEdge();

  void lowQuality();

  void mediumQuality();

  void highQuality();

private:

  unsigned int m_handle{0};

  uint32_t m_width{0};

  uint32_t m_height{0};

  unsigned int m_internalFormat{0};
};

} // namespace g3d
