#pragma once
#include "Apple.h"
#include "Person.h"
#include <vector>
#include <random>

namespace evo
{
class EvoSim
{
public:
  static EvoSim & get();
  EvoSim(EvoSim const &) = delete;
  EvoSim & operator=(EvoSim const &) = delete;

  std::vector<Person> m_persons;
  std::vector<Apple> m_apples;

  // gui
  int const m_width{600};
  int const m_height{700};
  int const m_yStart{100};

  // random
  std::random_device m_rd{};
  std::mt19937 m_rng{m_rd()};
  std::uniform_real_distribution<float> m_xdis{0.0f,
                                               static_cast<float>(m_width)};
  std::uniform_real_distribution<float> m_ydis{static_cast<float>(m_yStart),
                                               static_cast<float>(m_height)};

private:
  EvoSim() = default;
};
} // namespace evo