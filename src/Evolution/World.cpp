#include "World.h"
#include "GL/glew.h"
#include "Input/InputManager.h"
#include <array>
#include <iostream>
#include <random>

using namespace std;

namespace
{
std::uniform_real_distribution<double> g_reproDis(0.0, 1.0f);
std::random_device g_rd{};
std::mt19937 g_rng{g_rd()};
} // namespace

namespace evo
{
std::vector<Apple> const & World::getApples() const { return m_apples; }
std::vector<Person> const & World::getPersons() const { return m_persons; }
std::vector<Apple> & World::getApples() { return m_apples; }
std::vector<Person> & World::getPersons() { return m_persons; }
Vector3f const & World::getSize() const { return m_terrain.getSize(); }
Terrain & World::getTerrain() { return m_terrain; }
Camera & World::getCamera() { return m_camera; }

void World::update(double const deltaTime)
{
  static double passedAppleTime = 0.0;

  // if (InputManager::getInstance().getKeyDown(KEY_DOWN))
  //{
  //  --g_numNewApples;
  //  cout << g_numNewApples << endl;
  //}

  // TODO: think about order, deltaTime is from last frame
  // people
  std::vector<size_t> died;
  std::vector<Person> born;
  for (size_t pi = 0; pi < m_persons.size(); ++pi)
  {
    auto & p = m_persons[pi];

    // movement
    p.m_movementTrait->move(p, *this, deltaTime);

    // find closest apple
    double closestAppleDistance = std::numeric_limits<double>::max();
    size_t closestAppleIndex = m_apples.size();
    for (size_t ai = 0; ai < m_apples.size(); ++ai)
    {
      double const distSq =
          (p.m_position - m_apples[ai].m_position).squaredNorm();
      if (distSq < closestAppleDistance)
      {
        closestAppleDistance = distSq;
        closestAppleIndex = ai;
      }
    }

    // eat apple
    double const irsq = p.m_interactionRange * p.m_interactionRange;
    if (p.isHungry() && closestAppleIndex < m_apples.size() &&
        closestAppleDistance < irsq)
    {
      auto const apple = std::begin(m_apples) + closestAppleIndex;
      p.m_energy += apple->m_energy;
      p.m_energy = std::min(p.m_maxEnergy, p.m_energy);
      m_apples.erase(apple);
    }

    // reproduction
    p.m_reproductionPassedTime += deltaTime;
    if (p.m_reproductionPassedTime >= p.m_reproductionTime)
    {
      p.m_reproductionPassedTime -= p.m_reproductionTime;
      if (g_reproDis(g_rng) <= p.m_reproductionProbability)
      {
        Person newP = p;
        newP.m_movementTrait->setDefault();
        newP.m_energy = newP.m_maxEnergy;
        newP.m_reproductionPassedTime = 0.0;

        // speed mutation
        std::uniform_real_distribution<double> speedMutationDis(
            0.9 * p.m_speed, 1.1 * p.m_speed);
        newP.m_speed = speedMutationDis(g_rng);

        // sensing range mutation
        std::uniform_real_distribution<double> sensingMutationDis(
            0.9 * p.m_sensingRange, 1.1 * p.m_sensingRange);
        newP.m_sensingRange = sensingMutationDis(g_rng);

        born.emplace_back(newP);
      }
    }

    // energy
    p.m_energy -= (p.m_speed /** p.m_engergySpeedLossFactor*/ +
                   p.m_energyFixedLossPerSec + p.m_sensingRange) *
                  deltaTime;

    // death
    if (p.m_energy <= 0.0)
    {
      died.emplace_back(pi);
    }
  }

  for (auto riter = std::rbegin(died); riter != std::rend(died); ++riter)
  {
    m_persons.erase(std::begin(m_persons) + (*riter));
  }
  for (auto & b : born)
  {
    m_persons.emplace_back(b);
  }

  // end people

  // apple decay
  auto appleIter = std::begin(m_apples);
  while (appleIter != std::end(m_apples))
  {
    appleIter->m_passedTime += deltaTime;
    if (appleIter->m_passedTime >= appleIter->m_lifeTime)
    {
      appleIter = m_apples.erase(appleIter);
    }
    else
    {
      ++appleIter;
    }
  }

  // new apples
  passedAppleTime += deltaTime;
  if (passedAppleTime >= g_newAppleTime)
  {
    passedAppleTime -= g_newAppleTime;
    addRandomApples(g_numNewApples);
  }

  // camera
  m_camera.update(deltaTime);
}

void World::render() const
{
  // basic lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glShadeModel(GL_SMOOTH);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  std::array<float, 4> const lp = {1, 1, 1, 0};
  glLightfv(GL_LIGHT0, GL_POSITION, lp.data());

  // init
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // camera
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMultMatrixf(m_camera.getGlProjectionMatrix().data());
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixf(m_camera.getGlWorldToCameraTrafo().data());

  // terrain
  m_terrain.render();

  // persons
  GLUquadricObj * quadric;
  quadric = gluNewQuadric();
  gluQuadricDrawStyle(quadric, GLU_FILL);
  for (auto const & p : m_persons)
  {
    glPushMatrix();
    // double const c = p.m_energy / p.m_maxEnergy;
    // double const c = p.m_speed / 40.0;
    double const c = p.m_sensingRange / 120.0;
    glColor3d(1.0, c, c);
    glTranslatef(p.m_position.x(), p.m_position.y(), p.m_position.z());
    gluSphere(quadric, 4, 18, 9);
    glPopMatrix();
  }

  // apples
  glColor3f(0.2f, 0.75f, 0.1f);
  for (auto const & a : m_apples)
  {
    glPushMatrix();
    glTranslatef(a.m_position.x(), a.m_position.y(), a.m_position.z());
    gluSphere(quadric, 2, 18, 9);
    glPopMatrix();
  }
  gluDeleteQuadric(quadric);
}

Vector3f World::getRandomPosition() const
{
  std::uniform_real_distribution<float> xdis{0.0f, getSize().x()};
  std::uniform_real_distribution<float> ydis{0.0f, getSize().y()};
  return {xdis(g_rng), ydis(g_rng), 0.0f};
}

void World::addRandomPeople(uint32_t number)
{
  for (uint32_t i = 0; i < number; ++i)
  {
    Person p;
    p.m_position = getRandomPosition();
    m_persons.emplace_back(p);
  }
}

void World::addRandomApples(uint32_t number)
{
  for (uint32_t i = 0; i < number; ++i)
  {
    Apple a;
    a.m_position = getRandomPosition();
    m_apples.emplace_back(a);
  }
}
} // namespace evo