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
  updateMatrices();
}

void Camera::setDistance(float distance)
{
  m_distance = distance;
  updateMatrices();
}

void Camera::setFocus(Vector3f const & focus)
{
  m_focus = focus;
  updateMatrices();
}

void Camera::update(double /*deltaTime*/)
{
  auto & input = InputManager::getInstance();
  if (input.getMouseButtonDown(MOUSE_BUTTON_3))
  {
    startRotationDrag(input.getMousePosition().cast<float>());
  }
  if (input.getMouseButton(MOUSE_BUTTON_3))
  {
    rotationDrag(input.getMousePosition().cast<float>());
  }
  if (input.getMouseButtonDown(MOUSE_BUTTON_2))
  {
    startTranslationDrag(input.getMousePosition().cast<float>());
  }
  if (input.getMouseButton(MOUSE_BUTTON_2))
  {
    translationDrag(input.getMousePosition().cast<float>());
  }

  m_distance += m_scrollSpeed * static_cast<float>(input.getMouseWheelDeltaY());
  m_distance = std::max(m_distance, 10.0f);
  updateMatrices();
}

void Camera::startTranslationDrag(Vector2f const & point)
{
  m_lastFocus = m_focus;
  mapToPlane(point, m_startTranslationPoint);
}

void Camera::translationDrag(Vector2f const & point)
{
  Vector3f endPoint{0.0f, 0.0f, 0.0f};
  mapToPlane(point, endPoint);
  m_focus = m_lastFocus + m_startTranslationPoint - endPoint;
}

void Camera::mapToPlane(Vector2f const & point, Vector3f & planePoint)
{
  // camera matrix
  Matrix3f cm = Matrix3f::Identity();
  cm(0, 0) = -static_cast<float>(m_windowHeight) /
             (2.0f * std::tan(0.0174533f * 0.5f * m_fovY));
  cm(1, 1) = cm(0, 0);
  cm(0, 2) = static_cast<float>(m_windowWidth) / 2.0f;
  cm(1, 2) = static_cast<float>(m_windowHeight) / 2.0f;

  // world to camera trafo
  Transform3f td = Transform3f::Identity();
  td.translation() = Vector3f(0.0f, 0.0f, m_distance);
  Transform3f tf = Transform3f::Identity();
  tf.translation() = -m_lastFocus;

  Transform3f const w2c = td * m_rotation * tf;
  Transform3f const c2w = w2c.inverse();

  Matrix3f const icm = (cm * w2c.linear()).inverse();

  Vector3f const ray = (icm * Vector3f(point.x(), point.y(), 1.0f)).normalized();
  Vector3f const cc = c2w.translation();
  Vector3f const pn(0.0f, 0.0f, 1.0f);

  float const t = (-pn.dot(cc)) / (pn.dot(ray));
  planePoint = cc + t * ray;
}

void Camera::startRotationDrag(Vector2f const & point)
{
  m_lastRotation = m_rotation;
  mapToSphere(point, m_startRotationVector);
}
void Camera::rotationDrag(Vector2f const & point)
{
  Vector3f endVector{0.0, 0.0, 0.0};
  mapToSphere(point, endVector);
  Vector3f const axis = m_startRotationVector.cross(endVector).normalized();
  float const dot =
      std::clamp(m_startRotationVector.dot(endVector), -1.0f, 1.0f);
  float const angle = std::acos(dot);
  Eigen::AngleAxisf aa(angle, axis);
  Transform3f newRotation = Transform3f::Identity();
  newRotation.linear() = aa.toRotationMatrix();
  m_rotation = newRotation * m_lastRotation;
}

void Camera::mapToSphere(Vector2f const & point, Vector3f & vector)
{
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



Transform3f const & Camera::getGlWorldToCameraTrafo() const
{
  return m_glWorldToCameraTrafo;
}

Matrix4f const & Camera::getGlProjectionMatrix() const
{
  return m_glProjection;
}


void Camera::updateMatrices()
{
  // transformation
  Transform3f m180 = Transform3f::Identity();
  m180(0, 0) = -1;
  m180(2, 2) = -1;
  Transform3f td = Transform3f::Identity();
  td.translation() = Vector3f(0.0f, 0.0f, m_distance);
  Transform3f tf = Transform3f::Identity();
  tf.translation() = -m_focus;
  m_glWorldToCameraTrafo = m180 * td * m_rotation * tf;

  // projection
  float const f = 1.0f / std::tan(0.0174533f * m_fovY / 2.0f);
  float const aspect =
      static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight);
  m_glProjection = Matrix4f::Zero();
  m_glProjection(0, 0) = f / aspect;
  m_glProjection(1, 1) = f;
  m_glProjection(2, 2) = (m_farPlane + m_nearPlane) / (m_nearPlane - m_farPlane);
  m_glProjection(2, 3) =
      (2.0f * m_farPlane * m_nearPlane) / (m_nearPlane - m_farPlane);
  m_glProjection(3, 2) = -1.0;
}

} // namespace evo