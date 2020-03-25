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

  Transform3f const & getGlWorldToCameraTrafo() const;
  Matrix4f const & getGlProjectionMatrix() const;


private:
  void startRotationDrag(Vector2f const & point);
  void rotationDrag(Vector2f const & point);
  void mapToSphere(Vector2f const & point, Vector3f & vector);

  void startTranslationDrag(Vector2f const & point);
  void translationDrag(Vector2f const & point);
  void mapToPlane(Vector2f const & point, Vector3f & planePoint);

  void updateMatrices();

  uint32_t m_windowWidth{100};
  uint32_t m_windowHeight{100};

  float m_fovY{70.0f};
  float m_nearPlane{10.0f};
  float m_farPlane{2000.0f};

  Vector3f m_focus{0.0f, 0.0f, 0.0f};
  Transform3f m_rotation{Transform3f::Identity()};
  float m_distance{0.0f};

  Transform3f m_lastRotation{Transform3f::Identity()};
  Vector3f m_startRotationVector{0.0f, 0.0f, 0.0f};
  float m_adjustWidth{1.0f};
  float m_adjustHeight{1.0f};

  float m_scrollSpeed{10.0f};

  Vector3f m_startTranslationPoint{0.0f, 0.0f, 0.0f};
  Vector3f m_lastFocus{0.0f, 0.0f, 0.0f};

  Transform3f m_glWorldToCameraTrafo{Transform3f::Identity()};
  Matrix4f m_glProjection{Matrix4f::Identity()};
};
} // namespace evo