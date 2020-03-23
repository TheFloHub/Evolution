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
    //float const sx = m_size.x() / static_cast<float>(m_width);
    //float const sy = m_size.y() / static_cast<float>(m_height);
  }
  stbi_image_free(image);
}

void Terrain::render() const
{
  glBegin(GL_QUADS);

  glColor3f(1.0f, 1.0f, 1.0f);

  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(m_size.x(), 0.0f, 0.0f);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(m_size.x(), m_size.y(), 0.0f);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0.0f, m_size.y(), 0.0f);





  glEnd();
}
} // namespace evo