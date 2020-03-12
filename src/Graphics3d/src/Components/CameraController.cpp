#pragma warning(disable : 4201)
#include "Graphics3d/Components/CameraController.h"
#include "Graphics3d/Components/Camera.h"
#include "Graphics3d/Components/Transform.h"
#include <Graphics3d/Input/InputManager.h>
#include <Graphics3d/Scene/SceneObject.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

g3d::CameraController::CameraController()
    : Component(), mMoveSpeed(2.0), mRotationSpeed(10.0), mRotationX(0.0),
      mRotationY(0.0)
{
}

g3d::CameraController::~CameraController() {}

g3d::Component * g3d::CameraController::clone() const
{
  CameraController * pCameraController = new CameraController();
  pCameraController->mIsEnabled = mIsEnabled;
  pCameraController->mMoveSpeed = mMoveSpeed;
  pCameraController->mRotationSpeed = mRotationSpeed;
  return pCameraController;
}

void g3d::CameraController::update(double deltaTime)
{
  if (InputManager::getInstance().getKeyDown(KEY_E))
  {
    mMoveSpeed += 1.0;
  }
  if (InputManager::getInstance().getKeyDown(KEY_Q))
  {
    mMoveSpeed -= 1.0;
  }

  Transform & transform = *(mpSceneObject->getTransform());

  // rotation
  mRotationX -=
      InputManager::getInstance().getMouseDeltaX() * mRotationSpeed * deltaTime;
  mRotationY +=
      InputManager::getInstance().getMouseDeltaY() * mRotationSpeed * deltaTime;
  transform.setRotation(glm::angleAxis(glm::radians((float)mRotationX),
                                       glm::vec3(0.0f, 1.0f, 0.0f)) *
                        glm::angleAxis(glm::radians((float)mRotationY),
                                       glm::vec3(1.0f, 0.0f, 0.0f)));

  // translation
  glm::mat4 localToWorldMatrix = transform.getLocalToWorldMatrix();
  glm::vec3 forwardDir(localToWorldMatrix[2][0], localToWorldMatrix[2][1],
                       localToWorldMatrix[2][2]);
  glm::vec3 upDir(localToWorldMatrix[1][0], localToWorldMatrix[1][1],
                  localToWorldMatrix[1][2]);
  glm::vec3 leftDir(localToWorldMatrix[0][0], localToWorldMatrix[0][1],
                    localToWorldMatrix[0][2]);
  glm::vec3 translation(0.0f, 0.0f, 0.0f);

  if (InputManager::getInstance().getKey(KEY_W))
  {
    translation += forwardDir;
  }
  if (InputManager::getInstance().getKey(KEY_S))
  {
    translation -= forwardDir;
  }
  if (InputManager::getInstance().getKey(KEY_A))
  {
    translation += leftDir;
  }
  if (InputManager::getInstance().getKey(KEY_D))
  {
    translation -= leftDir;
  }
  if (InputManager::getInstance().getKey(KEY_SPACE))
  {
    translation += upDir;
  }
  if (InputManager::getInstance().getKey(KEY_LEFT_SHIFT))
  {
    translation -= upDir;
  }

  if (glm::length(translation) > 0.0f)
  {
    glm::normalize(translation);
    translation *= mMoveSpeed * deltaTime;
    transform.translate(translation);
  }
}