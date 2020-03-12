/**
 * @author Florian Feuerstein
 * @date 09.03.2017
 *
 */

#pragma once

#include <Graphics3d/Rendering/RenderPath.h>
#include <Graphics3d/SharedPtrTypes.h>
#include <vector>

namespace g3d
{

class Camera;
class SceneObject;

class GraphicsBufferPath : public RenderPath
{
public:
  GraphicsBufferPath(unsigned int width, unsigned int height);

  virtual ~GraphicsBufferPath();

  void render(Camera const * pCamera,
              std::vector<SceneObject const *> sceneObjects);

  TexturePtr getDepthBuffer();

  TexturePtr getNormalBuffer();

  TexturePtr getAlbedoBuffer();

protected:
  GraphicsBufferPath(GraphicsBufferPath const &) = delete;

  GraphicsBufferPath & operator=(GraphicsBufferPath const &) = delete;
};

} // namespace g3d
