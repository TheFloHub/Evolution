#include "Graphics3d/Assets/Materials/SpecularMaterial.h"
#include <Graphics3d/Assets/Shader.h>
#include <Graphics3d/Assets/ShaderManager.h>
#include <Graphics3d/Assets/Texture.h>
#include <Graphics3d/Assets/TextureManager.h>
#pragma warning(disable : 4201)
#include <glm/gtc/type_ptr.hpp>

G3d::SpecularMaterial::SpecularMaterial()
    : Material(ShaderManager::getInstance().getSpecular()), mDiffuseRed(1.0f),
      mDiffuseGreen(1.0f), mDiffuseBlue(1.0f), mTilingX(1.0f), mTilingY(1.0f),
      mShininess(32.0f), mpTexture(TextureManager::getInstance().getRGBA255()),
      mMvpMatrixLoc(0), mNormalMatrixLoc(0), mDiffuseColorLoc(0), mTilingLoc(0),
      mShininessLoc(0)
{
  mpShader->use();
  glUniform1i(mpShader->getUniformLocation("diffuseTexture"), 0);
  mMvpMatrixLoc = mpShader->getUniformLocation("modelViewProjectionMatrix");
  mNormalMatrixLoc = mpShader->getUniformLocation("normalMatrix");
  mDiffuseColorLoc = mpShader->getUniformLocation("diffuseColor");
  mTilingLoc = mpShader->getUniformLocation("tiling");
  mShininessLoc = mpShader->getUniformLocation("shininess");
}

G3d::SpecularMaterial::~SpecularMaterial() {}

void G3d::SpecularMaterial::use() const
{
  Material::use(); // enables shader program
  glUniformMatrix4fv(
      mMvpMatrixLoc, 1, GL_FALSE,
      glm::value_ptr(Shader::getGlobalMat4("modelViewProjectionMatrix")));
  glUniformMatrix3fv(mNormalMatrixLoc, 1, GL_FALSE,
                     glm::value_ptr(Shader::getGlobalMat3("normalMatrix")));
  glUniform3f(mDiffuseColorLoc, mDiffuseRed, mDiffuseGreen, mDiffuseBlue);
  glUniform2f(mTilingLoc, mTilingX, mTilingY);
  glUniform1f(mShininessLoc, mShininess);
  glActiveTexture(GL_TEXTURE0);
  mpTexture->use();
}

void G3d::SpecularMaterial::setDiffuseColor(float red, float green, float blue)
{
  mDiffuseRed = red;
  mDiffuseGreen = green;
  mDiffuseBlue = blue;
}

void G3d::SpecularMaterial::setShininess(float shininess)
{
  mShininess = shininess;
}

void G3d::SpecularMaterial::setTexture(TextureCPtr pTexture)
{
  mpTexture = pTexture;
}

void G3d::SpecularMaterial::setTiling(float x, float y)
{
  mTilingX = x;
  mTilingY = y;
}