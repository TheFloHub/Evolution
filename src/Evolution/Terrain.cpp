#include "Terrain.h"
#include "gl/glew.h"

namespace evo
{
Terrain::Terrain() = default;
Terrain::Terrain(Terrain const & other) = default;
Terrain & Terrain::operator=(Terrain const & other) = default;
Terrain::Terrain(Vector3f const & size) : m_size(size) {}

Vector3f const & Terrain::getSize() const { return m_size; }
void Terrain::render() const
{
  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, m_size.z());
  glVertex3f(m_size.x(), 0.0f, m_size.z());
  glVertex3f(m_size.x(), 0.0f, 0.0f);
  glEnd();
}
} // namespace evo