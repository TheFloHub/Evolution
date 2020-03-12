/**
 * @author Florian Feuerstein
 * @date 28.07.2016
 *
 * Each scene object has a transform component, which stores
 * the position, rotation and scale relative to the parent
 * transform. The class also provides functions to manipulate
 * this attributes.
 */

#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace g3d
{

class Transform : public Component
{
public:
  // TODO: make all this more performant!!! inverse, dirty flag, etc..

  Transform();

  virtual ~Transform();

  void scale(glm::vec3 const & scale);

  void rotate(float angle, glm::vec3 const & axis);

  void translate(glm::vec3 const & translation);

  void setScale(glm::vec3 const & scale);

  void setRotation(float angle, glm::vec3 const & axis);

  void setRotation(glm::quat const & quaternion);

  void setPosition(glm::vec3 const & position);

  void lookAt(glm::vec3 const & eye, glm::vec3 const & center,
              glm::vec3 const & up);

  glm::mat4 getLocalToParentMatrix() const;

  glm::mat4 getParentToLocalMatrix() const;

  glm::mat4 getLocalToWorldMatrix() const;

  glm::mat4 getWorldToLocalMatrix() const;

  glm::mat3 getLocalToWorldRotationMatrix() const;

  glm::mat3 getWorldToLocalRotationMatrix() const;

protected:
  Component * clone() const override;

  glm::vec3 mScale;

  glm::vec3 mPosition;

  glm::quat mQuaternion;

private:
  Transform(Transform const &) = delete;

  Transform & operator=(Transform const &) = delete;
};

} // namespace g3d
