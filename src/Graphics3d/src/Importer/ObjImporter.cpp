#include "Graphics3d/Importer/ObjImporter.h"
#include <sstream>
#include <fstream>

using namespace std;

g3d::ObjImporter::ObjImporter(string objFileName)
    : mFileOkay(false), mVertexList(), mNormalList(), mTextureList(),
      mNewNormalList(), mNewTextureList(), mIndices(), mCalculatedNormals()
{
  ifstream objFile(objFileName.data());
  if (objFile.is_open())
  {
    mFileOkay = true;
    readModel(objFile);
    objFile.close();
    if (mFileOkay)
    {
      centerScale();
      if (mNewNormalList.empty())
        calculateNormals();
    }
    else
    {
      cout << "Reading obj file failed." << endl;
    }
  }
  else
  {
    cout << "Unable to open file: " << objFileName << endl;
  }
}

void g3d::ObjImporter::readModel(ifstream & objFile)
{
  string line;

  while (objFile.good() && mFileOkay)
  {
    getline(objFile, line);
    if (line.length() > 0)
    {
      trim(line);
      string mode = line.substr(0, 2);

      if (mode.compare("v ") == 0)
      { // vertex
        Point3f v;
        mFileOkay = read3Floats(line, v);
        mVertexList.push_back(v);
      }
      else if (mode.compare("vt") == 0)
      { // tex coord
        Point2f vt;
        mFileOkay = read2Floats(line, vt);
        mTextureList.push_back(vt);
      }
      else if (mode.compare("vn") == 0)
      { // normal
        Point3f n;
        mFileOkay = read3Floats(line, n);
        mNormalList.push_back(n);
      }
      else if (mode.compare("f ") == 0)
      { // face
        mFileOkay = addFace(line);
      }
      else if (line[0] == '#')
      { // comment line
        continue;
      }
    }
  }
}

g3d::ObjImporter::~ObjImporter() {}

bool g3d::ObjImporter::addFace(string & line)
{
  // Get the three face tokens ignoring the first modus f token
  vector<string> faceTokens = getTokens(line, ' ');

  if (faceTokens.size() != 4)
    return false;

  int vertexIndices[3];
  stringstream sstream;
  unsigned int vertexIndex, faceTokenIndex;
  int vi, ti, ni;
  for (faceTokenIndex = 1, vertexIndex = 0; faceTokenIndex < 4;
       ++faceTokenIndex, ++vertexIndex)
  {
    vector<string> infoTokens = getTokens(faceTokens[faceTokenIndex], '/');
    size_t numberOfInfo = infoTokens.size();

    // Sort the normals and texture coords by the index of the vertex
    if (numberOfInfo == 1)
    {
      sstream << infoTokens[0];
      sstream >> vi;
      sstream.clear();
      vi = vi - 1;
      vertexIndices[vertexIndex] = vi;
    }
    else if (numberOfInfo == 2)
    {
      sstream << infoTokens[0];
      sstream >> vi;
      sstream.clear();
      vi = vi - 1;
      vertexIndices[vertexIndex] = vi;

      sstream << infoTokens[1];
      sstream >> ni;
      sstream.clear();
      ni = ni - 1;
      if (mNormalList.size() > ni)
      {
        if (mNewNormalList.size() < vi + 1)
          mNewNormalList.resize(vi + 1);
        mNewNormalList[vi] = mNormalList[ni];
      }
    }
    else if (numberOfInfo == 3)
    {
      sstream << infoTokens[0];
      sstream >> vi;
      sstream.clear();
      vi = vi - 1;
      vertexIndices[vertexIndex] = vi;

      sstream << infoTokens[1];
      sstream >> ti;
      sstream.clear();
      ti = ti - 1;
      if (mTextureList.size() > ti)
      {
        if (mNewTextureList.size() < vi + 1)
          mNewTextureList.resize(vi + 1);
        mNewTextureList[vi] = mTextureList[ti];
      }

      sstream << infoTokens[1];
      sstream >> ni;
      sstream.clear();
      ni = ni - 1;
      if (mNormalList.size() > ni)
      {
        if (mNewNormalList.size() < vi + 1)
          mNewNormalList.resize(vi + 1);
        mNewNormalList[vi] = mNormalList[ni];
      }
    }
    else
    {
      return false;
    }
  }

  Point3i p;
  p.x = vertexIndices[0];
  p.y = vertexIndices[1];
  p.z = vertexIndices[2];
  mIndices.push_back(p);
  return true;
}

bool g3d::ObjImporter::read3Floats(const string & line, Point3f & p)
{
  vector<string> strings = getTokens(line, ' ');
  if (strings.size() != 4)
    return false;

  stringstream sstream;

  sstream << strings[1];
  sstream >> p.x;
  sstream.clear();

  sstream << strings[2];
  sstream >> p.y;
  sstream.clear();

  sstream << strings[3];
  sstream >> p.z;
  sstream.clear();

  return true;
}

bool g3d::ObjImporter::read2Floats(const string & line, Point2f & p)
{
  vector<string> strings = getTokens(line, ' ');
  if (strings.size() != 3)
    return false;

  stringstream sstream;

  sstream << strings[1];
  sstream >> p.x;
  sstream.clear();

  sstream << strings[2];
  sstream >> p.y;
  sstream.clear();

  return true;
}

