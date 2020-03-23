#include "Movement.h"
#include "World.h"
#include <random>

namespace
{
std::random_device g_rd{};
std::mt19937 g_rng{g_rd()};
std::uniform_real_distribution<double> g_angleDis(0, 6.2831853);
} // namespace

namespace evo
{
std::unique_ptr<MovementTrait> NormalMovement::clone() const
{
  return std::unique_ptr<MovementTrait>(new NormalMovement(*this));
}

void NormalMovement::setDefault() { *this = NormalMovement(); }

void NormalMovement::move(Person & p, World const & world,
                          double const deltaTime)
{
  auto const & apples = world.getApples();

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
    Vector3f const dxy = (apple->m_position - p.m_position).normalized();
    mdx = dxy.x();
    mdy = dxy.y();
  }
  else
  {
    m_angleChangePassedTime += deltaTime;
    if (m_angleChangePassedTime >= m_angleChangeTime)
    {
      m_angleChangePassedTime -= m_angleChangeTime;
      m_dirAngle = g_angleDis(g_rng);
    }

    mdx = std::cos(m_dirAngle);
    mdy = std::sin(m_dirAngle);
  }

  p.m_position.x() += static_cast<float>(deltaTime * p.m_speed * mdx);
  p.m_position.y() += static_cast<float>(deltaTime * p.m_speed * mdy);
  auto const & worldSize = world.getSize();
  p.m_position.x() = std::clamp(p.m_position.x(), 0.0f, worldSize.x());
  p.m_position.y() = std::clamp(p.m_position.y(), 0.0f, worldSize.y());
}
} // namespace evo