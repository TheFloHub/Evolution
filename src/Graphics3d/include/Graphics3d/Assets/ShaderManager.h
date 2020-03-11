/**
 * @author Florian Feuerstein
 * @date 03.08.2016
 *
 * The shader manager contains all loaded shaders and provides access to them.
 */

#pragma once

#include <Graphics3d/SharedPtrTypes.h>
#include <map>
#include <string>

namespace G3d
{

class ShaderManager
{
public:
  static ShaderManager & getInstance();

  ~ShaderManager();

  ShaderPtr loadFromFile(std::string const & shaderName,
                         std::string const & vertexShaderFileName,
                         std::string const & fragmentShaderFileName);

  ShaderPtr get(std::string const & shaderName) const;

  ShaderPtr getPBRStandard() const;

  ShaderPtr getPBRTextured() const;

  ShaderPtr getDiffuse() const;

  ShaderPtr getSpecular() const;

  ShaderPtr getDefaultDepth() const;

  ShaderPtr getTerrainDepth() const;

  ShaderPtr getTerrain() const;

  bool initStandardShader();

private:
  ShaderManager();

  ShaderManager(ShaderManager const &) = delete;

  ShaderManager & operator=(ShaderManager const &) = delete;

  std::map<std::string, ShaderPtr> mShaderMap;
};

} // namespace G3d
