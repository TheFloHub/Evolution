#include "Graphics3d/Rendering/RenderPaths/FinalShadingPath.h"
#include <Graphics3d/Assets/Mesh.h>
#include <Graphics3d/Assets/MeshManager.h>
#include <Graphics3d/Assets/Shader.h>
#include <Graphics3d/Assets/ShaderManager.h>
#include <Graphics3d/Assets/Texture.h>

g3d::FinalShadingPass::FinalShadingPass(unsigned int width, unsigned int height)
    : RenderPath(ShaderManager::getInstance().get("FinalShading"), nullptr),
      mWidth(width), mHeight(height), mpInput()
{
  mpShader->use();
  glUniform1i(mpShader->getUniformLocation("inputTexture"), 0);
}

g3d::FinalShadingPass::~FinalShadingPass() {}

void g3d::FinalShadingPass::render()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, mWidth, mHeight);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glActiveTexture(GL_TEXTURE0);
  mpInput->use();

  mpShader->use();
  MeshManager::getInstance().getScreenAlignedQuad()->render();

  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
}

void g3d::FinalShadingPass::setInput(TextureCPtr pTexture)
{
  mpInput = pTexture;
}
