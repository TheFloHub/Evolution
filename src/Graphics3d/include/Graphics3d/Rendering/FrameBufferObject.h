/**
 * @author Florian Feuerstein
 * @date 09.03.2017
 *
 */

#pragma once

#include <Graphics3d/SharedPtrTypes.h>
#include <gl/glew.h>
#include <vector>

namespace G3d
{

class FrameBufferObject
{
public:
  // TODO: variadic templates
  FrameBufferObject(unsigned int width, unsigned int height, GLenum depthFormat,
                    GLenum colorFormat1 = 0, GLenum colorFormat2 = 0,
                    GLenum colorFormat3 = 0);

  ~FrameBufferObject();

  void use() const;

  TexturePtr getTexture(size_t index);

  TexturePtr getDepthComponent();

  unsigned int getWidth() const;

  unsigned int getHeight() const;

private:
  FrameBufferObject(FrameBufferObject const &) = delete;

  FrameBufferObject & operator=(FrameBufferObject const &) = delete;

  std::vector<TexturePtr> mColorAttachments;

  TexturePtr mDepthComponent;

  GLuint mFboHandle;

  unsigned int mWidth;

  unsigned int mHeight;
};

} // namespace G3d
