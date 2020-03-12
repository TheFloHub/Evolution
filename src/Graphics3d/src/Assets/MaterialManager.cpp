#include "Graphics3d/Assets/MaterialManager.h"
#include "Graphics3d/Assets/Material.h"
#include <Graphics3d/Assets/Materials/DefaultDepthMaterial.h>

g3d::MaterialManager & g3d::MaterialManager::getInstance()
{
  static MaterialManager instance;
  return instance;
}

g3d::MaterialManager::MaterialManager() : mMaterialMap() {}

g3d::MaterialManager::~MaterialManager() {}

g3d::MaterialCPtr g3d::MaterialManager::getDefaultDepthMaterial() const
{
  return get<DefaultDepthMaterial>("DefaultDepthMaterial");
}

void g3d::MaterialManager::initStandardMaterials()
{
  create<DefaultDepthMaterial>("DefaultDepthMaterial");
}
