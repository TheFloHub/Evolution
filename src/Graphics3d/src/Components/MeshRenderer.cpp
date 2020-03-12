#include "Graphics3d/Components/MeshRenderer.h"
#include <Graphics3d/Assets/Material.h>
#include <Graphics3d/Assets/MaterialManager.h>
#include <Graphics3d/Assets/Mesh.h>

g3d::MeshRenderer::MeshRenderer(MeshPtr pMesh, MaterialPtr pMaterial,
                                MaterialPtr pDepthMaterial)
    : Component(), mpMesh(pMesh), mpMaterial(pMaterial),
      mpDepthMaterial(pDepthMaterial)
{
}

g3d::MeshRenderer::MeshRenderer(MeshPtr pMesh, MaterialPtr pMaterial)
    : Component(), mpMesh(pMesh), mpMaterial(pMaterial),
      mpDepthMaterial(MaterialManager::getInstance().getDefaultDepthMaterial())
{
}

g3d::MeshRenderer::~MeshRenderer() {}

g3d::Component * g3d::MeshRenderer::clone() const
{
  MeshRenderer * pMeshRenderer =
      new MeshRenderer(mpMesh, mpMaterial, mpDepthMaterial);
  pMeshRenderer->mIsEnabled = mIsEnabled;
  pMeshRenderer->mpMesh = mpMesh;
  pMeshRenderer->mpMaterial = mpMaterial;
  pMeshRenderer->mpDepthMaterial = mpDepthMaterial;
  return pMeshRenderer;
}

void g3d::MeshRenderer::render() const
{
  mpMaterial->use();
  mpMesh->render();
}

void g3d::MeshRenderer::renderDepth() const
{
  mpDepthMaterial->use();
  mpMesh->render();
}

void g3d::MeshRenderer::setMaterial(MaterialPtr pNewMaterial)
{
  mpMaterial = pNewMaterial;
}

g3d::MaterialPtr g3d::MeshRenderer::getMaterial() const { return mpMaterial; }