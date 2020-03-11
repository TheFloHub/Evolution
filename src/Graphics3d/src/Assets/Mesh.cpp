#include "Graphics3d/Assets/Mesh.h"
#include "Graphics3d/Importer/ObjImporter.h"
#include <gl/glew.h>
#include <glm/glm.hpp>

G3d::Mesh::Mesh(std::string const & objFileName)
    : mVao(0), mVboVertices(0), mVboNormals(0), mVboTexcoords(0),
      mVboTangents(0), mVboBitangents(0), mVboIndices(0), mNumberOfVertices(0),
      mNumberOfElementsToDraw(0)
{
  ObjImporter objImporter(objFileName);
  if (objImporter.isSuccessfullyLoaded())
  {
    generateVbos(objImporter.getVertices(), objImporter.getNormals(),
                 objImporter.getTextureCoords(), objImporter.getIndices());
  }
}

G3d::Mesh::Mesh(std::vector<GLfloat> const & vertices,
                std::vector<GLfloat> const & normals,
                std::vector<GLfloat> const & texCoords,
                std::vector<GLuint> const & indices)
    : mVao(0), mVboVertices(0), mVboNormals(0), mVboTexcoords(0),
      mVboTangents(0), mVboBitangents(0), mVboIndices(0), mNumberOfVertices(0),
      mNumberOfElementsToDraw(0)
{
  generateVbos(vertices, normals, texCoords, indices);
}

G3d::Mesh::~Mesh()
{
  if (mVboVertices != 0)
    glDeleteBuffers(1, &mVboVertices);
  if (mVboNormals != 0)
    glDeleteBuffers(1, &mVboNormals);
  if (mVboTexcoords != 0)
    glDeleteBuffers(1, &mVboTexcoords);
  if (mVboTangents != 0)
    glDeleteBuffers(1, &mVboTangents);
  if (mVboBitangents != 0)
    glDeleteBuffers(1, &mVboBitangents);
  if (mVboIndices != 0)
    glDeleteBuffers(1, &mVboIndices);
  if (mVao != 0)
    glDeleteVertexArrays(1, &mVao);
}

void G3d::Mesh::generateVbos(std::vector<GLfloat> const & vertices,
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
    glGenBuffers(1, &mVboTangents);
    glGenBuffers(1, &mVboBitangents);
  }
  glGenBuffers(1, &mVboIndices);

  glBindVertexArray(mVao);

  // vertices
  glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
               vertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0),
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // normals
  glBindBuffer(GL_ARRAY_BUFFER, mVboNormals);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat),
               normals.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(1),
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // tex coords
  if (hasTexCoords)
  {
    glBindBuffer(GL_ARRAY_BUFFER, mVboTexcoords);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat),
                 texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2),
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    generateTangentSpace(vertices, normals, texCoords, indices);
  }

  // indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);

  mNumberOfVertices = (unsigned int)vertices.size() / 3;
  mNumberOfElementsToDraw = (unsigned int)indices.size();
}

