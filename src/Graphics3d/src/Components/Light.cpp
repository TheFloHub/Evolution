#include "Graphics3d/Components/Light.h"
#include <algorithm>

g3d::Light::Light(g3d::Light::Type type)
    : Component(), mType(type), mColor(1.0f, 1.0f, 1.0f), mIntensity(1.0f)
{
}

g3d::Light::~Light() {}

g3d::Component * g3d::Light::clone() const
{
  Light * pLight = new Light(mType);
  pLight->mIsEnabled = mIsEnabled;
  pLight->mType = mType;
  pLight->mColor = mColor;
  pLight->mIntensity = mIntensity;
  return pLight;
}

g3d::Light::Type g3d::Light::getType() const { return mType; }

void g3d::Light::setType(g3d::Light::Type type) { mType = type; }

bool g3d::Light::isDirectionalLight() const
{
  return mType == Light::DIRECTIONAL;
}

bool g3d::Light::isPointLight() const { return mType == Light::POINT; }

glm::vec3 const & g3d::Light::getColor() const { return mColor; }

void g3d::Light::setColor(glm::vec3 const & color) { mColor = color; }

float g3d::Light::getIntensity() const { return mIntensity; }

void g3d::Light::setIntensity(float intensity) { mIntensity = intensity; }
