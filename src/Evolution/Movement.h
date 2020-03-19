#pragma once
#include <memory>

namespace evo
{

struct Person;

// TODO: make the person a member?
// and add evosim as a person member? or even better world as member
// world contains its dimensions and all living beeings etcc..
// World pointer is automatically passed over to children by cloning
struct MovementTrait
{
  virtual ~MovementTrait() {}

  // TODO: distinguish between perfect clone and create new from same type with
  // default values!
  virtual std::unique_ptr<MovementTrait> clone() const = 0;
  virtual void setDefault() = 0;
  virtual void move(Person & p, double const deltaTime) = 0;
};

struct NormalMovement : public MovementTrait
{
  std::unique_ptr<MovementTrait> clone() const override;
  void setDefault() override;
  void move(Person & p, double const deltaTime) override;

  double m_dirAngle{0.0};
  double m_angleChangeTime{2.0};
  double m_angleChangePassedTime{2.0};
};

} // namespace evo