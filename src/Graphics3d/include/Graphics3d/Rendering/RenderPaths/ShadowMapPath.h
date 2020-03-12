/**
 * @author Florian Feuerstein
 * @date 09.03.2017
 *
 */

#pragma once

#include <Graphics3d/SharedPtrTypes.h>
#include <Graphics3d/rendering/RenderPath.h>

namespace g3d
{

class Light;
class SceneObject;

class ShadowMapPass : public RenderPath
{
public:
  ShadowMapPass(unsigned int width, unsigned int height);

  virtual ~ShadowMapPass();

  void render(Light const * pLight,
              std::vector<SceneObject const *> sceneObjects);

  TexturePtr getDepthBuffer();

protected:
  ShadowMapPass(ShadowMapPass const &) = delete;

  ShadowMapPass & operator=(ShadowMapPass const &) = delete;
};

} // namespace g3d
