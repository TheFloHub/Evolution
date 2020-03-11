/**
 * @author Florian Feuerstein
 * @date 28.07.2016
 *
 * Base class for everything attached to scene objects.
 * Components add logic and functionality to empty scene objects.
 */

#pragma once

namespace G3d
{

// forward declarations
class SceneObject;

class Component
{

  friend class SceneObject;

public:
  Component();

  virtual ~Component();

  virtual void update(double deltaTime);

  virtual void render() const;

  virtual void renderDepth() const;

  SceneObject const * getSceneObject() const;

  SceneObject * getSceneObject();

  void setEnabled(bool isEnabled);

  bool isEnabled() const;

protected:
  /**
   * Clones the component without cloning any resources.
   * The new component is not attached to any scene object.
   */
  virtual Component * clone() const = 0;

  SceneObject * mpSceneObject;

  bool mIsEnabled;

private:
  Component(Component const &) = delete;

  Component & operator=(Component const &) = delete;
};

} // namespace G3d
