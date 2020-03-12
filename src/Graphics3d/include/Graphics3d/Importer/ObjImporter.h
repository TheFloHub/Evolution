/**
 * @author Florian Feuerstein
 * @date 22.07.2016
 *
 * The OBJImporter imports an obj file and returns
 * its vertices, texture coordinates, normals
 * and triangle indices. If the file doesn't define
 * any normals, they are automatically computed.
 */

#pragma once

#include <gl/glew.h>
#include <iostream>
#include <string>
#include <vector>

namespace g3d
{

class ObjImporter
{
public:
  explicit ObjImporter(std::string objFileName);

  ~ObjImporter();

  /** Returns true if the file has been successfully loaded. */
  bool isSuccessfullyLoaded() const;

  /** Returns all 3d vertices. */
  std::vector<GLfloat> const getVertices() const;

  /** Returns all 3d normals. */
  std::vector<GLfloat> const getNormals() const;

  /** Returns all 2d texture coordinates. */
  std::vector<GLfloat> const getTextureCoords() const;

  /** Returns the indices of all triangles. */
  std::vector<GLuint> const getIndices() const;

private:
  template <typename T> class Point2
  {
  public:
    Point2() : x(static_cast<T>(0)), y(static_cast<T>(0)) {}
    Point2(T xx, T yy) : x(xx), y(yy) {}
    ~Point2(){};

    Point2<T> operator+(Point2<T> const & other) const
    {
      return Point2<T>(x + other.x, y + other.y);
    }

    Point2<T> operator-(Point2<T> const & other) const
    {
      return Point2<T>(x - other.x, y - other.y);
    }

    T x;
    T y;
  };
  typedef Point2<int> Point2i;
  typedef Point2<float> Point2f;

  template <typename T> class Point3
  {
  public:
    Point3() : x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0))
    {
    }
    Point3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
    ~Point3(){};

    Point3<T> cross(Point3<T> const & other) const
    {
      return Point3<T>(y * other.z - z * other.y, z * other.x - x * other.z,
                       x * other.y - y * other.x);
    }

    T length() const { return sqrt(x * x + y * y + z * z); }

    void normalize()
    {
      T length = this->length();
      x /= length;
      y /= length;
      z /= length;
    }

    Point3<T> operator+(Point3<T> const & other) const
    {
      return Point3<T>(x + other.x, y + other.y, z + other.z);
    }

    Point3<T> & operator+=(Point3<T> const & other)
    {
      x += other.x;
      y += other.y;
      z += other.z;
      return *this;
    }

    Point3<T> operator-(Point3<T> const & other) const
    {
      return Point3<T>(x - other.x, y - other.y, z - other.z);
    }

    T x;
    T y;
    T z;
  };
  using Point3i = Point3<int>;
  using Point3f = Point3<float>;

  /** Reads the obj file. */
  void readModel(std::ifstream & objFile);

  /** Trims all spaces at the start and end of a string. */
  void trim(std::string & str);

  /** Reads 3 floats in point and returns true if it has succeeded. */
  bool read3Floats(const std::string & line, Point3f & point);

  /** Reads 2 floats in point and returns true if it has succeeded. */
  bool read2Floats(const std::string & line, Point2f & point);

  /** Adds a triangle to the importer's list. */
  bool addFace(std::string & line);

  /** Splits a string in tokens divided by the character c. */
  std::vector<std::string> getTokens(const std::string & line, const char c);

  /** Scales the model to size 1 and sets the pivot point to the bottom's
   * center. */
  void centerScale();

  /** Calculates missing normals of the model. */
  void calculateNormals();

  /** This flag indicates if the file can be read by the importer. */
  bool mFileOkay;

  /** The vertex list. */
  std::vector<Point3f> mVertexList;

  /** The normal list. */
  std::vector<Point3f> mNormalList;

  /** The texture coordinate list. */
  std::vector<Point2f> mTextureList;

  /** The normal list ordered by the vertices. */
  std::vector<Point3f> mNewNormalList;

  /** The texture coordinate list ordered by the vertices. */
  std::vector<Point2f> mNewTextureList;

  /** The triangle index list. */
  std::vector<Point3i> mIndices;

  /** This list is used for calculating missing normals. */
  std::vector<std::vector<Point3f>> mCalculatedNormals;
};

} // namespace g3d
