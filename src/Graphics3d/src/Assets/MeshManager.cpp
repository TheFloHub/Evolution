#include "Graphics3d/Assets/MeshManager.h"
#include "Graphics3d/Assets/Mesh.h"
#include <iostream>

g3d::MeshManager & g3d::MeshManager::getInstance()
{
  static MeshManager instance;
  return instance;
}

g3d::MeshManager::MeshManager() : mMeshMap() {}

g3d::MeshManager::~MeshManager() {}

g3d::MeshPtr g3d::MeshManager::load(std::string const & meshName,
                                    std::string const & fileName)
{
  MeshPtr pMesh = get(meshName);
  if (pMesh)
  {
    std::cout << "Mesh with name " << meshName << " is already existing."
              << std::endl;
    return pMesh;
  }
  return (mMeshMap.insert(
              std::make_pair(meshName, MeshPtr(new Mesh(fileName)))))
      .first->second;
}

g3d::MeshPtr g3d::MeshManager::get(std::string const & meshName) const
{
  std::map<std::string, MeshPtr>::const_iterator iter = mMeshMap.find(meshName);
  if (iter != mMeshMap.end())
  {
    return iter->second;
  }
  return MeshPtr();
}

g3d::MeshPtr g3d::MeshManager::getPlane() const { return get("StdPlane"); }

g3d::MeshPtr g3d::MeshManager::getCircle() const { return get("StdCircle"); }

g3d::MeshPtr g3d::MeshManager::getTriangle() const
{
  return get("StdTriangle");
}

g3d::MeshPtr g3d::MeshManager::getGrid() const { return get("StdGrid"); }

g3d::MeshPtr g3d::MeshManager::getScreenAlignedQuad() const
{
  return get("StdScreenAlignedQuad");
}

void g3d::MeshManager::initStandardMeshes()
{
  createPlane();
  createCircle();
  createTriangle();
  createScreenAlignedQuad();
  createGrid();
}

