#include "Movement.h"
#include "EvoSim.h"

namespace
{
std::uniform_real_distribution<double> g_angleDis(0, 6.2831853);
}

namespace evo
{
std::unique_ptr<MovementTrait> NormalMovement::clone() const
{
  NormalMovement * mt = new NormalMovement;
  *mt = *this;
  return std::unique_ptr<MovementTrait>(mt);
}

void NormalMovement::setDefault() { *this = NormalMovement(); }

void NormalMovement::move(Person & p, double const deltaTime)
{
  auto & evoSim = EvoSim::get();
  auto const & apples = evoSim.m_apples;

  // find closest apple
  double closestAppleDistance = std::numeric_limits<double>::max();
  size_t closestAppleIndex = apples.size();
  for (size_t ai = 0; ai < apples.size(); ++ai)
  {
    double const distSq = (p.m_position - apples[ai].m_position).squaredNorm();
    if (distSq < closestAppleDistance)
    {
      closestAppleDistance = distSq;
      closestAppleIndex = ai;
    }
  }

  // movement
  double mdx = 0.0;
  double mdy = 0.0;
  double const srsq = p.m_sensingRange * p.m_sensingRange;
  if (p.isHungry() && closestAppleDistance < srsq)
  {
    auto const apple = std::begin(apples) + closestAppleIndex;
    math::Vector2d const dxy = (apple->m_position - p.m_position).normalized();
    mdx = dxy.x();
    mdy = dxy.y();
  }
  else
  {
    m_angleChangePassedTime += deltaTime;
    if (m_angleChangePassedTime >= m_angleChangeTime)
    {
      m_angleChangePassedTime -= m_angleChangeTime;
      m_dirAngle = g_angleDis(evoSim.m_rng);
    }

    mdx = std::cos(m_dirAngle);
    mdy = std::sin(m_dirAngle);
  }

  p.m_position.x() += deltaTime * p.m_speed * mdx;
  p.m_position.y() += deltaTime * p.m_speed * mdy;
  p.m_position.x() =
      std::clamp(p.m_position.x(), 0.0, static_cast<double>(evoSim.m_width));
  p.m_position.y() =
      std::clamp(p.m_position.y(), static_cast<double>(evoSim.m_yStart),
                 static_cast<double>(evoSim.m_height));
}
} // namespace evo