void G3d::Mesh::generateTangentSpace(std::vector<GLfloat> const & vertices,
                                     std::vector<GLfloat> const & normals,
                                     std::vector<GLfloat> const & texCoords,
                                     std::vector<GLuint> const & indices)
{
  std::vector<GLfloat> tangents(vertices.size(), 0.0f);
  std::vector<GLfloat> bitangents(vertices.size(), 0.0f);

  for (size_t faceVertexIndex = 0; faceVertexIndex < indices.size();
       faceVertexIndex += 3)
  {
    GLuint vi1 = 3 * indices[faceVertexIndex];
    GLuint vi2 = 3 * indices[faceVertexIndex + 1];
    GLuint vi3 = 3 * indices[faceVertexIndex + 2];

    GLuint ti1 = 2 * indices[faceVertexIndex];
    GLuint ti2 = 2 * indices[faceVertexIndex + 1];
    GLuint ti3 = 2 * indices[faceVertexIndex + 2];

    glm::vec3 p1(vertices[vi1], vertices[vi1 + 1], vertices[vi1 + 2]);
    glm::vec3 p2(vertices[vi2], vertices[vi2 + 1], vertices[vi2 + 2]);
    glm::vec3 p3(vertices[vi3], vertices[vi3 + 1], vertices[vi3 + 2]);

    glm::vec2 uv1(texCoords[ti1], texCoords[ti1 + 1]);
    glm::vec2 uv2(texCoords[ti2], texCoords[ti2 + 1]);
    glm::vec2 uv3(texCoords[ti3], texCoords[ti3 + 1]);

    glm::vec3 edge1 = p2 - p1;
    glm::vec3 edge2 = p3 - p1;
    glm::vec2 edge1uv = uv2 - uv1;
    glm::vec2 edge2uv = uv3 - uv1;

    // float r = 1.0f / (edge1uv.y * edge2uv.x - edge1uv.x * edge2uv.y);
    // glm::vec3 tangent =   (edge1 * -edge2uv.y + edge2 * edge1uv.y) * r;
    // glm::vec3 bitangent = (edge1 * -edge2uv.x + edge2 * edge1uv.x) * r;

    float r = 1.0f / (edge1uv.x * edge2uv.y - edge1uv.y * edge2uv.x);
    glm::vec3 tangent = (edge1 * edge2uv.y - edge2 * edge1uv.y) * r;
    glm::vec3 bitangent = (edge2 * edge1uv.x - edge1 * edge2uv.x) * r;

    tangents[vi1] += tangent.x;
    tangents[vi1 + 1] += tangent.y;
    tangents[vi1 + 2] += tangent.z;
    bitangents[vi1] += bitangent.x;
    bitangents[vi1 + 1] += bitangent.y;
    bitangents[vi1 + 2] += bitangent.z;

    tangents[vi2] += tangent.x;
    tangents[vi2 + 1] += tangent.y;
    tangents[vi2 + 2] += tangent.z;
    bitangents[vi2] += bitangent.x;
    bitangents[vi2 + 1] += bitangent.y;
    bitangents[vi2 + 2] += bitangent.z;

    tangents[vi3] += tangent.x;
    tangents[vi3 + 1] += tangent.y;
    tangents[vi3 + 2] += tangent.z;
    bitangents[vi3] += bitangent.x;
    bitangents[vi3 + 1] += bitangent.y;
    bitangents[vi3 + 2] += bitangent.z;
  }

  // normalize
  for (size_t i = 0; i < tangents.size(); i += 3)
  {
    float length =
        sqrt(tangents[i] * tangents[i] + tangents[i + 1] * tangents[i + 1] +
             tangents[i + 2] * tangents[i + 2]);
    tangents[i] /= length;
    tangents[i + 1] /= length;
    tangents[i + 2] /= length;

    length = sqrt(bitangents[i] * bitangents[i] +
                  bitangents[i + 1] * bitangents[i + 1] +
                  bitangents[i + 2] * bitangents[i + 2]);
    bitangents[i] /= length;
    bitangents[i + 1] /= length;
    bitangents[i + 2] /= length;

    // re-orthogonalize
    glm::vec3 normal(normals[i], normals[i + 1], normals[i + 2]);
    glm::vec3 tangent(tangents[i], tangents[i + 1], tangents[i + 2]);
    tangent = glm::normalize(tangent - glm::dot(tangent, normal) * normal);
    glm::vec3 bitangent = glm::cross(normal, tangent);

    tangents[i] = tangent.x;
    tangents[i + 1] = tangent.y;
    tangents[i + 2] = tangent.z;

    bitangents[i] = bitangent.x;
    bitangents[i + 1] = bitangent.y;
    bitangents[i + 2] = bitangent.z;
  }

  // tangents
  glBindBuffer(GL_ARRAY_BUFFER, mVboTangents);
  glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(GLfloat),
               tangents.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(3),
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // bitangents
  glBindBuffer(GL_ARRAY_BUFFER, mVboBitangents);
  glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(GLfloat),
               bitangents.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(4),
      glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void G3d::Mesh::render() const
{
  glBindVertexArray(mVao);
  glDrawRangeElements(GL_TRIANGLES, 0, mNumberOfVertices,
                      mNumberOfElementsToDraw, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}