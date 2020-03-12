#include "Graphics3d/Assets/Materials/PBRTexturedMaterial.h"
#include <Graphics3d/Assets/Shader.h>
#include <Graphics3d/Assets/ShaderManager.h>
#include <Graphics3d/Assets/Texture.h>
#include <Graphics3d/Assets/TextureManager.h>
#pragma warning(disable : 4201)
#include <glm/gtc/type_ptr.hpp>

g3d::PBRTexturedMaterial::PBRTexturedMaterial()
    : Material(ShaderManager::getInstance().getPBRTextured()),
      mpAlbedoTexture(TextureManager::getInstance().getRGB255()),
      mpMetallicTexture(TextureManager::getInstance().getL0()),
      mpRoughnessTexture(TextureManager::getInstance().getL255()),
      mpNormalTexture(TextureManager::getInstance().getDefaultNormalMap()),
      mTilingX(1.0f), mTilingY(1.0f), mMvpMatrixLoc(0), mNormalMatrixLoc(0),
      mTilingLoc(0)
{
  mpShader->use();
  mMvpMatrixLoc = mpShader->getUniformLocation("modelViewProjectionMatrix");
  mNormalMatrixLoc = mpShader->getUniformLocation("normalMatrix");
  glUniform1i(mpShader->getUniformLocation("albedoTexture"), 0);
  glUniform1i(mpShader->getUniformLocation("metallicTexture"), 1);
  glUniform1i(mpShader->getUniformLocation("roughnessTexture"), 2);
  glUniform1i(mpShader->getUniformLocation("normalTexture"), 3);
  mTilingLoc = mpShader->getUniformLocation("tiling");
}

g3d::PBRTexturedMaterial::~PBRTexturedMaterial() {}

void g3d::PBRTexturedMaterial::use() const
{
  Material::use(); // enables shader program
  glUniformMatrix4fv(
      mMvpMatrixLoc, 1, GL_FALSE,
      glm::value_ptr(Shader::getGlobalMat4("modelViewProjectionMatrix")));
  glUniformMatrix3fv(mNormalMatrixLoc, 1, GL_FALSE,
                     glm::value_ptr(Shader::getGlobalMat3("normalMatrix")));
  glUniform2f(mTilingLoc, mTilingX, mTilingY);
  glActiveTexture(GL_TEXTURE0);
  mpAlbedoTexture->use();
  glActiveTexture(GL_TEXTURE1);
  mpMetallicTexture->use();
  glActiveTexture(GL_TEXTURE2);
  mpRoughnessTexture->use();
  glActiveTexture(GL_TEXTURE3);
  mpNormalTexture->use();
}

void g3d::PBRTexturedMaterial::setAlbedo(TextureCPtr pTexture)
{
  mpAlbedoTexture = pTexture;
}

void g3d::PBRTexturedMaterial::setMetallic(TextureCPtr pTexture)
{
  mpMetallicTexture = pTexture;
}

void g3d::PBRTexturedMaterial::setRoughness(TextureCPtr pTexture)
{
  mpRoughnessTexture = pTexture;
}

void g3d::PBRTexturedMaterial::setNormal(TextureCPtr pTexture)
{
  mpNormalTexture = pTexture;
}
