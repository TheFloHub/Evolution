#include "PopulationCounter.h"
#include "gl/glew.h"
#include <iostream>

using namespace std;

namespace evo
{
void PopulationCounter::add(double deltaTime, size_t count)
{
  m_passedTime += deltaTime;
  m_totalCount += count;
  ++m_numCounts;
  if (m_passedTime >= m_timeDiscretization)
  {
    m_chart[m_index] = static_cast<uint32_t>(m_totalCount / m_numCounts);
    ++m_index;
    m_passedTime = 0.0;
    m_totalCount = 0;
    m_numCounts = 0;
    cout << m_chart[m_index - 1] << endl;
    if (m_index >= m_chart.size())
    {
      cout << "Population buffer overflow!" << endl;
      m_index = 0;
      m_filled = true;
    }
  }
}

void PopulationCounter::render(int const startY, int const width,
                               int const height) const
{
  if (m_index == 0 && !m_filled)
  {
    return;
  }

  uint32_t const maxCount =
      *std::max_element(std::begin(m_chart), std::end(m_chart));
  if (maxCount == 0)
  {
    return;
  }
  double const yScale =
      static_cast<double>(height) / static_cast<double>(maxCount);

  if (m_index == 0)
  {
    // TODO:
    return;
  }
  double const xScale =
      static_cast<double>(width) / static_cast<double>(m_index);

  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f);
  double lastX = 0.0;
  for (int i = 0; i < m_index; ++i)
  {
    double const x = (i + 1) * xScale;
    double const y = startY + yScale * static_cast<double>(m_chart[i]);
    glVertex3d(lastX, startY, 0.0);
    glVertex3d(x, startY, 0.0);
    glVertex3d(x, y, 0.0);
    glVertex3d(lastX, y, 0.0);
    lastX = x;
  }
  glEnd();
}
} // namespace evo