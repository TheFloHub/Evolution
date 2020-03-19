#pragma once
#include <Math/MathTypes.h>

namespace evo
{
struct PopulationCounter
{

  PopulationCounter(double const timeDiscretization, size_t const bufferSize)
      : m_timeDiscretization(timeDiscretization), m_chart(bufferSize, 0)
  {
  }

  // TODO: actually scale count with time and add parts to the next count
  // this one assumes that framerate stays constant and ignores time overflow
  void add(double deltaTime, size_t count);

  void render(int const startY, int const width, int const height) const;

  double const m_timeDiscretization;
  double m_passedTime{0.0};
  size_t m_totalCount{0};
  size_t m_numCounts{0};
  std::vector<uint32_t> m_chart;
  size_t m_index{0};
  bool m_filled{false};
};
} // namespace evo