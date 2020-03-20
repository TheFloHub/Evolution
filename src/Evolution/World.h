#pragma once
#include "Apple.h"
#include "Person.h"
#include "Terrain.h"
#include "Graphics/Camera.h"
#include <random>
#include <vector>

namespace evo
{
class World final
{
public:
  void update(double const deltaTime);
  void render() const;

  std::vector<Apple> const & getApples() const;
  std::vector<Person> const & getPersons() const;
  Vector3f const & getSize() const;
  void createTerrain(Vector3f const & size);
  Terrain const & getTerrain() const;
  Vector3f getRandomPosition() const;

private:
  std::vector<Apple> m_apples{};
  std::vector<Person> m_persons{};
  Terrain m_terrain;
  Camera m_camera; // Does the world really own the camera?
};

} // namespace evo