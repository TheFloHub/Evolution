/**
 * @author Florian Feuerstein
 * @date 03.08.2016
 *
 * The shader manager contains all loaded shaders and provides access to them.
 */

#pragma once

#include "Material.h"
#include <Graphics3d/SharedPtrTypes.h>
#include <map>
#include <string>

namespace g3d
{

class MaterialManager
{
public:
  static MaterialManager & getInstance();

  ~MaterialManager();

  template <class TMaterial>
  std::shared_ptr<TMaterial> create(std::string const & materialName)
  {
    std::map<std::string, MaterialPtr>::const_iterator iter =
        mMaterialMap.find(materialName);
    if (iter != mMaterialMap.end())
    {
      std::shared_ptr<TMaterial> pSpecifiMaterial =
          std::dynamic_pointer_cast<TMaterial>(iter->second);
      if (pSpecifiMaterial)
      {
        return pSpecifiMaterial;
      }
      else
      {
        return std::shared_ptr<TMaterial>();
      }
    }
    std::shared_ptr<TMaterial> pSpecifiMaterial(new TMaterial);
    mMaterialMap.insert(std::make_pair(
        materialName, std::static_pointer_cast<Material>(pSpecifiMaterial)));
    return pSpecifiMaterial;
  }

  template <class TMaterial>
  std::shared_ptr<TMaterial> get(std::string const & materialName) const
  {
    std::map<std::string, MaterialPtr>::const_iterator iter =
        mMaterialMap.find(materialName);
    if (iter != mMaterialMap.end())
    {
      std::shared_ptr<TMaterial> pSpecifiMaterial =
          std::dynamic_pointer_cast<TMaterial>(iter->second);
      if (pSpecifiMaterial)
      {
        return pSpecifiMaterial;
      }
    }
    return std::shared_ptr<TMaterial>();
  }

  MaterialPtr getDefaultDepthMaterial() const;

  void initStandardMaterials();

private:
  MaterialManager();

  MaterialManager(MaterialManager const &) = delete;

  MaterialManager & operator=(MaterialManager const &) = delete;

  std::map<std::string, MaterialPtr> mMaterialMap;
};

} // namespace g3d
