#include "Graphics3d/Components/Camera.h"
#pragma warning(disable : 4201)
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

g3d::Camera::Camera(unsigned int width, unsigned int height, float nearPlane,
                    float farPlane, float fov)
    : Component(), mWidth(width), mHeight(height), mNearPlane(nearPlane),
      mFarPlane(farPlane), mFov(fov),
      mProjectionMatrix(glm::perspectiveFov(glm::radians(mFov), (float)mWidth,
                                            (float)mHeight, mNearPlane,
                                            mFarPlane)),
      mInverseProjectionMatrix(glm::inverse(mProjectionMatrix))
{
}

g3d::Camera::~Camera() {}

g3d::Component * g3d::Camera::clone() const
{
  Camera * pCamera = new Camera(mWidth, mHeight, mNearPlane, mFarPlane, mFov);
  pCamera->mIsEnabled = mIsEnabled;
  pCamera->mWidth = mWidth;
  pCamera->mHeight = mHeight;
  pCamera->mNearPlane = mNearPlane;
  pCamera->mFarPlane = mFarPlane;
  pCamera->mFov = mFov;
  pCamera->mProjectionMatrix = mProjectionMatrix;
  pCamera->mInverseProjectionMatrix = mInverseProjectionMatrix;
  return pCamera;
}

glm::mat4 const & g3d::Camera::getProjectionMatrix() const
{
  return mProjectionMatrix;
}

glm::mat4 const & g3d::Camera::getInverseProjectionMatrix() const
{
  return mInverseProjectionMatrix;
}

void g3d::Camera::setImageSize(unsigned int width, unsigned int height)
{
  if (mWidth != width || mHeight != height)
  {
    mWidth = width;
    mHeight = height;
    mProjectionMatrix =
        glm::perspectiveFov(glm::radians(mFov), (float)mWidth, (float)mHeight,
                            mNearPlane, mFarPlane);
    mInverseProjectionMatrix = glm::inverse(mProjectionMatrix);
  }
}

float g3d::Camera::getNearPlane() const { return mNearPlane; }

float g3d::Camera::getFarPlane() const { return mFarPlane; }
