#include "Graphics/Mesh.h"
#include "Importer/ObjImporter.h"
#include <gl/glew.h>
#include <iostream>

namespace evo
{

Mesh::Mesh(std::string const & objFileName)
    : mVao(0), mVboVertices(0), mVboNormals(0), mVboTexcoords(0),
      mVboIndices(0), mNumberOfVertices(0), mNumberOfElementsToDraw(0)
{
  ObjImporter objImporter(objFileName);
  if (objImporter.isSuccessfullyLoaded())
  {
    generateVbos(objImporter.getVertices(), objImporter.getNormals(),
                 objImporter.getTextureCoords(), objImporter.getIndices());
  }
  else
  {
    std::cout << "Could not load file: " << objFileName << "." << std::endl;
  }
}

Mesh::Mesh(std::vector<GLfloat> const & vertices,
           std::vector<GLfloat> const & normals,
           std::vector<GLfloat> const & texCoords,
           std::vector<GLuint> const & indices)
    : mVao(0), mVboVertices(0), mVboNormals(0), mVboTexcoords(0),
      mVboIndices(0), mNumberOfVertices(0), mNumberOfElementsToDraw(0)
{
  generateVbos(vertices, normals, texCoords, indices);
}

Mesh::~Mesh()
{
  if (mVboVertices != 0)
    glDeleteBuffers(1, &mVboVertices);

  if (mVboNormals != 0)
    glDeleteBuffers(1, &mVboNormals);

  if (mVboTexcoords != 0)
    glDeleteBuffers(1, &mVboTexcoords);

  if (mVboIndices != 0)
    glDeleteBuffers(1, &mVboIndices);

  if (mVao != 0)
    glDeleteVertexArrays(1, &mVao);
}

void Mesh::generateVbos(std::vector<GLfloat> const & vertices,
                        std::vector<GLfloat> const & normals,
                        std::vector<GLfloat> const & texCoords,
                        std::vector<GLuint> const & indices)
{
  bool hasTexCoords = !texCoords.empty();

  // TODO: better consistency checks with exceptions
  if (normals.size() != vertices.size())
    throw std::exception("Different number of normals and vertices.");
  if (hasTexCoords && texCoords.size() / 2 != vertices.size() / 3)
    throw std::exception(
        "Different number of texture coordinates and vertices.");

  glGenVertexArrays(1, &mVao);
  glGenBuffers(1, &mVboVertices);
  glGenBuffers(1, &mVboNormals);
  if (hasTexCoords)
  {
    glGenBuffers(1, &mVboTexcoords);
  }
  glGenBuffers(1, &mVboIndices);

  glBindVertexArray(mVao);

  // vertices
  glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
               vertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // normals
  glBindBuffer(GL_ARRAY_BUFFER, mVboNormals);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat),
               normals.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // tex coords
  if (hasTexCoords)
  {
    glBindBuffer(GL_ARRAY_BUFFER, mVboTexcoords);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat),
                 texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  }

  // indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);

  mNumberOfVertices = (unsigned int)vertices.size() / 3;
  mNumberOfElementsToDraw = (unsigned int)indices.size();
}

void Mesh::render() const
{
  glBindVertexArray(mVao);
  glDrawRangeElements(GL_TRIANGLES, 0, mNumberOfVertices,
                      mNumberOfElementsToDraw, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

} // namespace evo