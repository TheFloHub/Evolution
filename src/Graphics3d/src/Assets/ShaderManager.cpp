#include "Graphics3d/Assets/ShaderManager.h"
#include "Graphics3d/Assets/Shader.h"
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>


G3d::ShaderManager& G3d::ShaderManager::getInstance()
{
	static ShaderManager instance;
	return instance;
}

G3d::ShaderManager::ShaderManager() :
mShaderMap()
{
}

G3d::ShaderManager::~ShaderManager()
{
}

G3d::ShaderPtr G3d::ShaderManager::loadFromFile(
	std::string const& shaderName, 
	std::string const& vertexShaderFileName,
	std::string const& fragmentShaderFileName)
{
	ShaderPtr pShader = get(shaderName);
	if (pShader)
	{
		std::cout << "Shader with name " << shaderName << " is already existing." << std::endl;
		return pShader;
	}

	std::ifstream vertexShaderFile(vertexShaderFileName);
	std::string vertexShader((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());

	std::ifstream fragmentShaderFile(fragmentShaderFileName);
	std::string fragmentShader((std::istreambuf_iterator<char>(fragmentShaderFile)), std::istreambuf_iterator<char>());

	if (vertexShader.empty() || fragmentShader.empty())
	{
		return pShader;
	}

	return (mShaderMap.insert(std::make_pair(shaderName, ShaderPtr(new Shader(vertexShader, fragmentShader))))).first->second;
}

G3d::ShaderPtr G3d::ShaderManager::get(std::string const& shaderName) const
{
	auto iter = mShaderMap.find(shaderName);
	if (iter != mShaderMap.end())
	{
		return iter->second;
	}
	return ShaderPtr();
}

G3d::ShaderPtr G3d::ShaderManager::getPBRStandard() const
{
	return get("PBRStandard"); 
}

G3d::ShaderPtr G3d::ShaderManager::getPBRTextured() const
{
	return get("PBRTextured");
}

G3d::ShaderPtr G3d::ShaderManager::getDiffuse() const
{
	return get("Diffuse");
}

G3d::ShaderPtr G3d::ShaderManager::getSpecular() const
{
	return get("Specular");
}

G3d::ShaderPtr G3d::ShaderManager::getDefaultDepth() const
{
	return get("DefaultDepth");
}

G3d::ShaderPtr G3d::ShaderManager::getTerrainDepth() const
{ return nullptr; }

G3d::ShaderPtr G3d::ShaderManager::getTerrain() const
{ return nullptr; }

bool G3d::ShaderManager::initStandardShader()
{
	std::string baseHome("D:\\Repositories\\Evolution\\src\\Graphics3d\\src\\Resources\\Shader\\");
	std::string baseWork("D:\\Code\\Evolution\\src\\Graphics3d\\src\\Resources\\Shader\\");

	std::string base = baseHome;
	
	// TODO: make Visual Studio resource files or so
	if (!loadFromFile("Diffuse", base + "Diffuse.vert", base + "Diffuse.frag"))
		return false;

	if (!loadFromFile("Specular", base + "Specular.vert", base + "Specular.frag"))
		return false;

	if (!loadFromFile("DeferredLight", base + "DeferredLight.vert", base + "DeferredLight.frag"))
		return false;

	if (!loadFromFile("DefaultDepth", base + "DefaultDepth.vert", base + "DefaultDepth.frag"))
		return false;

	if (!loadFromFile("PBRStandard", base + "PBRStandard.vert", base + "PBRStandard.frag"))
		return false;

	if (!loadFromFile("PBRTextured", base + "PBRTextured.vert", base + "PBRTextured.frag"))
		return false;

	if (!loadFromFile("FinalShading", base + "FinalShading.vert", base + "FinalShading.frag"))
		return false;

	return true;
}
