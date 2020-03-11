#include "Graphics3d/Assets/Materials/TerrainDepthMaterial.h"
#include <Graphics3d/Assets/Shader.h>
#include <Graphics3d/Assets/ShaderManager.h>
#include <Graphics3d/Assets/Texture.h>
#include <Graphics3d/Assets/TextureManager.h>
#pragma warning(disable : 4201)
#include <glm/gtc/type_ptr.hpp>

G3d::TerrainDepthMaterial::TerrainDepthMaterial() :
	Material(ShaderManager::getInstance().getTerrainDepth()),
	mpHeightMap(TextureManager::getInstance().getRGB255()), // TODO: Luminence0 ?
	mMvpMatrixLoc(0)
{
	mpShader->use();
	GLuint program = mpShader->getHandle();
	glUniform1i(glGetUniformLocation(program, "heightMap"), 0);
	mMvpMatrixLoc = glGetUniformLocation(program, "modelViewProjectionMatrix");
}

G3d::TerrainDepthMaterial::~TerrainDepthMaterial()
{

}

void G3d::TerrainDepthMaterial::use() const
{
	Material::use(); // enables shader program
	glUniformMatrix4fv(mMvpMatrixLoc, 1, GL_FALSE, glm::value_ptr(Shader::getGlobalMat4("modelViewProjectionMatrix")));
	glActiveTexture(GL_TEXTURE0);
	mpHeightMap->use();
}

void G3d::TerrainDepthMaterial::setHeightMap(TextureCPtr pHeightMap)
{
	mpHeightMap = pHeightMap;
}

