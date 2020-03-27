#include "Terrain.h"
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
    // setup 2d heightmap
    m_width = static_cast<uint32_t>(width);
    m_height = static_cast<uint32_t>(height);
    uint32_t const numPixels = m_width * m_height;
    m_heights.reserve(numPixels);
    for (uint32_t i = 0; i < numPixels; ++i)
    {
      m_heights.emplace_back(static_cast<float>(image[i])/255.0f);
    }

    // setup mesh
    float const sx = m_size.x() / static_cast<float>(m_width - 1);
    float const sy = m_size.y() / static_cast<float>(m_height - 1);
    float const sz = m_size.z();

    float const texScaleX =
        (1.0f/static_cast<float>(m_width - 1)) *
        (static_cast<float>(m_width - 1) / static_cast<float>(m_width));
    float const texScaleY =
        (1.0f / static_cast<float>(m_height - 1)) *
        (static_cast<float>(m_height - 1) / static_cast<float>(m_height));
    float const texOffsetX = 0.5f * (1.0f / static_cast<float>(m_width));
    float const texOffsetY = 0.5f * (1.0f / static_cast<float>(m_height));
    
    std::vector<Vector3f> vertices3{};
    std::vector<Vector2f> texCoords2{};
    vertices3.reserve(m_width * m_height);
    texCoords2.reserve(m_width * m_height);
    size_t i = 0;
    for (uint32_t y = 0; y < m_height; ++y)
    {
      for (uint32_t x = 0; x < m_width; ++x)
      {
        float const xf = static_cast<float>(x);
        float const yf = static_cast<float>(y);
        vertices3.emplace_back(xf * sx, yf * sy, m_heights[i] * sz);
        texCoords2.emplace_back(texOffsetX + xf * texScaleX,
                               texOffsetY + yf * texScaleY);
        ++i;
      }
    }

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;
    std::vector<GLuint> indices;
    GLuint index = 0;
    for (uint32_t y = 0; y < m_height - 1; ++y)
    {
      for (uint32_t x = 0; x < m_width - 1; ++x)
      {
        uint32_t const bl = y * m_width + x;
        uint32_t const br = bl + 1;
        uint32_t const tl = bl + m_width;
        uint32_t const tr = tl + 1;
        
        Vector3f const & vbl = vertices3[bl];
        Vector3f const & vbr = vertices3[br];
        Vector3f const & vtl = vertices3[tl];
        Vector3f const & vtr = vertices3[tr];

        Vector2f const & tbl = texCoords2[bl];
        Vector2f const & tbr = texCoords2[br];
        Vector2f const & ttl = texCoords2[tl];
        Vector2f const & ttr = texCoords2[tr];

        Vector3f const normal1 = (vbr - vbl).cross(vtl - vbl).normalized();
        Vector3f const normal2 = (vtl - vtr).cross(vbr - vtr).normalized();

        // first face

        // vertices
        vertices.emplace_back(vbl.x());
        vertices.emplace_back(vbl.y());
        vertices.emplace_back(vbl.z());

        vertices.emplace_back(vbr.x());
        vertices.emplace_back(vbr.y());
        vertices.emplace_back(vbr.z());

        vertices.emplace_back(vtl.x());
        vertices.emplace_back(vtl.y());
        vertices.emplace_back(vtl.z());

        // normals
        normals.emplace_back(normal1.x());
        normals.emplace_back(normal1.y());
        normals.emplace_back(normal1.z());

        normals.emplace_back(normal1.x());
        normals.emplace_back(normal1.y());
        normals.emplace_back(normal1.z());

        normals.emplace_back(normal1.x());
        normals.emplace_back(normal1.y());
        normals.emplace_back(normal1.z());

        // tex
        texCoords.emplace_back(tbl.x());
        texCoords.emplace_back(tbl.y());

        texCoords.emplace_back(tbr.x());
        texCoords.emplace_back(tbr.y());

        texCoords.emplace_back(ttl.x());
        texCoords.emplace_back(ttl.y());

        // indices
        indices.emplace_back(index);
        ++index;
        indices.emplace_back(index);
        ++index;
        indices.emplace_back(index);
        ++index;

        // second face

                // vertices
        vertices.emplace_back(vtl.x());
        vertices.emplace_back(vtl.y());
        vertices.emplace_back(vtl.z());

        vertices.emplace_back(vbr.x());
        vertices.emplace_back(vbr.y());
        vertices.emplace_back(vbr.z());

        vertices.emplace_back(vtr.x());
        vertices.emplace_back(vtr.y());
        vertices.emplace_back(vtr.z());

        // normals
        normals.emplace_back(normal2.x());
        normals.emplace_back(normal2.y());
        normals.emplace_back(normal2.z());

        normals.emplace_back(normal2.x());
        normals.emplace_back(normal2.y());
        normals.emplace_back(normal2.z());

        normals.emplace_back(normal2.x());
        normals.emplace_back(normal2.y());
        normals.emplace_back(normal2.z());

        // tex
        texCoords.emplace_back(ttl.x());
        texCoords.emplace_back(ttl.y());

        texCoords.emplace_back(tbr.x());
        texCoords.emplace_back(tbr.y());

        texCoords.emplace_back(ttr.x());
        texCoords.emplace_back(ttr.y());

        // indices
        indices.emplace_back(index);
        ++index;
        indices.emplace_back(index);
        ++index;
        indices.emplace_back(index);
        ++index;
      }
    }
    m_mesh = std::make_unique<Mesh>(vertices, normals, texCoords, indices);


    m_texture = std::make_unique<Texture>(imageFilePath);
    //uint32_t const nw = 2 * (m_width - 1);
    //uint32_t const nh = 2 * (m_height - 1);


  }
  stbi_image_free(image);
}

void Terrain::render() const
{
  if (m_texture)
  {
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    m_texture->use();
  }
  glColor3f(1.0f, 1.0f, 1.0f);
  if (m_mesh)
  {
    m_mesh->render();
  }
  //glBegin(GL_TRIANGLES); // STRIP?
  //glColor3f(1.0f, 1.0f, 1.0f);
  //uint32_t ni = 0;
  //for (uint32_t y = 0; y < m_height-1; ++y)
  //{
  //  for (uint32_t x = 0; x < m_width-1; ++x)
  //  {
  //    uint32_t const bl = y * m_width + x;
  //    uint32_t const br = bl + 1;
  //    uint32_t const tl = bl + m_width;
  //    uint32_t const tr = tl + 1;

  //    glNormal3fv(m_normals[ni].data());
  //    glVertex3fv(m_vertices[bl].data());
  //    glNormal3fv(m_normals[ni].data());
  //    glVertex3fv(m_vertices[br].data());
  //    glNormal3fv(m_normals[ni].data());
  //    glVertex3fv(m_vertices[tl].data());
  //    ++ni;

  //    glNormal3fv(m_normals[ni].data());
  //    glVertex3fv(m_vertices[tl].data());
  //    glNormal3fv(m_normals[ni].data());
  //    glVertex3fv(m_vertices[br].data());
  //    glNormal3fv(m_normals[ni].data());
  //    glVertex3fv(m_vertices[tr].data());
  //    ++ni;
  //  }
  //}
  //glEnd();

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