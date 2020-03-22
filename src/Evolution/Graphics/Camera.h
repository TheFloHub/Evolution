#pragma once
#include "Math/MathTypes.h"

namespace evo
{

class Camera final
{
public:
  void update(double deltaTime);
  void applyMatrix() const;
  void setWindowSize(uint32_t width, uint32_t height);
  void setFocus(Vector3f const & focus);
  void setDistance(float distance);

private:
  void startDrag(Vector2f const & point);
  void drag(Vector2f const & point);
  void mapToSphere(Vector2f const & point, Vector3f & vector);

  uint32_t m_windowWidth{100};
  uint32_t m_windowHeight{100};
  float m_fovY{60.0f};
  float m_nearPlane{10.0f};
  float m_farPlane{1000.0f};
  Vector3f m_focus{0.0f, 0.0f, 0.0f};
  float m_distance{0.0f};
  Transform3f m_lastRotation{Transform3f::Identity()};
  Transform3f m_currentRotation{Transform3f::Identity()};
  Vector3f m_startVector{0.0f, 0.0f, 0.0f};
  Vector3f m_endVector{0.0f, 0.0f, 0.0f};
  float m_adjustWidth{1.0f};
  float m_adjustHeight{1.0f};
};
} // namespace evo