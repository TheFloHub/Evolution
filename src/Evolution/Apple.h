#pragma once
#include <Math/MathTypes.h>

namespace evo
{
double constexpr g_appleLifeTime = 5.0;
double constexpr g_newAppleTime = 1.0;
uint32_t constexpr g_numNewApples = 50;

struct Apple
{
  // TODO: energy = lifetime?
  // energy decreases, giving also less energy when eaten
  Vector3f m_position{0.0f, 0.0f, 0.0f};
  double m_lifeTime{g_appleLifeTime};
  double m_passedTime{0.0};
  double m_energy{150};
};
} // namespace evo