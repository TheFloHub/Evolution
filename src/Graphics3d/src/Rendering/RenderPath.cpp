#include "Graphics3d/Rendering/RenderPath.h"
#include <Graphics3d/Assets/Shader.h>
#include <Graphics3d/Assets/Texture.h>
#include <Graphics3d/GlInfo.h>
#include <Graphics3d/Rendering/FrameBufferObject.h>

g3d::RenderPath::RenderPath(ShaderCPtr pShader,
                            FrameBufferObject * pFrameBuffer)
    : m180Rotation(-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                   -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),
      mpShader(pShader), mpFrameBuffer(pFrameBuffer)
{
}

g3d::RenderPath::~RenderPath() {}
