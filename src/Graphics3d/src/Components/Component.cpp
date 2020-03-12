#include "Graphics3d/Components/Component.h"
#include <Graphics3d/Scene/SceneObject.h>

g3d::Component::Component() : mpSceneObject(nullptr), mIsEnabled(true) {}

g3d::Component::~Component()
{
  if (mpSceneObject != nullptr)
  {
    mpSceneObject->removeComponent(this);
  }
}

void g3d::Component::update(double /*deltaTime*/) {}

void g3d::Component::render() const {}

void g3d::Component::renderDepth() const {}

g3d::SceneObject const * g3d::Component::getSceneObject() const
{
  return mpSceneObject;
}

g3d::SceneObject * g3d::Component::getSceneObject() { return mpSceneObject; }

void g3d::Component::setEnabled(bool isEnabled) { mIsEnabled = isEnabled; }

bool g3d::Component::isEnabled() const { return mIsEnabled; }
