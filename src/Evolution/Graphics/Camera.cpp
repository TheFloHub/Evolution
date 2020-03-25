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


  m_distance += m_scrollSpeed * static_cast<float>(input.getMouseWheelDeltaY());
  updateMatrices();
}

void Camera::startTranslationDrag(Vector2f const & point)
{
    // camera matrix
  Matrix3f cm = Matrix3f::Identity();
  cm(0, 0) = -static_cast<float>(m_windowHeight) /
               (2.0f * std::tan(0.0174533f * 0.5f * m_fovY));
  cm(1, 1) = cm(0, 0);
  cm(0, 2) = m_windowWidth / 2.0f;
  cm(1, 2) = m_windowHeight / 2.0f;

  // world to camera trafo
  Transform3f td = Transform3f::Identity();
  td.translation() = Vector3f(0.0f, 0.0f, m_distance);
  Transform3f tf = Transform3f::Identity();
  tf.translation() = -m_focus;

  Transform3f w2c = td * m_currentRotation * tf;

  Matrix3f icm = (cm * w2c.linear()).inverse();

  Vector3f const ray = (icm * Vector3f(point.x(), point.y(), 1.0f)).normalized();
  Vector3f const cc = m_cameraToWorldTrafo.translation();
  Vector3f const pn(0.0f, 0.0f, 1.0f);

  float const t = (-pn.dot(cc)) / (pn.dot(ray));
  Vector3f const p3d = cc + t * ray;
  cout << p3d.transpose() << endl;
  
}

void Camera::startRotationDrag(Vector2f const & point)
{
  m_lastRotation = m_currentRotation;
  mapToSphere(point, m_startVector);
}
void Camera::rotationDrag(Vector2f const & point)
{
  mapToSphere(point, m_endVector);
  Vector3f const axis = m_startVector.cross(m_endVector).normalized();
  Eigen::AngleAxisf aa(std::acos(m_startVector.dot(m_endVector)), axis);
  Transform3f newRotation = Transform3f::Identity();
  newRotation.linear() = aa.toRotationMatrix();
  m_currentRotation = newRotation * m_lastRotation;
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

Transform3f const & Camera::getCameraToWorldTrafo() const
{
  return m_cameraToWorldTrafo;
}

Transform3f const & Camera::getWorldToCameraTrafo() const
{
  return m_worldToCameraTrafo;
}

Matrix4f const & Camera::getProjectionMatrix() const { return m_projection; }
Matrix4f const & Camera::getFullProjectionMatrix() const
{
  return m_fullProjection;
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

  m_worldToCameraTrafo = m180 * td * m_currentRotation * tf;
  m_cameraToWorldTrafo = m_worldToCameraTrafo.inverse();

  // projection
  float const f = 1.0f / std::tan(0.0174533f * m_fovY / 2.0f);
  float const aspect =
      static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight);
  m_projection = Matrix4f::Zero();
  m_projection(0, 0) = f / aspect;
  m_projection(1, 1) = f;
  m_projection(2, 2) = (m_farPlane + m_nearPlane) / (m_nearPlane - m_farPlane);
  m_projection(2, 3) =
      (2.0f * m_farPlane * m_nearPlane) / (m_nearPlane - m_farPlane);
  m_projection(3, 2) = -1.0;

  // full
  m_fullProjection = m_projection * m_worldToCameraTrafo.matrix();

  Matrix3f cm = Matrix3f::Identity();
  cm(0, 0) = 2.0f / m_windowWidth;
  cm(1, 1) = 2.0f / m_windowWidth;
  cm(0, 2) = m_windowWidth / 2.0f;
  cm(1, 2) = m_windowHeight / 2.0f;

  m_invCam = (cm * m_worldToCameraTrafo.linear()).inverse();
}

} // namespace evo