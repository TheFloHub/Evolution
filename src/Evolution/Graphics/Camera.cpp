#include "Graphics/Camera.h"
#include "Input/InputManager.h"
#include "gl/glew.h"
#include <iostream>

using namespace std;

namespace evo
{
void Camera::setWindowSize(uint32_t width, uint32_t height)
{
  m_windowHeight = height;
  m_windowWidth = width;
  m_adjustWidth = 1.0f / ((width - 1.0f) * 0.5f);
  m_adjustHeight = 1.0f / ((height - 1.0f) * 0.5f);
}


void Camera::setFocus(Vector3f const & focus) { m_focus = focus; }
void Camera::setDistance(float distance) { m_distance = distance; }

void Camera::update(double /*deltaTime*/)
{
  auto & input = InputManager::getInstance();
  if (input.getMouseButtonDown(MOUSE_BUTTON_1))
  {
    startDrag(input.getMousePosition().cast<float>());
  }
  if (input.getMouseButton(MOUSE_BUTTON_1))
  {
    drag(input.getMousePosition().cast<float>());
  }
}

void Camera::startDrag(Vector2f const & point)
{
  m_lastRotation = m_currentRotation;
  mapToSphere(point, m_startVector);
}
void Camera::drag(Vector2f const & point)
{
  Transform3f thisQuat = Transform3f::Identity();
  drag(point, thisQuat); // Update End Vector And Get Rotation As Quaternion
  m_currentRotation = thisQuat;
  m_currentRotation = m_currentRotation * m_lastRotation;
}

void Camera::drag(Vector2f const & point, Transform3f & newRot)
{
  mapToSphere(point, m_endVector);
    Vector3f const Perp = m_startVector.cross(m_endVector).normalized();
  Eigen::AngleAxisf aa(std::acos(m_startVector.dot(m_endVector)), Perp);
    newRot = aa.matrix();
}

void Camera::mapToSphere(Vector2f const & point, Vector3f & vector) {
  // Copy paramter into temp point
  Vector2f tempPoint = point;

  // Adjust point coords and scale down to range of [-1 ... 1]
  tempPoint.x() = (tempPoint.x() * m_adjustWidth) - 1.0f;
  tempPoint.y() = 1.0f - (tempPoint.y() * m_adjustHeight);

  // Compute the square of the length of the vector to the point from the center
  float length =
      (tempPoint.x() * tempPoint.x()) + (tempPoint.y() * tempPoint.y());

  // If the point is mapped outside of the sphere... (length > radius squared)
  if (length > 1.0f)
  {
    // Compute a normalizing factor (radius / sqrt(length))
    float const norm = 1.0f / std::sqrt(length);

    // Return the "normalized" vector, a point on the sphere
    vector.x() = tempPoint.x() * norm;
    vector.y() = tempPoint.y() * norm;
    vector.z() = 0.0f;
  }
  else // Else it's on the inside
  {
    // Return a vector to a point mapped inside the sphere sqrt(radius squared -
    // length)
    vector.x() = tempPoint.x();
    vector.y() = tempPoint.y();
    vector.z() = std::sqrt(1.0f - length);
  }
}


void Camera::applyMatrix() const
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(m_fovY,
                 static_cast<double>(m_windowWidth) /
                     static_cast<double>(m_windowHeight),
                 m_nearPlane, m_farPlane);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  /*gluLookAt(800 / 2, 800, 800 / 2, 800 / 2, 0, 800 / 2, 0, 0, 1);*/
  // distance from camera to object
  glTranslatef(0, 0, -m_distance);
  // arc ball rotation matrix
  glMultMatrixf(m_currentRotation.data());
  // move focus to zero point
  glTranslatef(-m_focus.x(), -m_focus.y(), -m_focus.z());
}
} // namespace evo