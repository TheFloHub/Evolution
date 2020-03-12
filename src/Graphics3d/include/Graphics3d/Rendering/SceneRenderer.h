/**
 * @author Florian Feuerstein
 * @date 01.08.2016
 *
 * The scene renderer is responsible for rendering the whole scene
 * with all lights, cameras and objects.
 */

#pragma once

#include <Graphics3d/Rendering/RenderPaths/DeferredShadingPath.h>
#include <Graphics3d/Rendering/RenderPaths/FinalShadingPath.h>
#include <Graphics3d/Rendering/RenderPaths/GraphicsBufferPath.h>
#include <Graphics3d/Rendering/RenderPaths/ShadowMapPath.h>
#include <vector>

namespace g3d
{

// forward declarations
class SceneObject;
class Light;
class Camera;

class SceneRenderer
{
public:
  SceneRenderer(unsigned int width, unsigned int height);

  ~SceneRenderer();

  void render(SceneObject const * pRoot);

  void setImageSize(unsigned int width, unsigned int height);

private:
  SceneRenderer(SceneRenderer const &) = delete;

  SceneRenderer & operator=(SceneRenderer const &) = delete;

  void traverseGraph(SceneObject const * pCurrentNode,
                     std::vector<SceneObject const *> & sceneObjects,
                     std::vector<Light const *> & lights,
                     std::vector<Camera const *> & cameras) const;

  unsigned int mWidth;

  unsigned int mHeight;

  // G Buffer Path
  GraphicsBufferPath mGraphicsBufferPath;

  // Shadow Map Path
  ShadowMapPass mShadowMapPath;

  // Deferred Shading Pass
  DeferredShadingPass mDeferredShadingPath;

  // Final Shading Pass (ambient / HDR / Gamma correction)
  FinalShadingPass mFinalShadingPass;
};

} // namespace g3d
