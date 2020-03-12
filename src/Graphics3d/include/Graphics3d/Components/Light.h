/**
 * @author Florian Feuerstein
 * @date 02.08.2016
 *
 * A light. That's pretty much it.
 */

#pragma once

#include "Component.h"
#include <glm/vec3.hpp>
#include <vector>

namespace g3d
{

class Light : public Component
{
public:
  enum Type
  {
    DIRECTIONAL = 0,
    POINT = 1
  };

  Light(g3d::Light::Type type);

  virtual ~Light();

  g3d::Light::Type getType() const;

  void setType(g3d::Light::Type type);

  glm::vec3 const & getColor() const;

  void setColor(glm::vec3 const & color);

  float getIntensity() const;

  void setIntensity(float intensity);

  bool isDirectionalLight() const;

  bool isPointLight() const;

protected:
  Component * clone() const override;

  Type mType;

  glm::vec3 mColor;

  float mIntensity;

private:
  Light(Light const &) = delete;

  Light & operator=(Light const &) = delete;
};

} // namespace g3d
