/**
 * @author Florian Feuerstein
 * @date 01.08.2016
 *
 * A camera renders the scene from its point of view.
 */

#pragma once

#include "Component.h"
#include <glm/glm.hpp>

namespace g3d
{

class Camera : public Component
{
public:
  Camera(unsigned int width, unsigned int height, float nearPlane,
         float farPlane, float fov);

  virtual ~Camera();

  glm::mat4 const & getProjectionMatrix() const;

  glm::mat4 const & getInverseProjectionMatrix() const;

  void setImageSize(unsigned int width, unsigned int height);

  float getNearPlane() const;

  float getFarPlane() const;

protected:
  Component * clone() const override;

  unsigned int mWidth;

  unsigned int mHeight;

  float mNearPlane;

  float mFarPlane;

  float mFov;

  glm::mat4 mProjectionMatrix;

  glm::mat4 mInverseProjectionMatrix;

private:
  Camera(Camera const &) = delete;

  Camera & operator=(Camera const &) = delete;
};

} // namespace g3d
