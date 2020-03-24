#include "Terrain.h"
#include "Graphics/Mesh.h"
#include "gl/glew.h"
#include "Importer/stb_image.h"
#include <iostream>

namespace evo
{
Terrain::Terrain() = default;
Vector3f const & Terrain::getSize() const { return m_size; }

void Terrain::createFromImage(std::string const & imageFilePath,
                              Vector3f const & size)

{
  m_size = size;
  int width = 0;
  int height = 0;
  int channels = 0;
  stbi_set_flip_vertically_on_load(true);
  unsigned char * image =
      stbi_load(imageFilePath.c_str(), &width, &height, &channels, 0);
  std::cout << height << " " << width << " " << channels << std::endl;

  if (channels != 1)
  {
    std::cout << "Terrain height map could not be loaded." << std::endl;
  }
  else
  {
    m_width = static_cast<uint32_t>(width);
    m_height = static_cast<uint32_t>(height);
    float const sx = m_size.x() / static_cast<float>(m_width-1);
    float const sy = m_size.y() / static_cast<float>(m_height-1);
    float const sz = m_size.z() / 255.0f;

    m_vertices.reserve(m_width * m_height);
    size_t i = 0;
    for (uint32_t y = 0; y < m_height; ++y)
    {
      for (uint32_t x = 0; x < m_width; ++x)
      {
        m_vertices.emplace_back(static_cast<float>(x) * sx,
                                 static_cast<float>(y) * sy,
                                 static_cast<float>(image[i]) * sz);
        ++i;
      }
    }

    for (uint32_t y = 0; y < m_height - 1; ++y)
    {
      for (uint32_t x = 0; x < m_width - 1; ++x)
      {
        uint32_t const bl = y * m_width + x;
        uint32_t const br = bl + 1;
        uint32_t const tl = bl + m_width;
        uint32_t const tr = tl + 1;

        m_normals.emplace_back((m_vertices[br] - m_vertices[bl])
                                   .cross(m_vertices[tl] - m_vertices[bl])
                                   .normalized());

        m_normals.emplace_back((m_vertices[tl] - m_vertices[tr])
                                   .cross(m_vertices[br] - m_vertices[tr])
                                   .normalized());
      }
    }


    uint32_t const nw = 2 * (m_width - 1);
    uint32_t const nh = 2 * (m_height - 1);


  }
  stbi_image_free(image);
}

void Terrain::render() const
{
  glBegin(GL_TRIANGLES); // STRIP?
  glColor3f(1.0f, 1.0f, 1.0f);
  uint32_t ni = 0;
  for (uint32_t y = 0; y < m_height-1; ++y)
  {
    for (uint32_t x = 0; x < m_width-1; ++x)
    {
      uint32_t const bl = y * m_width + x;
      uint32_t const br = bl + 1;
      uint32_t const tl = bl + m_width;
      uint32_t const tr = tl + 1;

      glNormal3fv(m_normals[ni].data());
      glVertex3fv(m_vertices[bl].data());
      glNormal3fv(m_normals[ni].data());
      glVertex3fv(m_vertices[br].data());
      glNormal3fv(m_normals[ni].data());
      glVertex3fv(m_vertices[tl].data());
      ++ni;

      glNormal3fv(m_normals[ni].data());
      glVertex3fv(m_vertices[tl].data());
      glNormal3fv(m_normals[ni].data());
      glVertex3fv(m_vertices[br].data());
      glNormal3fv(m_normals[ni].data());
      glVertex3fv(m_vertices[tr].data());
      ++ni;
    }
  }
  glEnd();

  //glBegin(GL_QUADS);
  //glColor3f(1.0f, 1.0f, 1.0f);
  //glNormal3f(0.0f, 0.0f, 1.0f);
  //glVertex3f(0.0f, 0.0f, 0.0f);
  //glNormal3f(0.0f, 0.0f, 1.0f);
  //glVertex3f(m_size.x(), 0.0f, 0.0f);
  //glNormal3f(0.0f, 0.0f, 1.0f);
  //glVertex3f(m_size.x(), m_size.y(), 0.0f);
  //glNormal3f(0.0f, 0.0f, 1.0f);
  //glVertex3f(0.0f, m_size.y(), 0.0f);
  //glEnd();
}
} // namespace evo