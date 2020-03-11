#include "Graphics3d/Assets/Materials/DiffuseMaterial.h"
#include <Graphics3d/Assets/Shader.h>
#include <Graphics3d/Assets/ShaderManager.h>
#include <Graphics3d/Assets/Texture.h>
#include <Graphics3d/Assets/TextureManager.h>
#pragma warning(disable : 4201)
#include <glm/gtc/type_ptr.hpp>

G3d::DiffuseMaterial::DiffuseMaterial()
    : Material(ShaderManager::getInstance().getDiffuse()), mRed(1.0f),
      mGreen(1.0f), mBlue(1.0f), mTilingX(1.0f), mTilingY(1.0f),
      mpTexture(TextureManager::getInstance().getRGB255()), mMvpMatrixLoc(0),
      mNormalMatrixLoc(0), mDiffuseColorLoc(0), mTilingLoc(0)
{
  mpShader->use();
  glUniform1i(mpShader->getUniformLocation("diffuseTexture"), 0);
  mMvpMatrixLoc = mpShader->getUniformLocation("modelViewProjectionMatrix");
  mNormalMatrixLoc = mpShader->getUniformLocation("normalMatrix");
  mDiffuseColorLoc = mpShader->getUniformLocation("diffuseColor");
  mTilingLoc = mpShader->getUniformLocation("tiling");
}

G3d::DiffuseMaterial::~DiffuseMaterial() {}

void G3d::DiffuseMaterial::use() const
{
  Material::use(); // enables shader program
  glUniformMatrix4fv(
      mMvpMatrixLoc, 1, GL_FALSE,
      glm::value_ptr(Shader::getGlobalMat4("modelViewProjectionMatrix")));
  glUniformMatrix3fv(mNormalMatrixLoc, 1, GL_FALSE,
                     glm::value_ptr(Shader::getGlobalMat3("normalMatrix")));
  glUniform3f(mDiffuseColorLoc, mRed, mGreen, mBlue);
  glUniform2f(mTilingLoc, mTilingX, mTilingY);
  glActiveTexture(GL_TEXTURE0);
  mpTexture->use();
}

void G3d::DiffuseMaterial::setDiffuseColor(float red, float green, float blue)
{
  mRed = red;
  mGreen = green;
  mBlue = blue;
}

void G3d::DiffuseMaterial::setTexture(TextureCPtr pTexture)
{
  mpTexture = pTexture;
}

void G3d::DiffuseMaterial::setTiling(float x, float y)
{
  mTilingX = x;
  mTilingY = y;
}