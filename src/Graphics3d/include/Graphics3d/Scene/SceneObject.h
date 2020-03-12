/**
 * @author Florian Feuerstein
 * @date 28.07.2016
 *
 * Scene objects are empty containers for components.
 * Adding components to a scene object is like
 * filling the scene object with logic and
 * functionality. Scene objects can have children
 * so that they create a hierarchy. The standard
 * component of a scene object is the
 * transform component giving it a distinct
 * position, rotation and scale. These transformations
 * are also applied to all children. The scene object is
 * also the owner of all its components and children.
 */

// TODO: make all SceneObjects and Components shared?

#pragma once

#include <map>
#include <string>
#include <typeinfo>
#include <vector>

namespace g3d
{

// forward declarations
class Component;
class Transform;

class SceneObject
{
public:

  static void instantiate(SceneObject * object, SceneObject * parent);

  static void destroy(SceneObject * object);

  static void updateSceneGraph();

  SceneObject(std::string const & name);

  ~SceneObject();

  /**
   * Clones all components and children with their components without cloning
   * any resources. The new scene object has no parent.
   */
  SceneObject * clone() const;

  // TODO: delete AddComponent and implement CreateComponent with perfect
  // forwarding?!

  /**
   * Adds the component to the scene object and commits ownership.
   * If the component could not be added the method returns false.
   */
  bool addComponent(Component * pComponent);

  // TODO: make RemoveComponent a template function with bool return! Does
  // anyone use this currently?

  /**
   * Removes the component from this scene object committing ownership to the
   * caller. Only call this, if you know what you're doing.
   */
  void removeComponent(Component * pComponent);

  /** Returns the component of the specific type or nullptr. */
  template <class TComponent> TComponent * getComponent()
  {
    std::map<size_t, Component *>::iterator iter =
        mComponents.find(typeid(TComponent).hash_code());
    if (iter != mComponents.end())
      return static_cast<TComponent *>(iter->second);
    return nullptr;
  }

  /** Returns the component of the specific type or nullptr. */
  template <class TComponent> TComponent const * getComponent() const
  {
    std::map<size_t, Component *>::const_iterator iter =
        mComponents.find(typeid(TComponent).hash_code());
    if (iter != mComponents.cend())
      return static_cast<TComponent const *>(iter->second);
    return nullptr;
  }

  /** Returns the parent scene object or nullptr. */
  SceneObject * getParent();

  /** Returns the parent scene object or nullptr. */
  SceneObject const * getParent() const;

  /** Returns the transform component. */
  Transform * getTransform();

  /** Returns the transform component. */
  Transform const * getTransform() const;

  /**
   * Adds the scene object as child and commits ownership.
   * If the child could not be added the method returns false.
   */
  bool addChild(SceneObject * pChild);

  /** Updates all components and children with their components. */
  void update(double deltaTime);

  /** Renders all components. */
  void render() const;

  /** Renders all components without Materials. */
  void renderDepth() const;

  /** Returns the number of children. */
  size_t getNumberOfChildren() const;

  /** Returns true if the node has no children. */
  bool isLeafNode() const;

  /** Return the child at the given index. Not boundary safe. */
  SceneObject * getChild(size_t index);

  /** Return the child at the given index. Not boundary safe. */
  SceneObject const * getChild(size_t index) const;

  /** Returns the first child with the given name. */
  SceneObject * getChild(std::string const & name);

  /** Returns the first child with the given name. */
  SceneObject const * getChild(std::string const & name) const;

  /** Enables or disables the scene object. */
  void setEnabled(bool isEnabled);

  /** Returns true if the scene object is enabled. */
  bool isEnabled() const;

  /** Returns the name of the scene object. */
  std::string const & getName() const;

  /**
   * Removes the child from this scene object committing ownership to the
   * caller. Only call this, if you know what you're doing.
   */
  void g3d::SceneObject::removeChild(SceneObject * child);

private:
  SceneObject(SceneObject const &) = delete;

  SceneObject & operator=(SceneObject const &) = delete;

  /** This is just a helper constructor for clone method. */
  SceneObject(std::string const & name, Transform * pTransform);

  /** Checks if the given node is a descendant of this scene object. */
  bool isNodeDescendant(SceneObject const * pNode) const;

  /** The name of this scene object. */
  std::string mName;

  /** The parent scene object. */
  SceneObject * mpParent;

  /** The children scene objects. */
  std::vector<SceneObject *> mChildren;

  /** The standard transform component. */
  Transform * mpTransform;

  /**
   * The map of all components. The key is a unique identifier
   * of the component class. Thus only one instance of one
   * specific component type is allowed, which is part of the concept.
   */
  std::map<size_t, Component *> mComponents;

  /**
   * If a scene object is disabled then its components as well as its children
   * are neither updated nor rendered.
   */
  bool mIsEnabled;
};



} // namespace g3d
