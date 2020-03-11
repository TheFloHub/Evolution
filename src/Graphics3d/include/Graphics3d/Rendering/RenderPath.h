/**
 * @author Florian Feuerstein
 * @date 09.03.2017
 *
 */

#pragma once

#include <Graphics3d/Assets/Shader.h>
#include <Graphics3d/Rendering/FrameBufferObject.h>
#include <Graphics3d/SharedPtrTypes.h>
#include <glm/glm.hpp>
#include <memory>

namespace G3d
{
// TODO: do we really need this class?
class RenderPath
{
public:
  RenderPath(ShaderCPtr pShader, FrameBufferObject * pFrameBuffer);

  virtual ~RenderPath();

protected:
  RenderPath(RenderPath const &) = delete;

  RenderPath & operator=(RenderPath const &) = delete;

  glm::mat4 m180Rotation;

  ShaderCPtr mpShader;

  std::unique_ptr<FrameBufferObject> mpFrameBuffer;
};

} // namespace G3d
