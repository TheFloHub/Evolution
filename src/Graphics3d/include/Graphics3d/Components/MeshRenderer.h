/**
 * @author Florian Feuerstein
 * @date 01.08.2016
 *
 * A mesh renderer component renders a mesh with a certain material.
 */

#pragma once

#include "Component.h"
#include <Graphics3d/SharedPtrTypes.h>

namespace g3d
{

class MeshRenderer : public Component
{
public:
  MeshRenderer(MeshPtr pMesh, MaterialPtr pMaterial,
               MaterialPtr pDepthMaterial);

  MeshRenderer(MeshPtr pMesh, MaterialPtr pMaterial);

  virtual ~MeshRenderer();

  void render() const override;

  void renderDepth() const override;

  void setMaterial(MaterialPtr pNewMaterial);

  MaterialPtr getMaterial() const;

protected:
  Component * clone() const override;

  MeshPtr mpMesh;

  MaterialPtr mpMaterial;

  MaterialPtr mpDepthMaterial;

private:
  MeshRenderer(MeshRenderer const &) = delete;

  MeshRenderer & operator=(MeshRenderer const &) = delete;
};

} // namespace g3d
