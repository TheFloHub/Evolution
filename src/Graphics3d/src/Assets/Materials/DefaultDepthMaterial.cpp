#include "Graphics3d/Assets/Materials/DefaultDepthMaterial.h"
#include <Graphics3d/Assets/Shader.h>
#include <Graphics3d/Assets/ShaderManager.h>
#pragma warning(disable : 4201)
#include <glm/gtc/type_ptr.hpp>

g3d::DefaultDepthMaterial::DefaultDepthMaterial()
    : Material(ShaderManager::getInstance().getDefaultDepth()), mMvpMatrixLoc(0)
{
  mpShader->use();
  mMvpMatrixLoc = mpShader->getUniformLocation("modelViewProjectionMatrix");
}

g3d::DefaultDepthMaterial::~DefaultDepthMaterial() {}

void g3d::DefaultDepthMaterial::use() const
{
  Material::use(); // enables shader program
  glUniformMatrix4fv(
      mMvpMatrixLoc, 1, GL_FALSE,
      glm::value_ptr(Shader::getGlobalMat4("modelViewProjectionMatrix")));
}
