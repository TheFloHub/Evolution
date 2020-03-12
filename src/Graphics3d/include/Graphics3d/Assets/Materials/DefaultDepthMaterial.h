/**
 * @author Florian Feuerstein
 * @date 01.08.2016
 *
 *
 */

#pragma once

#include <Graphics3d/Assets/Material.h>
#include <gl/glew.h>

namespace g3d
{

class DefaultDepthMaterial : public Material
{
public:
  DefaultDepthMaterial();

  virtual ~DefaultDepthMaterial();

  void use() const override;

protected:
  GLuint mMvpMatrixLoc;

private:
  DefaultDepthMaterial(Material const &) = delete;

  DefaultDepthMaterial & operator=(DefaultDepthMaterial const &) = delete;
};

typedef std::shared_ptr<DefaultDepthMaterial> DefaultDepthMaterialPtr;
typedef std::shared_ptr<DefaultDepthMaterial const> DefaultDepthMaterialCPtr;
} // namespace g3d
