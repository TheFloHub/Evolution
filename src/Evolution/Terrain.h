#pragma once
#include "Math/MathTypes.h"

namespace evo
{

class Terrain final
{
public:
  Terrain();
  Terrain(Terrain const & other);
  explicit Terrain(Vector3f const & size);

  Terrain& operator=(Terrain const & other);

  Vector3f const & getSize() const;

  void render() const;

private:
  Vector3f m_size{0.0f, 0.0f, 0.0f};
};
} // namespace evo