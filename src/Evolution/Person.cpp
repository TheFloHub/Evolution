#include "Person.h"

namespace evo
{
Person::Person(const Person & other) { operator=(other); }

Person & Person::operator=(const Person & other)
{
  m_position = other.m_position;
  m_speed = other.m_speed;
  m_sensingRange = other.m_sensingRange;
  m_interactionRange = other.m_interactionRange;
  m_maxEnergy = other.m_maxEnergy;
  m_energy = other.m_energy;
  m_energyFixedLossPerSec = other.m_energyFixedLossPerSec;

  //m_engergySpeedLossFactor = other.m_engergySpeedLossFactor;
  m_reproductionProbability = other.m_reproductionProbability;
  m_reproductionTime = other.m_reproductionTime;
  m_reproductionPassedTime = other.m_reproductionPassedTime;
  m_movementTrait = other.m_movementTrait->clone();
  return *this;
}
bool Person::isHungry() const { return m_energy < m_maxEnergy / 2.0; }

} // namespace evo