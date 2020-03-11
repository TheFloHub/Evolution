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

class PBRMaterial : public Material
{
public:
  PBRMaterial();

  virtual ~PBRMaterial();

  void use() const override;

  void setAlbedo(float red, float green, float blue);

  void setMetallic(float metallic);

  void setRoughness(float roughness);

protected:
  float mRed;

  float mGreen;

  float mBlue;

  float mMetallic;

  float mRoughness;

  GLuint mMvpMatrixLoc;

  GLuint mNormalMatrixLoc;

  GLuint mAlbedoLoc;

  GLuint mMetallicLoc;

  GLuint mRoughnessLoc;

private:
  PBRMaterial(PBRMaterial const &) = delete;

  PBRMaterial & operator=(PBRMaterial const &) = delete;
};

typedef std::shared_ptr<PBRMaterial> PBRMaterialPtr;
typedef std::shared_ptr<PBRMaterial const> PBRMaterialCPtr;
} // namespace G3d
