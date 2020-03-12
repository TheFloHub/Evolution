/**
 * @author Florian Feuerstein
 * @date 03.08.2016
 *
 * The texture manager contains all loaded textures and provides access to them.
 */

#pragma once

#include <Graphics3d/SharedPtrTypes.h>
#include <map>
#include <string>

namespace g3d
{

class TextureManager
{
public:
  static TextureManager & getInstance();

  ~TextureManager();

  TexturePtr load(std::string const & textureName,
                  std::string const & fileName);

  TexturePtr get(std::string const & textureName) const;

  TexturePtr getRGB255() const;

  TexturePtr getR255G0B0() const;

  TexturePtr getRGBA255() const;

  TexturePtr getL255() const;

  TexturePtr getL0() const;

  TexturePtr getMinRoughness() const;

  TexturePtr getDefaultNormalMap() const;

  // TODO: should only exist in TerrainClass
  TexturePtr getDefaultHeightMap() const;

  TexturePtr getTerrainNoise() const;

  void initStandardTextures();

private:
  TextureManager();
  TextureManager(TextureManager const &) = delete;
  TextureManager & operator=(TextureManager const &) = delete;

  void createHeightMap();

  void createTerrainNoise();

  std::map<std::string, TexturePtr> mTextureMap;
};

} // namespace g3d
