#include "Graphics3d/Assets/Material.h"
#include <Graphics3d/Assets/Shader.h>

G3d::Material::Material(ShaderCPtr pShader) : 
mpShader(pShader)
{

}

G3d::Material::~Material()
{

}

void G3d::Material::use() const
{
	mpShader->use();
}

G3d::ShaderCPtr G3d::Material::getShader() const
{
	return mpShader;
}
