#pragma warning(disable : 4201)
#include "Graphics3d/Rendering/RenderPaths/ShadowMapPath.h"
#include <Graphics3d/Assets/Mesh.h>
#include <Graphics3d/Assets/MeshManager.h>
#include <Graphics3d/Assets/Shader.h>
#include <Graphics3d/Assets/ShaderManager.h>
#include <Graphics3d/Components/Light.h>
#include <Graphics3d/Components/Transform.h>
#include <Graphics3d/Rendering/FrameBufferObject.h>
#include <Graphics3d/Scene/SceneObject.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

g3d::ShadowMapPass::ShadowMapPass(unsigned int width, unsigned int height)
    : RenderPath(nullptr,
                 new FrameBufferObject(width, height,
                                       GL_DEPTH_COMPONENT24)) // depth buffer
{
}

g3d::ShadowMapPass::~ShadowMapPass() {}

void g3d::ShadowMapPass::render(Light const * pLight,
                                std::vector<SceneObject const *> sceneObjects)
{
  mpFrameBuffer->use();
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, mpFrameBuffer->getWidth(), mpFrameBuffer->getHeight());
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_DEPTH_BUFFER_BIT);

  glm::mat4 modelViewMatrix;
  glm::mat4 projectionMatrix =
      glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -20.0f, 20.0f);
  glm::mat4 modelViewProjectionMatrix;
  // glCullFace(GL_FRONT);
  for (std::vector<SceneObject const *>::const_iterator objectIter =
           sceneObjects.cbegin();
       objectIter != sceneObjects.cend(); ++objectIter)
  {
    // TODO: add Camera::current or Camera::main and do this in the materials!?
    // this is not possible here since it is a light as camera?
    modelViewMatrix =
        m180Rotation *
        pLight->getSceneObject()->getTransform()->getWorldToLocalMatrix() *
        (*objectIter)->getTransform()->getLocalToWorldMatrix();
    modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
    Shader::setGlobalMat4("modelViewProjectionMatrix",
                          modelViewProjectionMatrix);

    // TODO: !!!!!!!!!
    // simply render the filled border of the terrain and render in normal front
    // mode! easy!

    // optional?
    // glFrontFace(GL_CW);
    (*objectIter)->renderDepth();

    // optional?
    // glFrontFace(GL_CCW);
    //(*objectIter)->RenderDepth();
  }
  // glCullFace(GL_BACK);
}

g3d::TexturePtr g3d::ShadowMapPass::getDepthBuffer()
{
  return mpFrameBuffer->getDepthComponent();
}
