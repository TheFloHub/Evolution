/**
 * @author Florian Feuerstein
 * @date 01.08.2016
 *
 * The material class is the base class
 * for each specific material. A material
 * calls a shader with certain values.
 */

#pragma once

#include <Graphics3d/SharedPtrTypes.h>

namespace G3d
{

class Material
{
public:
  Material(ShaderCPtr pShader);

  virtual ~Material();

  virtual void use() const;

  ShaderCPtr getShader() const;

protected:
  ShaderCPtr mpShader;

private:
  Material(Material const &) = delete;

  Material & operator=(Material const &) = delete;
};

} // namespace G3d
