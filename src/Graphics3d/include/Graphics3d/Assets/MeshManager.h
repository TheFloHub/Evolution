/**
 * @author Florian Feuerstein
 * @date 03.08.2016
 *
 * The mesh manager contains all loaded meshes and provides access to them.
 */

#pragma once

#include <Graphics3d/SharedPtrTypes.h>
#include <map>
#include <string>

namespace G3d
{

class MeshManager
{
public:
  static MeshManager & getInstance();

  ~MeshManager();

  MeshPtr load(std::string const & meshName, std::string const & fileName);

  MeshPtr get(std::string const & meshName) const;

  MeshPtr getPlane() const;

  MeshPtr getCircle() const;

  MeshPtr getTriangle() const;

  MeshPtr getScreenAlignedQuad() const;

  MeshPtr getGrid() const;

  void initStandardMeshes();

private:
  MeshManager();

  MeshManager(MeshManager const &) = delete;

  MeshManager & operator=(MeshManager const &) = delete;

  void createPlane();
  void createCircle();
  void createTriangle();
  void createScreenAlignedQuad();
  void createGrid();

  std::map<std::string, MeshPtr> mMeshMap;
};

} // namespace G3d
