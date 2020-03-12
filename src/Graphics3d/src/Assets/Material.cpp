#include "Graphics3d/Assets/Material.h"
#include <Graphics3d/Assets/Shader.h>

g3d::Material::Material(ShaderCPtr pShader) : 
mpShader(pShader)
{

}

g3d::Material::~Material()
{

}

void g3d::Material::use() const
{
	mpShader->use();
}

g3d::ShaderCPtr g3d::Material::getShader() const
{
	return mpShader;
}
