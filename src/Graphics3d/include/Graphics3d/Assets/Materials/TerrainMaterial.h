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

class TerrainMaterial : public Material
{
public:
  TerrainMaterial();

  virtual ~TerrainMaterial();

  void use() const override;

  void setHeightMap(TextureCPtr pHeightMap);

protected:
  TextureCPtr mpHeightMap;

  TexturePtr mpNoise;

  GLuint mMvpMatrixLoc;

  GLuint mNormalMatrixLoc;

private:
  TerrainMaterial(Material const &) = delete;

  TerrainMaterial & operator=(TerrainMaterial const &) = delete;
};

typedef std::shared_ptr<TerrainMaterial> TerrainMaterialPtr;
typedef std::shared_ptr<TerrainMaterial const> TerrainMaterialCPtr;
} // namespace G3d
