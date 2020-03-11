/**
 * @author Florian Feuerstein
 * @date 09.03.2017
 *
 */

#pragma once

#include <Graphics3d/Rendering/RenderPath.h>
#include <Graphics3d/SharedPtrTypes.h>
#include <gl/glew.h>

namespace G3d
{

class Camera;
class Light;

class DeferredShadingPass : public RenderPath
{
public:
  DeferredShadingPass(unsigned int width, unsigned int height);

  virtual ~DeferredShadingPass();

  void render(Camera const * pCamera, Light const * pLight);

  void setInputDepth(TextureCPtr pTexture);

  void setInputNormals(TextureCPtr pTexture);

  void setInputAlbedo(TextureCPtr pTexture);

  void setInputShadowMap(TextureCPtr pTexture);

  TexturePtr getOutputBuffer();

  void clear();

protected:
  DeferredShadingPass(DeferredShadingPass const &) = delete;

  DeferredShadingPass & operator=(DeferredShadingPass const &) = delete;

  unsigned int mWidth;

  unsigned int mHeight;

  TextureCPtr mpInputDepth;

  TextureCPtr mpInputNormals;

  TextureCPtr mpInputAlbedo;

  TextureCPtr mpInputShadowMap;

  GLuint mLightTypeLoc;
  GLuint mLightPosDirLoc;
  GLuint mLightColorLoc;
  GLuint mLightIntensityLoc;
  GLuint mNearPlaneLoc;
  GLuint mFarPlaneLoc;
  GLuint mInvProjLoc;
  GLuint mCameraToLightTrafo;
};

} // namespace G3d
