/**
 * @author Florian Feuerstein
 * @date 01.08.2016
 *
 *
 */

#pragma once

#include "Graphics3d/Assets/Material.h"
#include <gl/glew.h>

namespace g3d
{

class PBRTexturedMaterial : public Material
{
public:
  PBRTexturedMaterial();

  virtual ~PBRTexturedMaterial();

  void use() const override;

  void setAlbedo(TextureCPtr pTexture);

  void setMetallic(TextureCPtr pTexture);

  void setRoughness(TextureCPtr pTexture);

  void setNormal(TextureCPtr pTexture);

protected:
  TextureCPtr mpAlbedoTexture;

  TextureCPtr mpMetallicTexture;

  TextureCPtr mpRoughnessTexture;

  TextureCPtr mpNormalTexture;

  float mTilingX;

  float mTilingY;

  GLuint mMvpMatrixLoc;

  GLuint mNormalMatrixLoc;

  GLuint mTilingLoc;

private:
  PBRTexturedMaterial(PBRTexturedMaterial const &) = delete;

  PBRTexturedMaterial & operator=(PBRTexturedMaterial const &) = delete;
};

typedef std::shared_ptr<PBRTexturedMaterial> PBRTexturedMaterialPtr;
typedef std::shared_ptr<PBRTexturedMaterial const> PBRTexturedMaterialCPtr;
} // namespace g3d
