#include "Individual.h"
#include <Graphics3d/Scene/SceneObject.h>
#include <Graphics3d\Components\Transform.h>
#include <Graphics3d\Components\MeshRenderer.h>
#include <Graphics3d\Assets\Material.h>
#include <Graphics3d\Assets\Materials\PBRMaterial.h>
#include <random>

using namespace g3d;

namespace evo
{
Individual::Individual() : Component() {}

Individual::~Individual() {}

Component * Individual::clone() const
{
  Individual * individual = new Individual();
  individual->mIsEnabled = mIsEnabled;
  individual->m_speed = m_speed;

  individual->m_sensingRange = m_sensingRange;
  individual->m_interactionRange = m_interactionRange;
  individual->m_size = m_size;
  individual->m_dirAngle = m_dirAngle;
  individual->m_vitality = m_vitality;
  individual->m_vitalityDecreasePerSec = m_vitalityDecreasePerSec;
  individual->m_reproductionTime = m_reproductionTime;
  individual->m_reproductionPassedTime = m_reproductionPassedTime;
  individual->m_apples = m_apples;
  individual->m_individuals = m_individuals;

  return individual;
}

void Individual::update(double deltaTime)
{
  static std::random_device randomDevice;
  static std::mt19937 rng(randomDevice());
  static std::uniform_real_distribution<float> angleDis(-0.3f, 0.3f);

  float const terrainSize = 10.0f;

  // vitality and color
  m_vitality -= m_vitalityDecreasePerSec * deltaTime;
  PBRMaterialPtr material = std::static_pointer_cast<PBRMaterial>(
      mpSceneObject->getComponent<MeshRenderer>()->getMaterial());
  float const c = static_cast<float>(m_vitality / 200.0);
  material->setAlbedo(c, c, c);

  // movement
  m_dirAngle += angleDis(rng);
  float const x =
      static_cast<float>(deltaTime * m_speed * std::cos(m_dirAngle));
  float const z =
      static_cast<float>(deltaTime * m_speed * std::sin(m_dirAngle));

  Transform & transform = *(mpSceneObject->getTransform());
  glm::mat4 const localToWorldMatrix = transform.getLocalToWorldMatrix();
  glm::vec3 const worldPosition(
      std::clamp(localToWorldMatrix[3][0] + x, 0.0f, terrainSize),
      localToWorldMatrix[3][1],
      std::clamp(localToWorldMatrix[3][2] + z, 0.0f, terrainSize));
  transform.setPosition(worldPosition);

  // apples
  size_t const nc = m_apples->getNumberOfChildren();
  float closestAppleDistance = std::numeric_limits<float>::max();
  float const irsq = m_interactionRange * m_interactionRange;
  SceneObject * closestApple = nullptr;
  for (size_t i = 0; i < nc; ++i)
  {
    auto const apple = m_apples->getChild(i);
    auto const l2wm = apple->getTransform()->getLocalToWorldMatrix();
    glm::vec3 const pos(l2wm[3][0], l2wm[3][1], l2wm[3][2]);
    float dx = pos.x - worldPosition.x;
    float dz = pos.z - worldPosition.z;
    float distSq = dx * dx + dz * dz;
    if (distSq <= irsq && distSq < closestAppleDistance)
    {
      closestAppleDistance = distSq;
      closestApple = apple;
    }
  }

  if (closestApple)
  {
    m_apples->removeChild(closestApple);
    delete closestApple;
    closestApple = nullptr;
    m_vitality += 100.0;
  }


  // reproduction
  //m_reproductionPassedTime += deltaTime;
  //if (m_reproductionPassedTime >= m_reproductionTime)
  //{
  //  m_reproductionPassedTime -= m_reproductionTime;
  //  SceneObject::instantiate(mpSceneObject->clone(), m_individuals);
  //}

  // death
  if (m_vitality <= 0.0)
  {
    SceneObject::destroy(mpSceneObject);
  }
}
void Individual::setApples(g3d::SceneObject * apples) { m_apples = apples; }
void Individual::setIndividuals(g3d::SceneObject * individuals)
{
  m_individuals = individuals;
}

} // namespace evo