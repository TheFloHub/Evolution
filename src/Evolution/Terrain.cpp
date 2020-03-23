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
  //stbi_set_flip_vertically_on_load(true);
  unsigned char * image =
      stbi_load(imageFilePath.c_str(), &width, &height, &channels, 0);
  std::cout << height << " " << width << " " << channels << std::endl;



  stbi_image_free(image);
}

void Terrain::render() const
{
  static Mesh mesh("D:\\Eigene Daten\\Dokumente\\3D Modelle\\teddy.obj");

  glPushMatrix();
  glTranslatef(400.0f, 100.0f, 400.0f);
  glScalef(300.0f, 300.0f, 300.0f);
  glColor3f(1.0f, 1.0f, 1.0f);
  mesh.render();
  glPopMatrix();

  glBegin(GL_QUADS);
 
  glVertex3f(0.0f, 0.0f, 0.0f);
  glNormal3f(0.0f, 1.0f, 0.0f);
  glColor3f(1.0f, 1.0f, 1.0f);

  glVertex3f(0.0f, 0.0f, m_size.z());
  glNormal3f(0.0f, 1.0f, 0.0f);
  glColor3f(1.0f, 1.0f, 1.0f);

  glVertex3f(m_size.x(), 0.0f, m_size.z());
  glNormal3f(0.0f, 1.0f, 0.0f);
  glColor3f(1.0f, 1.0f, 1.0f);

  glVertex3f(m_size.x(), 0.0f, 0.0f);
  glNormal3f(0.0f, 1.0f, 0.0f);
  glColor3f(1.0f, 1.0f, 1.0f);

  glEnd();
}
} // namespace evo