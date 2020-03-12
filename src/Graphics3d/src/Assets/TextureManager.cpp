#include "Graphics3d/Assets/TextureManager.h"
#include "Graphics3d/Assets/Texture.h"
#include <glm/gtc/noise.hpp>
#include <iostream>
#include <random>

g3d::TextureManager & g3d::TextureManager::getInstance()
{
  static TextureManager instance;
  return instance;
}

g3d::TextureManager::TextureManager() : mTextureMap() {}

g3d::TextureManager::~TextureManager() {}

g3d::TexturePtr g3d::TextureManager::load(std::string const & textureName,
                                          std::string const & fileName)
{
  TexturePtr pTexture = get(textureName);
  if (pTexture)
  {
    std::cout << "Texture with name " << textureName << " is already existing."
              << std::endl;
    return pTexture;
  }
  return (mTextureMap.insert(
              std::make_pair(textureName, TexturePtr(new Texture(fileName)))))
      .first->second;
}

g3d::TexturePtr g3d::TextureManager::get(std::string const & textureName) const
{
  std::map<std::string, TexturePtr>::const_iterator iter =
      mTextureMap.find(textureName);
  if (iter != mTextureMap.end())
  {
    return iter->second;
  }
  return TexturePtr();
}

g3d::TexturePtr g3d::TextureManager::getRGB255() const { return get("RGB255"); }

g3d::TexturePtr g3d::TextureManager::getR255G0B0() const
{
  return get("R255G0B0");
}

g3d::TexturePtr g3d::TextureManager::getRGBA255() const
{
  return get("RGBA255");
}

g3d::TexturePtr g3d::TextureManager::getL255() const { return get("L255"); }

g3d::TexturePtr g3d::TextureManager::getL0() const { return get("L0"); }

g3d::TexturePtr g3d::TextureManager::getMinRoughness() const
{
  return get("MinRoughness");
}

g3d::TexturePtr g3d::TextureManager::getDefaultNormalMap() const
{
  return get("DefaultNormalMap");
}

g3d::TexturePtr g3d::TextureManager::getDefaultHeightMap() const
{
  return get("DefaultHeightMap");
}

g3d::TexturePtr g3d::TextureManager::getTerrainNoise() const
{
  return get("TerrainNoise");
}

void g3d::TextureManager::initStandardTextures()
{
  // RGB255
  std::vector<unsigned char> data(4 * 4 * 3, 255);
  mTextureMap.insert(
      std::make_pair("RGB255", TexturePtr(new Texture(4, 4, 3, data))));

  // R255G0B0
  data = std::vector<unsigned char>(4 * 4 * 3, 0);
  for (size_t i = 0; i < data.size(); i += 3)
  {
    data[i] = 255;
  }
  mTextureMap.insert(
      std::make_pair("R255G0B0", TexturePtr(new Texture(4, 4, 3, data))));

  // RGBA255
  data = std::vector<unsigned char>(4 * 4 * 4, 255);
  mTextureMap.insert(
      std::make_pair("RGBA255", TexturePtr(new Texture(4, 4, 4, data))));

  // L255
  data = std::vector<unsigned char>(4 * 4 * 1, 255);
  mTextureMap.insert(
      std::make_pair("L255", TexturePtr(new Texture(4, 4, 1, data))));

  // L0
  data = std::vector<unsigned char>(4 * 4 * 1, 0);
  mTextureMap.insert(
      std::make_pair("L0", TexturePtr(new Texture(4, 4, 1, data))));

  // StandardNormalMap
  data = std::vector<unsigned char>(4 * 4 * 3, (unsigned char)(255 / 2));
  for (size_t i = 2; i < data.size(); i += 3)
  {
    data[i] = 255;
  }
  mTextureMap.insert(std::make_pair("DefaultNormalMap",
                                    TexturePtr(new Texture(4, 4, 3, data))));

  // MinRoughness
  data = std::vector<unsigned char>(4 * 4 * 1, 13);
  mTextureMap.insert(
      std::make_pair("MinRoughness", TexturePtr(new Texture(4, 4, 1, data))));

  createHeightMap();
  createTerrainNoise();
}

void g3d::TextureManager::createHeightMap()
{
  std::vector<float> data;
  data.reserve(512 * 512);

  for (float y = 0; y < 512; ++y)
  {
    for (float x = 0; x < 512; ++x)
    {
      data.push_back(0.5f +
                     0.5f * glm::simplex(glm::vec2(x / 100.0f, y / 100.0f)));
    }
  }

  // for (float y = 0; y < 512; ++y)
  //{
  //	for (float x = 0; x < 512; ++x)
  //	{
  //		float xt = x - 256.0f;
  //		float yt = y - 256.0f;
  //		data.push_back(std::max(std::max(1.0 - 0.01*xt*xt - 0.01*yt*yt, 1.0
  //- 0.01*x*x - 0.01*yt*yt), 0.0));
  //	}
  //}

  mTextureMap.insert(std::make_pair(
      "DefaultHeightMap", TexturePtr(new Texture(512, 512, 1, data))));
}

void g3d::TextureManager::createTerrainNoise()
{
  unsigned int size = 512;
  std::vector<float> data;
  data.reserve(size * size);

  // for (float y = 0; y < 1024; ++y)
  //{
  //	for (float x = 0; x < 1024; ++x)
  //	{
  //		data.push_back(0.5f + 0.5f*glm::simplex(glm::vec2(x/10.0f,
  //y/10.0f)));
  //	}
  //}

  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
  for (float y = 0; y < (float)size; ++y)
  {
    for (float x = 0; x < (float)size; ++x)
    {
      data.push_back(distribution(generator));
    }
  }

  mTextureMap.insert(std::make_pair(
      "TerrainNoise", TexturePtr(new Texture(size, size, 1, data))));
}
