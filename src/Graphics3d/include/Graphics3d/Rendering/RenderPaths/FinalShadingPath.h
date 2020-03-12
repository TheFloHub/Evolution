/**
 * @author Florian Feuerstein
 * @date 09.03.2017
 *
 */

#pragma once

#include <Graphics3d/Rendering/RenderPath.h>
#include <Graphics3d/SharedPtrTypes.h>

namespace g3d
{

class FinalShadingPass : public RenderPath
{
public:
  FinalShadingPass(unsigned int width, unsigned int height);

  virtual ~FinalShadingPass();

  void render();

  void setInput(TextureCPtr pTexture);

protected:
  FinalShadingPass(FinalShadingPass const &) = delete;

  FinalShadingPass & operator=(FinalShadingPass const &) = delete;

  unsigned int mWidth;

  unsigned int mHeight;

  TextureCPtr mpInput;
};

} // namespace g3d
