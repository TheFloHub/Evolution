#include "Graphics3d/Components/MeshRenderer.h"
#include <Graphics3d/Assets/Material.h>
#include <Graphics3d/Assets/MaterialManager.h>
#include <Graphics3d/Assets/Mesh.h>

G3d::MeshRenderer::MeshRenderer(MeshCPtr pMesh, MaterialCPtr pMaterial,
                                MaterialCPtr pDepthMaterial)
    : Component(), mpMesh(pMesh), mpMaterial(pMaterial),
      mpDepthMaterial(pDepthMaterial)
{
}

G3d::MeshRenderer::MeshRenderer(MeshCPtr pMesh, MaterialCPtr pMaterial)
    : Component(), mpMesh(pMesh), mpMaterial(pMaterial),
      mpDepthMaterial(MaterialManager::getInstance().getDefaultDepthMaterial())
{
}

G3d::MeshRenderer::~MeshRenderer() {}

G3d::Component * G3d::MeshRenderer::clone() const
{
  MeshRenderer * pMeshRenderer =
      new MeshRenderer(mpMesh, mpMaterial, mpDepthMaterial);
  pMeshRenderer->mIsEnabled = mIsEnabled;
  pMeshRenderer->mpMesh = mpMesh;
  pMeshRenderer->mpMaterial = mpMaterial;
  pMeshRenderer->mpDepthMaterial = mpDepthMaterial;
  return pMeshRenderer;
}

void G3d::MeshRenderer::render() const
{
  mpMaterial->use();
  mpMesh->render();
}

void G3d::MeshRenderer::renderDepth() const
{
  mpDepthMaterial->use();
  mpMesh->render();
}

void G3d::MeshRenderer::setMaterial(MaterialCPtr pNewMaterial)
{
  mpMaterial = pNewMaterial;
}