void g3d::MeshManager::createPlane()
{
  GLfloat v[] = {-0.5f, -0.5f, 0.0f, 0.5f,  -0.5f, 0.0f,
                 0.5f,  0.5f,  0.0f, -0.5f, 0.5f,  0.0f};

  GLfloat n[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

  GLfloat t[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

  GLuint i[] = {0, 1, 2, 0, 2, 3};

  std::vector<GLfloat> vertices(v, v + sizeof(v) / sizeof(GLfloat));
  std::vector<GLfloat> normals(n, n + sizeof(n) / sizeof(GLfloat));
  std::vector<GLfloat> texCoords(t, t + sizeof(t) / sizeof(GLfloat));
  std::vector<GLuint> indices(i, i + sizeof(i) / sizeof(GLuint));

  mMeshMap.insert(std::make_pair(
      "StdPlane", MeshPtr(new Mesh(vertices, normals, texCoords, indices))));
}

void g3d::MeshManager::createCircle()
{
  unsigned int numVerts = 36;
  std::vector<GLfloat> v;
  v.reserve(3 * numVerts + 1);
  std::vector<GLfloat> n;
  n.reserve(3 * numVerts + 1);
  std::vector<GLfloat> t;
  t.reserve(2 * numVerts + 1);
  std::vector<GLuint> i;
  i.reserve(3 * numVerts);

  float deltaAngle = 2.0f * 3.14159265359f / numVerts;
  float sinPhi = 0.0;
  float cosPhi = 0.0;
  float phi = 0.0;
  for (unsigned int j = 0; j < numVerts; ++j, phi += deltaAngle)
  {
    cosPhi = cos(phi);
    sinPhi = sin(phi);

    v.push_back(0.5f * cosPhi);
    v.push_back(0.0f);
    v.push_back(0.5f * sinPhi);

    n.push_back(0.0f);
    n.push_back(1.0f);
    n.push_back(0.0f);

    t.push_back(0.5f - 0.5f * cosPhi);
    t.push_back(0.5f + 0.5f * sinPhi);

    i.push_back(j);
    i.push_back(numVerts);
    i.push_back((j + 1) % numVerts);
  }

  // center point
  v.push_back(0.0);
  v.push_back(0.0);
  v.push_back(0.0);
  n.push_back(0.0);
  n.push_back(1.0);
  n.push_back(0.0);
  t.push_back(0.5);
  t.push_back(0.5);

  mMeshMap.insert(std::make_pair("StdCircle", MeshPtr(new Mesh(v, n, t, i))));
}

void g3d::MeshManager::createTriangle()
{
  GLfloat v[] = {0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.5f};

  GLfloat n[] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

  GLfloat t[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f};

  GLuint i[] = {0, 1, 2};

  std::vector<GLfloat> vertices(v, v + sizeof(v) / sizeof(GLfloat));
  std::vector<GLfloat> normals(n, n + sizeof(n) / sizeof(GLfloat));
  std::vector<GLfloat> texCoords(t, t + sizeof(t) / sizeof(GLfloat));
  std::vector<GLuint> indices(i, i + sizeof(i) / sizeof(GLuint));

  mMeshMap.insert(std::make_pair(
      "StdTriangle", MeshPtr(new Mesh(vertices, normals, texCoords, indices))));
}

void g3d::MeshManager::createScreenAlignedQuad()
{
  GLfloat v[] = {-1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f,  -1.0f, 0.0f};

  GLfloat n[] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

  GLfloat t[] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};

  GLuint i[] = {0, 1, 2, 2, 1, 3};

  std::vector<GLfloat> vertices(v, v + sizeof(v) / sizeof(GLfloat));
  std::vector<GLfloat> normals(n, n + sizeof(n) / sizeof(GLfloat));
  std::vector<GLfloat> texCoords(t, t + sizeof(t) / sizeof(GLfloat));
  std::vector<GLuint> indices(i, i + sizeof(i) / sizeof(GLuint));

  mMeshMap.insert(
      std::make_pair("StdScreenAlignedQuad",
                     MeshPtr(new Mesh(vertices, normals, texCoords, indices))));
}

/*
void g3d::MeshManager::CreateGrid()
{
        size_t const count = 512;
        size_t const numVerts = count*count;
        std::vector<GLfloat> v;
        v.reserve(3 * numVerts);
        std::vector<GLfloat> n;
        n.reserve(3 * numVerts);
        std::vector<GLfloat> t;
        t.reserve(2 * numVerts);
        std::vector<GLuint> i;
        i.reserve(3 * numVerts);

        GLfloat const texNormalizer = 1.0f/(GLfloat)count;
        for (GLfloat z = 0; z < count; ++z)
        {
                for (GLfloat x = 0; x < count; ++x)
                {
                        v.push_back(x);
                        v.push_back(0.0f);
                        v.push_back(z);

                        n.push_back(0.0f);
                        n.push_back(1.0f);
                        n.push_back(0.0f);

                        t.push_back((z+0.5f)*texNormalizer);
                        t.push_back((x+0.5f)*texNormalizer);
                }
        }

        GLuint rbi = 0;
        GLuint lbi = 0;
        GLuint rti = 0;
        GLuint lti = 0;
        for (GLuint z = 0; z < count-1; ++z)
        {
                for (GLuint x = 0; x < count-1; ++x)
                {
                        rbi = z*count + x;
                        lbi = rbi + 1;
                        rti = rbi + count;
                        lti = rti + 1;

                        i.push_back(rbi);
                        i.push_back(rti);
                        i.push_back(lbi);

                        i.push_back(lbi);
                        i.push_back(rti);
                        i.push_back(lti);
                }
        }

        mMeshMap.insert(std::make_pair("StdGrid", MeshPtr(new Mesh(v, n, t,
i))));
}
*/

void g3d::MeshManager::createGrid()
{
  size_t const numEdgeVertices = 512;
  size_t const numVerts = numEdgeVertices * numEdgeVertices;
  float const edgeLength = 1000.0f;

  float const temp1 = edgeLength / ((float)numEdgeVertices - 1.0f);
  float const temp2 = -edgeLength / 2.0f;

  std::vector<GLfloat> v;
  v.reserve(3 * numVerts);
  std::vector<GLfloat> n;
  n.reserve(3 * numVerts);
  std::vector<GLfloat> t;
  t.reserve(2 * numVerts);
  std::vector<GLuint> i;
  i.reserve(3 * numVerts);

  GLfloat const texNormalizer = 1.0f / (GLfloat)numEdgeVertices;
  for (GLfloat y = 0; y < numEdgeVertices; ++y)
  {
    for (GLfloat x = 0; x < numEdgeVertices; ++x)
    {
      v.push_back(x * temp1 + temp2);
      v.push_back(y * temp1 + temp2);
      v.push_back(0.0f);

      n.push_back(0.0f);
      n.push_back(0.0f);
      n.push_back(1.0f);

      t.push_back((x + 0.5f) * texNormalizer);
      t.push_back((y + 0.5f) * texNormalizer);
    }
  }

  GLuint rbi = 0;
  GLuint lbi = 0;
  GLuint rti = 0;
  GLuint lti = 0;
  for (GLuint y = 0; y < numEdgeVertices - 1; ++y)
  {
    for (GLuint x = 0; x < numEdgeVertices - 1; ++x)
    {
      lbi = y * numEdgeVertices + x;
      rbi = lbi + 1;
      lti = lbi + numEdgeVertices;
      rti = lti + 1;

      i.push_back(lbi);
      i.push_back(rbi);
      i.push_back(rti);

      i.push_back(lbi);
      i.push_back(rti);
      i.push_back(lti);
    }
  }

  mMeshMap.insert(std::make_pair("StdGrid", MeshPtr(new Mesh(v, n, t, i))));
}
