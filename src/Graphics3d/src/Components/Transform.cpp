#pragma warning(disable : 4201)
#include "Graphics3d/Components/Transform.h"
#include <Graphics3d/Scene/SceneObject.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <gtx/quaternion.hpp>
//#include <glm/gtx/io.hpp>
//#include <iostream>

G3d::Transform::Transform()
    : Component(), mScale(1.0f, 1.0f, 1.0f), mPosition(0.0f, 0.0f, 0.0f),
      mQuaternion()
{
  // glm::mat4 m;

  // glm::mat4 ra1 = glm::rotate(m, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f,
  // 0.0f)); glm::mat4 ra2 = glm::rotate(ra1, glm::radians(90.0f),
  // glm::vec3(1.0f, 0.0f, 0.0f)); // std::cout << ra2 << std::endl << std::endl;

  // glm::mat4 rb1 = glm::rotate(m, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f,
  // 0.0f)); glm::mat4 rb2 = glm::rotate(m, glm::radians(90.0f), glm::vec3(1.0f,
  // 0.0f, 0.0f)); // std::cout << rb1 * rb2 << std::endl << std::endl;
                  // std::cout << rb2 * rb1 << std::endl << std::endl;
}

G3d::Transform::~Transform() {}

G3d::Component * G3d::Transform::clone() const
{
  Transform * pTransform = new Transform();
  pTransform->mIsEnabled = mIsEnabled;
  pTransform->mScale = mScale;
  pTransform->mPosition = mPosition;
  pTransform->mQuaternion = mQuaternion;
  return pTransform;
}

void G3d::Transform::scale(glm::vec3 const & scale) { mScale *= scale; }

void G3d::Transform::rotate(float angle, glm::vec3 const & axis)
{
  // mQuaternion = glm::rotate(mQuaternion, angle, axis);
  mQuaternion = glm::angleAxis(glm::radians(angle), axis) * mQuaternion;
}

void G3d::Transform::translate(glm::vec3 const & translation)
{
  mPosition += translation;
}

void G3d::Transform::setScale(glm::vec3 const & scale) { mScale = scale; }

void G3d::Transform::setRotation(float angle, glm::vec3 const & axis)
{
  mQuaternion = glm::angleAxis(glm::radians(angle), axis);
}

void G3d::Transform::setRotation(glm::quat const & quaternion)
{
  mQuaternion = quaternion;
}

void G3d::Transform::setPosition(glm::vec3 const & position)
{
  mPosition = position;
}

void G3d::Transform::lookAt(glm::vec3 const & eye, glm::vec3 const & center,
                            glm::vec3 const & up)
{
  glm::vec3 const f(normalize(center - eye));
  glm::vec3 const s(normalize(cross(up, f)));
  glm::vec3 const u(cross(f, s));

  glm::mat3 r(1.0f);
  r[0][0] = s.x;
  r[0][1] = s.y;
  r[0][2] = s.z;

  r[1][0] = u.x;
  r[1][1] = u.y;
  r[1][2] = u.z;

  r[2][0] = f.x;
  r[2][1] = f.y;
  r[2][2] = f.z;

  mQuaternion = glm::quat_cast(r);
  mPosition = eye;
}

glm::mat4 G3d::Transform::getLocalToParentMatrix() const
{
  return glm::translate(glm::mat4(), mPosition) * glm::mat4_cast(mQuaternion) *
         glm::scale(glm::mat4(), mScale);
}

glm::mat4 G3d::Transform::getParentToLocalMatrix() const
{
  glm::vec3 inverseScale(1.0f / mScale.x, 1.0f / mScale.y, 1.0f / mScale.z);
  return glm::scale(glm::mat4(), inverseScale) *
         glm::mat4_cast(glm::inverse(mQuaternion)) *
         glm::translate(glm::mat4(), -mPosition);
  // alternative !?: return glm::affineInverse(GetLocalToParentMatrix());
}

glm::mat4 G3d::Transform::getLocalToWorldMatrix() const
{
  if (mpSceneObject != nullptr && mpSceneObject->getParent() != nullptr)
  {
    return mpSceneObject->getParent()->getTransform()->getLocalToWorldMatrix() *
           getLocalToParentMatrix();
  }
  return getLocalToParentMatrix(); // glm::mat4() or GetLocalToParentMatrix() ?
}

glm::mat4 G3d::Transform::getWorldToLocalMatrix() const
{
  if (mpSceneObject != nullptr && mpSceneObject->getParent() != nullptr)
  {
    return getParentToLocalMatrix() *
           mpSceneObject->getParent()->getTransform()->getWorldToLocalMatrix();
  }
  return getParentToLocalMatrix(); // glm::mat4() or GetParentToLocalMatrix() ?
}

glm::mat3 G3d::Transform::getLocalToWorldRotationMatrix() const // TODO: scale?
{
  if (mpSceneObject != nullptr && mpSceneObject->getParent() != nullptr)
  {
    return mpSceneObject->getParent()
               ->getTransform()
               ->getLocalToWorldRotationMatrix() *
           glm::mat3_cast(mQuaternion);
  }
  return glm::mat3_cast(mQuaternion);
}

glm::mat3 G3d::Transform::getWorldToLocalRotationMatrix() const // TODO: scale?
{
  if (mpSceneObject != nullptr && mpSceneObject->getParent() != nullptr)
  {
    return glm::mat3_cast(glm::inverse(mQuaternion)) *
           mpSceneObject->getParent()
               ->getTransform()
               ->getWorldToLocalRotationMatrix();
  }
  return glm::mat3_cast(
      glm::inverse(mQuaternion)); // glm::mat4() or GetParentToLocalMatrix() ?
}
