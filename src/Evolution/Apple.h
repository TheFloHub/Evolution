#pragma once
#include <Math/MathTypes.h>

namespace evo
{
double const g_appleLifeTime = 5.0;
double const g_appleEnergy = 25;
struct Apple
{
  // TODO: energy = lifetime?
  // energy decreases, giving also less energy when eaten
  math::Vector2d m_position{0.0, 0.0};
  double m_lifeTime{g_appleLifeTime};
  double m_passedTime{0.0};
  double m_energy{g_appleEnergy};
};
} // namespace evo