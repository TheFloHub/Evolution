#pragma warning(disable : 4201)
#include "Graphics3d/Rendering/RenderPaths/GraphicsBufferPath.h"
#include <Graphics3d/Assets/Shader.h>
#include <Graphics3d/Components/Camera.h>
#include <Graphics3d/Components/Transform.h>
#include <Graphics3d/Rendering/FrameBufferObject.h>
#include <Graphics3d/Scene/SceneObject.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

G3d::GraphicsBufferPath::GraphicsBufferPath(unsigned int width,
                                            unsigned int height)
    : RenderPath(nullptr,
                 new FrameBufferObject(width, height,
                                       GL_DEPTH_COMPONENT24, // depth buffer
                                       GL_RGBA16, // normals, A = roughness
                                       GL_RGBA8)) // albedo, A = metallic
{
}

G3d::GraphicsBufferPath::~GraphicsBufferPath() {}

void G3d::GraphicsBufferPath::render(
    Camera const * pCamera, std::vector<SceneObject const *> sceneObjects)
{
  mpFrameBuffer->use();
  glViewport(0, 0, mpFrameBuffer->getWidth(), mpFrameBuffer->getHeight());
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 modelViewMatrix;
  glm::mat4 modelViewProjectionMatrix;
  glm::mat3 normalMatrix;
  for (std::vector<SceneObject const *>::const_iterator objectIter =
           sceneObjects.cbegin();
       objectIter != sceneObjects.cend(); ++objectIter)
  {
    // TODO: add Camera::current or Camera::main and do this in the materials!?
    modelViewMatrix =
        m180Rotation *
        pCamera->getSceneObject()->getTransform()->getWorldToLocalMatrix() *
        (*objectIter)->getTransform()->getLocalToWorldMatrix();
    modelViewProjectionMatrix =
        pCamera->getProjectionMatrix() * modelViewMatrix;
    normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
    Shader::setGlobalMat4("modelViewMatrix", modelViewMatrix);
    Shader::setGlobalMat4("modelViewProjectionMatrix",
                          modelViewProjectionMatrix);
    Shader::setGlobalMat3("normalMatrix", normalMatrix);
    (*objectIter)->render();
  }
}

G3d::TexturePtr G3d::GraphicsBufferPath::getDepthBuffer()
{
  return mpFrameBuffer->getDepthComponent();
}

G3d::TexturePtr G3d::GraphicsBufferPath::getNormalBuffer()
{
  return mpFrameBuffer->getTexture(0);
}

G3d::TexturePtr G3d::GraphicsBufferPath::getAlbedoBuffer()
{
  return mpFrameBuffer->getTexture(1);
}
