#pragma warning(disable : 4201)
#include "Graphics3d/Scene/SceneObject.h"
#include "Graphics3d/Components/Component.h"
#include "Graphics3d/Components/Transform.h"
#include <algorithm>

using namespace G3d;

G3d::SceneObject::SceneObject(std::string const & name)
    : mName(name), mpParent(nullptr), mChildren(), mpTransform(new Transform),
      mComponents(), mIsEnabled(true)
{
  // HINT: this works here for Transform because no other methods using <this>
  // are called.
  mpTransform->mpSceneObject = this;
}

G3d::SceneObject::SceneObject(std::string const & name, Transform * pTransform)
    : mName(name), mpParent(nullptr), mChildren(), mpTransform(pTransform),
      mComponents(), mIsEnabled(true)
{
  // HINT: this works here for Transform because no other methods using <this>
  // are called.
  mpTransform->mpSceneObject = this;
}

G3d::SceneObject::~SceneObject()
{
  if (mpParent != nullptr)
  {
    mpParent->removeChild(this);
  }
  mpTransform->mpSceneObject = nullptr;
  delete mpTransform;
  for (auto iter = mComponents.begin(); iter != mComponents.end(); ++iter)
  {
    iter->second->mpSceneObject = nullptr; // detach component
    delete iter->second;                   // delete component
  }
  mComponents.clear();
  for (auto iter = mChildren.begin(); iter != mChildren.end(); ++iter)
  {
    (*iter)->mpParent = nullptr; // detach node
    delete (*iter);              // delete node
  }
  mChildren.clear();
}

G3d::SceneObject * G3d::SceneObject::clone() const
{
  SceneObject * pSceneObject = new SceneObject(
      mName,
      static_cast<Transform *>(static_cast<Component *>(mpTransform)->clone()));
  pSceneObject->mIsEnabled = mIsEnabled;
  for (auto iter = mComponents.cbegin(); iter != mComponents.cend(); ++iter)
  {
    pSceneObject->addComponent(iter->second->clone());
  }
  for (auto iter = mChildren.begin(); iter != mChildren.end(); ++iter)
  {
    pSceneObject->addChild((*iter)->clone());
  }
  return pSceneObject;
}

bool G3d::SceneObject::addComponent(Component * pComponent)
{
  // don't add if null or if there already exists an owner
  if (pComponent == nullptr || pComponent->mpSceneObject != nullptr)
    return false;
  // insert the component if there isn't a component of the same type yet.
  auto infoPair = mComponents.insert(std::pair<size_t, Component *>(
      typeid(*pComponent).hash_code(), pComponent));
  // if insertion was successfull set this as the owner
  if (infoPair.second)
  {
    infoPair.first->second->mpSceneObject = this;
  }
  return infoPair.second;
}

void G3d::SceneObject::removeComponent(Component * pComponent)
{
  mComponents.erase(typeid(*pComponent).hash_code());
}

Transform * G3d::SceneObject::getTransform() { return mpTransform; }

Transform const * G3d::SceneObject::getTransform() const { return mpTransform; }

bool G3d::SceneObject::addChild(SceneObject * pChild)
{
  if (pChild == nullptr || pChild == this || pChild->mpParent == this)
    return false;
  // if there already exists a parent for the new child
  if (pChild->mpParent != nullptr)
  {
    // and it's not valid to add the child return false.
    if (pChild->isNodeDescendant(this))
    {
      return false;
    }
    // otherwise detach the child
    else
    {
      pChild->mpParent->removeChild(pChild);
    }
    return false;
  }
  pChild->mpParent = this;
  mChildren.push_back(pChild);
  return true;
}

void G3d::SceneObject::removeChild(SceneObject * child)
{
  std::vector<SceneObject *>::iterator iter = mChildren.begin();
  std::vector<SceneObject *>::iterator last = mChildren.end();
  while (iter != last && (*iter) != child)
  {
    ++iter;
  }
  if (iter != last)
  {
    mChildren.erase(iter);
    child->mpParent = nullptr;
  }
}

void G3d::SceneObject::update(double deltaTime)
{
  // HINT: no need to update transform
  for (auto iter = mComponents.begin(); iter != mComponents.end(); ++iter)
  {
    if (iter->second->isEnabled())
    {
      iter->second->update(deltaTime);
    }
  }
  for (auto iter = mChildren.begin(); iter != mChildren.end(); ++iter)
  {
    if ((*iter)->isEnabled())
    {
      (*iter)->update(deltaTime);
    }
  }
}

void G3d::SceneObject::render() const
{
  // HINT: no need to render transform
  for (auto iter = mComponents.cbegin(); iter != mComponents.cend(); ++iter)
  {
    if (iter->second->isEnabled())
    {
      iter->second->render();
    }
  }
}

void G3d::SceneObject::renderDepth() const
{
  // HINT: no need to render transform
  for (auto iter = mComponents.cbegin(); iter != mComponents.cend(); ++iter)
  {
    if (iter->second->isEnabled())
    {
      iter->second->renderDepth();
    }
  }
}

bool G3d::SceneObject::isNodeDescendant(SceneObject const * pNode) const
{
  if (pNode == nullptr)
    return false;
  SceneObject const * pCurrentNode = pNode;
  while (pCurrentNode != nullptr && pCurrentNode != this)
    pCurrentNode = pCurrentNode->mpParent;
  return pCurrentNode == this;
}

size_t G3d::SceneObject::getNumberOfChildren() const
{
  return mChildren.size();
}

SceneObject * G3d::SceneObject::getParent() { return mpParent; }

SceneObject const * G3d::SceneObject::getParent() const { return mpParent; }

bool G3d::SceneObject::isLeafNode() const { return mChildren.empty(); }

SceneObject * G3d::SceneObject::getChild(size_t index)
{
  return mChildren[index];
}

SceneObject const * G3d::SceneObject::getChild(size_t index) const
{
  return mChildren[index];
}

SceneObject * G3d::SceneObject::getChild(std::string const & name)
{
  auto iter = mChildren.begin();
  auto end = mChildren.end();
  while (iter != end && (*iter)->mName != name)
  {
    ++iter;
  }
  if (iter != end)
  {
    return *iter;
  }
  return nullptr;
}

SceneObject const * G3d::SceneObject::getChild(std::string const & name) const
{
  auto iter = mChildren.cbegin();
  auto end = mChildren.cend();
  while (iter != end && (*iter)->mName != name)
  {
    ++iter;
  }
  if (iter != end)
  {
    return *iter;
  }
  return nullptr;
}

void G3d::SceneObject::setEnabled(bool isEnabled) { mIsEnabled = isEnabled; }

bool G3d::SceneObject::isEnabled() const { return mIsEnabled; }

std::string const & G3d::SceneObject::getName() const { return mName; }
