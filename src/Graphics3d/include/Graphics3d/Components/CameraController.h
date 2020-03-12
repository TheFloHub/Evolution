/**
 * @author Florian Feuerstein
 * @date 28.07.2016
 *
 */

#pragma once

#include "Component.h"

namespace g3d
{

class CameraController : public Component
{
public:
  CameraController();

  virtual ~CameraController();

  void update(double deltaTime) override;

protected:
  Component * clone() const override;

  double mMoveSpeed;

  double mRotationSpeed;

  double mRotationX;

  double mRotationY;

private:
  CameraController(CameraController const &) = delete;

  CameraController & operator=(CameraController const &) = delete;
};

} // namespace g3d
