/**
 * @author Florian Feuerstein
 * @date 22.07.2016
 *
 * The mesh class contains the VBO's of all vertices, faces, vertex normals and
 * texcoords to render this mesh with OpenGL.
 */

#pragma once

#include <gl/glew.h>
#include <string>
#include <vector>

namespace G3d
{

class Mesh
{
public:
  Mesh(std::string const & objFileName);

  Mesh(std::vector<GLfloat> const & vertices,
       std::vector<GLfloat> const & normals,
       std::vector<GLfloat> const & texCoords,
       std::vector<GLuint> const & indices);

  ~Mesh();

  void render() const;

private:
  Mesh(Mesh const &) = delete;

  Mesh & operator=(Mesh const &) = delete;

  /** Generates the contained VBO's. */
  void generateVbos(std::vector<GLfloat> const & vertices,
                    std::vector<GLfloat> const & normals,
                    std::vector<GLfloat> const & texCoords,
                    std::vector<GLuint> const & indices);

  /** Create tangents and bitangents */
  void generateTangentSpace(std::vector<GLfloat> const & vertices,
                            std::vector<GLfloat> const & normals,
                            std::vector<GLfloat> const & texCoords,
                            std::vector<GLuint> const & indices);

  /** The vertex array object of this mesh. */
  GLuint mVao;

  /** The VBO of the vertices. */
  GLuint mVboVertices;

  /** The VBO of the vertex normals. */
  GLuint mVboNormals;

  /** The VBO of the vertex texcoords. */
  GLuint mVboTexcoords;

  /** The VBO of the tangents. */
  GLuint mVboTangents;

  /** The VBO of the bitangents. */
  GLuint mVboBitangents;

  /** The VBO of the index list. */
  GLuint mVboIndices;

  /** The number of 3d vertices. */
  unsigned int mNumberOfVertices;

  /** The number of elements to draw. */
  unsigned int mNumberOfElementsToDraw;
};

} // namespace G3d
