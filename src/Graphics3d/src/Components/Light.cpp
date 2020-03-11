#include "Graphics3d/Components/Light.h"
#include <algorithm>

G3d::Light::Light(G3d::Light::Type type)
    : Component(), mType(type), mColor(1.0f, 1.0f, 1.0f), mIntensity(1.0f)
{
}

G3d::Light::~Light() {}

G3d::Component * G3d::Light::clone() const
{
  Light * pLight = new Light(mType);
  pLight->mIsEnabled = mIsEnabled;
  pLight->mType = mType;
  pLight->mColor = mColor;
  pLight->mIntensity = mIntensity;
  return pLight;
}

G3d::Light::Type G3d::Light::getType() const { return mType; }

void G3d::Light::setType(G3d::Light::Type type) { mType = type; }

bool G3d::Light::isDirectionalLight() const
{
  return mType == Light::DIRECTIONAL;
}

bool G3d::Light::isPointLight() const { return mType == Light::POINT; }

glm::vec3 const & G3d::Light::getColor() const { return mColor; }

void G3d::Light::setColor(glm::vec3 const & color) { mColor = color; }

float G3d::Light::getIntensity() const { return mIntensity; }

void G3d::Light::setIntensity(float intensity) { mIntensity = intensity; }
