#include "Graphics3d/Components/Component.h"
#include <Graphics3d/Scene/SceneObject.h>

G3d::Component::Component() : mpSceneObject(nullptr), mIsEnabled(true) {}

G3d::Component::~Component()
{
  if (mpSceneObject != nullptr)
  {
    mpSceneObject->removeComponent(this);
  }
}

void G3d::Component::update(double /*deltaTime*/) {}

void G3d::Component::render() const {}

void G3d::Component::renderDepth() const {}

G3d::SceneObject const * G3d::Component::getSceneObject() const
{
  return mpSceneObject;
}

G3d::SceneObject * G3d::Component::getSceneObject() { return mpSceneObject; }

void G3d::Component::setEnabled(bool isEnabled) { mIsEnabled = isEnabled; }

bool G3d::Component::isEnabled() const { return mIsEnabled; }
