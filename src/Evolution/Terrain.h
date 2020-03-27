#pragma once
#include "Math/MathTypes.h"
#include "Graphics/Mesh.h"
#include "Graphics/Texture.h"
#include <string>
#include <memory>

namespace evo
{

class Terrain final
{
public:
  Terrain();
  Terrain(Terrain const & other) = delete;
  Terrain & operator=(Terrain const & other) = delete;

  void createFromImage(std::string const & imageFilePath,
                       Vector3f const & size);
  Vector3f const & getSize() const;

  void render() const;

private:
  // length of the terrain in x, y and z (up)
  Vector3f m_size{0.0f, 0.0f, 0.0f};
  // Number of vertices in x direction.
  uint32_t m_width{0};
  // Number of vertices in y direction.
  uint32_t m_height{0};
  // the heights of the terrain [0-1]
  std::vector<float> m_heights;

  // TODO: maybe mesh is not the best way for terrain since
  // it uses each index only once!
  std::unique_ptr<Mesh> m_mesh{nullptr};

  std::unique_ptr<Texture> m_texture{nullptr};
};
} // namespace evo