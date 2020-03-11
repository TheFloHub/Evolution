#include "Graphics3d/Assets/Materials/PBRMaterial.h"
#include <Graphics3d/Assets/Shader.h>
#include <Graphics3d/Assets/ShaderManager.h>
#pragma warning(disable : 4201)
#include <glm/gtc/type_ptr.hpp>

G3d::PBRMaterial::PBRMaterial()
    : Material(ShaderManager::getInstance().getPBRStandard()), mRed(1.0f),
      mGreen(1.0f), mBlue(1.0f), mMetallic(0.0f), mRoughness(0.8f),
      mMvpMatrixLoc(0), mNormalMatrixLoc(0), mAlbedoLoc(0), mMetallicLoc(0),
      mRoughnessLoc(0)
{
  mpShader->use();
  mMvpMatrixLoc = mpShader->getUniformLocation("modelViewProjectionMatrix");
  mNormalMatrixLoc = mpShader->getUniformLocation("normalMatrix");
  mAlbedoLoc = mpShader->getUniformLocation("albedo");
  mMetallicLoc = mpShader->getUniformLocation("metallic");
  mRoughnessLoc = mpShader->getUniformLocation("roughness");
}

G3d::PBRMaterial::~PBRMaterial() {}

void G3d::PBRMaterial::use() const
{
  Material::use(); // enables shader program
  glUniformMatrix4fv(
      mMvpMatrixLoc, 1, GL_FALSE,
      glm::value_ptr(Shader::getGlobalMat4("modelViewProjectionMatrix")));
  glUniformMatrix3fv(mNormalMatrixLoc, 1, GL_FALSE,
                     glm::value_ptr(Shader::getGlobalMat3("normalMatrix")));
  glUniform3f(mAlbedoLoc, mRed, mGreen, mBlue);
  glUniform1f(mMetallicLoc, mMetallic);
  glUniform1f(mRoughnessLoc, mRoughness);
}

void G3d::PBRMaterial::setAlbedo(float red, float green, float blue)
{
  mRed = red;
  mGreen = green;
  mBlue = blue;
}

void G3d::PBRMaterial::setMetallic(float metallic) { mMetallic = metallic; }

void G3d::PBRMaterial::setRoughness(float roughness) { mRoughness = roughness; }
