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
  if (width > 0 && height > 0)
  {
    m_adjustWidth = 2.0f / width;
    m_adjustHeight = 2.0f / height;
  }
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
  mapToSphere(point, m_endVector);
  Vector3f const axis = m_startVector.cross(m_endVector).normalized();
  Eigen::AngleAxisf aa(std::acos(m_startVector.dot(m_endVector)), axis);
  Transform3f newRotation = Transform3f::Identity();
  newRotation.linear() = aa.toRotationMatrix();
  m_currentRotation = newRotation * m_lastRotation;
}

void Camera::mapToSphere(Vector2f const & point, Vector3f & vector) {
  // adjust point coords and scale down to range of [-1 ... 1]
  Vector2f const tempPoint(1.0f - (point.x() * m_adjustWidth),
                           1.0f - (point.y() * m_adjustHeight));
  float const squaredNorm = tempPoint.squaredNorm();
  if (squaredNorm > 1.0f)
  {
    float const norm = 1.0f / std::sqrt(squaredNorm);
    vector.x() = tempPoint.x() * norm;
    vector.y() = tempPoint.y() * norm;
    vector.z() = 0.0f;
  }
  else 
  {
    vector.x() = tempPoint.x();
    vector.y() = tempPoint.y();
    vector.z() = -std::sqrt(1.0f - squaredNorm);
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
  glRotatef(180, 0, 1, 0);
  // distance from camera to object
  glTranslatef(0, 0, m_distance);
  // arc ball rotation matrix
  glMultMatrixf(m_currentRotation.data());
  // move focus to zero point
  glTranslatef(-m_focus.x(), -m_focus.y(), -m_focus.z());
}
} // namespace evo