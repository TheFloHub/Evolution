#pragma once
#include "Math/MathTypes.h"

namespace evo
{

class Camera final
{
public:
  void update(double deltaTime);
  void setWindowSize(uint32_t width, uint32_t height);
  void setDistance(float distance);
  void setFocus(Vector3f const& focus);

  Transform3f const & getCameraToWorldTrafo() const;
  Transform3f const & getWorldToCameraTrafo() const;

  Matrix4f const & getProjectionMatrix() const;
  Matrix4f const & getFullProjectionMatrix() const;


private:
  void startRotationDrag(Vector2f const & point);
  void rotationDrag(Vector2f const & point);
  void mapToSphere(Vector2f const & point, Vector3f & vector);
  void updateMatrices();

  uint32_t m_windowWidth{100};
  uint32_t m_windowHeight{100};
  float m_fovY{70.0f};
  float m_nearPlane{10.0f};
  float m_farPlane{2000.0f};
  Vector3f m_focus{0.0f, 0.0f, 0.0f};
  float m_distance{0.0f};
  Transform3f m_lastRotation{Transform3f::Identity()};
  Transform3f m_currentRotation{Transform3f::Identity()};
  Vector3f m_startVector{0.0f, 0.0f, 0.0f};
  Vector3f m_endVector{0.0f, 0.0f, 0.0f};
  float m_adjustWidth{1.0f};
  float m_adjustHeight{1.0f};
  float m_scrollSpeed{10.0f};
  Transform3f m_cameraToWorldTrafo{Transform3f::Identity()};
  Transform3f m_worldToCameraTrafo{Transform3f::Identity()};
  Matrix4f m_projection{Matrix4f::Identity()};
  Matrix4f m_fullProjection{Matrix4f::Identity()};
};
} // namespace evo