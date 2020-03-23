#pragma once
#include "Math/MathTypes.h"
#include <string>

namespace evo
{

class Terrain final
{
public:
  Terrain();
  Terrain(Terrain const & other) = delete;
  Terrain& operator=(Terrain const & other) = delete;

  void createFromImage(std::string const & imageFilePath, Vector3f const & size);
  Vector3f const & getSize() const;

  void render() const;

private:
  Vector3f m_size{0.0f, 0.0f, 0.0f};
  uint32_t m_width{0};
  uint32_t m_height{};
  std::vector<Vector3f> m_vertices{};
  std::vector<Vector3f> m_normals{};
};
} // namespace evo