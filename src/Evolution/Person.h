#pragma once
#include <Math/MathTypes.h>
#include "Movement.h"

namespace evo
{

uint32_t const g_startPopulation = 20;

struct Person
{
  Person() = default;
  Person(Person const & other);
  Person & operator=(const Person & other);
  bool isHungry() const;


  math::Vector2d m_position{0.0, 0.0};

  // TODO: person might eat other person and get some of their energy (factor /
  // digestion efficiency)

  // TODO: speed should be quadratic for energy loss
  double m_speed{20.0};
  double m_sensingRange{60.0};
  double m_interactionRange{20.0};
  // double m_size{1.0};
  // size will increase the m_energyFixedLossPerSec and
  // m_engergySpeedLossFactor, maybe another members though

  double m_maxEnergy{600.0};
  double m_energy{m_maxEnergy};
  double m_energyFixedLossPerSec{50.0}; // 18 was stable
  //double m_engergySpeedLossFactor{1.0};

  // double m_vitality{200.0};
  // double m_vitalityDecreasePerSec{10};

  double m_reproductionProbability{0.1};
  double m_reproductionTime{1.0};
  double m_reproductionPassedTime{0.0};

  std::unique_ptr<MovementTrait> m_movementTrait{new NormalMovement};
};
} // namespace evo