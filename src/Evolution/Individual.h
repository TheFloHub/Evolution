#include <Graphics3d/Components/Component.h>

#include <Graphics3d/Scene/SceneObject.h>
namespace evo
{

class Individual : public g3d::Component
{
public:
  Individual();

  virtual ~Individual();

  void update(double deltaTime) override;
  void setApples(g3d::SceneObject * apples);
  void setIndividuals(g3d::SceneObject * individuals);

protected:
  double m_speed{0.25};
  float m_sensingRange{10.0f};
  float m_interactionRange{1.0f};
  float m_size{1.0f};
  double m_dirAngle{0.0};
  double m_vitality{200.0};
  double m_vitalityDecreasePerSec{10};
  double m_reproductionTime{10.0};
  double m_reproductionPassedTime{0.0};
  g3d::SceneObject * m_apples{nullptr};
  g3d::SceneObject * m_individuals{nullptr};
  
  Component * clone() const override;

private:
  Individual(Individual const &) = delete;

  Individual & operator=(Individual const &) = delete;
};
} // namespace evo