vector<string> g3d::ObjImporter::getTokens(const string & line, const char c)
{
  vector<string> tokens;
  string s;
  string::size_type pos1 = 0;
  string::size_type pos2 = 0;

  while (pos2 != string::npos)
  {
    pos2 = line.find_first_of(c, pos1);
    s = line.substr(pos1, pos2 - pos1);
    if (!s.empty())
      tokens.push_back(s);
    pos1 = pos2 + 1;
  }

  return tokens;
}

void g3d::ObjImporter::trim(string & str)
{
  string::size_type pos1 = str.find_first_not_of(' ');
  string::size_type pos2 = str.find_last_not_of(' ');
  str = str.substr(pos1 == string::npos ? 0 : pos1,
                   pos2 == string::npos ? str.length() - 1 : pos2 - pos1 + 1);
}

void g3d::ObjImporter::centerScale()
{
  Point3f p = mVertexList[0];
  float leftPt = p.x;
  float rightPt = p.x;
  float topPt = p.y;
  float bottomPt = p.y;
  float farPt = p.z;
  float nearPt = p.z;

  for (vector<Point3f>::iterator it = mVertexList.begin();
       it != mVertexList.end(); ++it)
  {
    if (it->x > rightPt)
      rightPt = it->x;
    if (it->x < leftPt)
      leftPt = it->x;

    if (it->y > topPt)
      topPt = it->y;
    if (it->y < bottomPt)
      bottomPt = it->y;

    if (it->z > farPt)
      farPt = it->z;
    if (it->z < nearPt)
      nearPt = it->z;
  }

  float width = rightPt - leftPt;
  float height = topPt - bottomPt;
  float depth = farPt - nearPt;
  float largest = width;
  if (height > largest)
    largest = height;
  if (depth > largest)
    largest = depth;

  Point3f center((rightPt + leftPt) / 2.0f, (topPt + bottomPt) / 2.0f,
                 (nearPt + farPt) / 2.0f);
  float scaleFactor = 1.0f / largest;

  for (vector<Point3f>::iterator it = mVertexList.begin();
       it != mVertexList.end(); ++it)
  {
    it->x = (it->x - center.x) * scaleFactor;
    it->y = (it->y - center.y) * scaleFactor;
    it->z = (it->z - center.z) * scaleFactor;
  }
}

void g3d::ObjImporter::calculateNormals()
{
  mNewNormalList.clear();

  mCalculatedNormals.resize(mVertexList.size());

  Point3f a, b, c, d1, d2, n;
  for (vector<Point3i>::iterator it = mIndices.begin(); it != mIndices.end();
       ++it)
  {
    a = mVertexList[it->x];
    b = mVertexList[it->y];
    c = mVertexList[it->z];
    d1 = a - b;
    d2 = a - c;
    n = d1.cross(d2);
    n.normalize();
    mCalculatedNormals[it->x].push_back(n);
    mCalculatedNormals[it->y].push_back(n);
    mCalculatedNormals[it->z].push_back(n);
  }

  for (vector<vector<Point3f>>::iterator it1 = mCalculatedNormals.begin();
       it1 != mCalculatedNormals.end(); ++it1)
  {
    Point3f normal(0, 0, 0);
    // int size = it1->size();
    for (vector<Point3f>::iterator it2 = it1->begin(); it2 != it1->end(); ++it2)
    {
      normal += (*it2);
    }
    // normal *= (1.0f/(float)size);
    normal.normalize();
    mNewNormalList.push_back(normal);
  }
}

bool g3d::ObjImporter::isSuccessfullyLoaded() const { return mFileOkay; }

std::vector<GLfloat> const g3d::ObjImporter::getVertices() const
{
  std::vector<GLfloat> vertices;
  vertices.reserve(3 * mVertexList.size());
  for (auto iter = mVertexList.cbegin(); iter < mVertexList.cend(); ++iter)
  {
    vertices.push_back(iter->x);
    vertices.push_back(iter->y);
    vertices.push_back(iter->z);
  }
  return vertices;
}

std::vector<GLfloat> const g3d::ObjImporter::getTextureCoords() const
{
  std::vector<GLfloat> texCoords;
  texCoords.reserve(2 * mNewTextureList.size());
  for (auto iter = mNewTextureList.cbegin(); iter < mNewTextureList.cend();
       ++iter)
  {
    texCoords.push_back(iter->x);
    texCoords.push_back(iter->y);
  }
  return texCoords;
}

std::vector<GLfloat> const g3d::ObjImporter::getNormals() const
{
  std::vector<GLfloat> normals;
  normals.reserve(3 * mNewNormalList.size());
  for (auto iter = mNewNormalList.cbegin(); iter < mNewNormalList.cend();
       ++iter)
  {
    normals.push_back(iter->x);
    normals.push_back(iter->y);
    normals.push_back(iter->z);
  }
  return normals;
}

std::vector<GLuint> const g3d::ObjImporter::getIndices() const
{
  std::vector<GLuint> indices;
  indices.reserve(3 * mIndices.size());
  for (auto iter = mIndices.cbegin(); iter < mIndices.cend(); ++iter)
  {
    indices.push_back(iter->x);
    indices.push_back(iter->y);
    indices.push_back(iter->z);
  }
  return indices;
}
