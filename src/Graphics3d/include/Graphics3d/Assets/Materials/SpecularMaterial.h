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

class SpecularMaterial : public Material
{
public:
  SpecularMaterial();

  virtual ~SpecularMaterial();

  void use() const override;

  void setDiffuseColor(float red, float green, float blue);

  void setTiling(float x, float y);

  void setShininess(float shininess);

  void setTexture(TextureCPtr pTexture);

protected:
  float mDiffuseRed;

  float mDiffuseGreen;

  float mDiffuseBlue;

  float mTilingX;

  float mTilingY;

  float mShininess;

  TextureCPtr mpTexture;

  GLuint mMvpMatrixLoc;

  GLuint mNormalMatrixLoc;

  GLuint mDiffuseColorLoc;

  GLuint mTilingLoc;

  GLuint mShininessLoc;

private:
  SpecularMaterial(Material const &) = delete;

  SpecularMaterial & operator=(SpecularMaterial const &) = delete;
};

typedef std::shared_ptr<SpecularMaterial> SpecularMaterialPtr;
typedef std::shared_ptr<SpecularMaterial const> SpecularMaterialCPtr;
} // namespace g3d
