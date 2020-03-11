/**
 * @author Florian Feuerstein
 * @date 01.08.2016
 *
 *
 */

#pragma once

#include "Graphics3d/Assets/Material.h"
#include <gl/glew.h>

namespace G3d
{

class TerrainDepthMaterial : public Material
{
public:
  TerrainDepthMaterial();

  virtual ~TerrainDepthMaterial();

  void use() const override;

  void setHeightMap(TextureCPtr pHeightMap);

protected:
  TextureCPtr mpHeightMap;

  GLuint mMvpMatrixLoc;

private:
  TerrainDepthMaterial(Material const &) = delete;

  TerrainDepthMaterial & operator=(TerrainDepthMaterial const &) = delete;
};

typedef std::shared_ptr<TerrainDepthMaterial> TerrainDepthMaterialPtr;
typedef std::shared_ptr<TerrainDepthMaterial const> TerrainDepthMaterialCPtr;
} // namespace G3d
