#include "Graphics3d/Assets/Materials/TerrainMaterial.h"
#include <Graphics3d/Assets/Shader.h>
#include <Graphics3d/Assets/ShaderManager.h>
#include <Graphics3d/Assets/Texture.h>
#include <Graphics3d/Assets/TextureManager.h>
#pragma warning(disable : 4201)
#include <glm/gtc/type_ptr.hpp>

g3d::TerrainMaterial::TerrainMaterial()
    : Material(ShaderManager::getInstance().getTerrain()),
      mpHeightMap(
          TextureManager::getInstance().getRGB255()), // TODO: Luminence0 ?
      mpNoise(TextureManager::getInstance().getTerrainNoise()),
      mMvpMatrixLoc(0), mNormalMatrixLoc(0)
{
  mpNoise->lowQuality();
  mpShader->use();
  GLuint program = mpShader->getHandle();
  glUniform1i(glGetUniformLocation(program, "heightMap"), 0);
  glUniform1i(glGetUniformLocation(program, "noiseSampler"), 1);
  mMvpMatrixLoc = glGetUniformLocation(program, "modelViewProjectionMatrix");
  mNormalMatrixLoc = glGetUniformLocation(program, "normalMatrix");
}

g3d::TerrainMaterial::~TerrainMaterial() {}

void g3d::TerrainMaterial::use() const
{
  Material::use(); // enables shader program
  glUniformMatrix4fv(
      mMvpMatrixLoc, 1, GL_FALSE,
      glm::value_ptr(Shader::getGlobalMat4("modelViewProjectionMatrix")));
  glUniformMatrix3fv(mNormalMatrixLoc, 1, GL_FALSE,
                     glm::value_ptr(Shader::getGlobalMat3("normalMatrix")));
  glActiveTexture(GL_TEXTURE0);
  mpHeightMap->use();
  glActiveTexture(GL_TEXTURE1);
  mpNoise->use();
}

void g3d::TerrainMaterial::setHeightMap(TextureCPtr pHeightMap)
{
  mpHeightMap = pHeightMap;
}
