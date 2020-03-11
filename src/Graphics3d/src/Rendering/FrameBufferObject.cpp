#include "Graphics3d/Rendering/FrameBufferObject.h"
#include <Graphics3d/Assets/Texture.h>
#include <Graphics3d/GlInfo.h>
#include <iostream>

G3d::FrameBufferObject::FrameBufferObject(
    unsigned int width, unsigned int height, GLenum depthFormat,
    GLenum colorFormat1, GLenum colorFormat2, GLenum colorFormat3)
    : mColorAttachments(), mDepthComponent(nullptr), mFboHandle(0),
      mWidth(width), mHeight(height)
{
  std::vector<GLenum> colorFormats;
  if (colorFormat1 != 0)
    colorFormats.push_back(colorFormat1);
  if (colorFormat2 != 0)
    colorFormats.push_back(colorFormat2);
  if (colorFormat3 != 0)
    colorFormats.push_back(colorFormat3);

  glGenFramebuffers(1, &mFboHandle);
  glBindFramebuffer(GL_FRAMEBUFFER, mFboHandle);

  if (!colorFormats.empty())
  {
    mColorAttachments.reserve(colorFormats.size());
    std::vector<GLenum> attachements(colorFormats.size(), 0);
    for (unsigned int i = 0; i < colorFormats.size(); ++i)
    {
      TexturePtr pTexture(new Texture(width, height, colorFormats[i]));
      CHECKGLERROR();
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                             GL_TEXTURE_2D, pTexture->getHandle(), 0);
      CHECKGLERROR();
      attachements[i] = GL_COLOR_ATTACHMENT0 + i;
      mColorAttachments.push_back(pTexture);
    }

    glDrawBuffers((GLsizei)attachements.size(), attachements.data());
    CHECKGLERROR();
  }
  else
  {
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }

  if (depthFormat != 0)
  {
    mDepthComponent = TexturePtr(new Texture(width, height, depthFormat));
    CHECKGLERROR();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           mDepthComponent->getHandle(), 0);
    CHECKGLERROR();
  }

  // - Finally check if framebuffer is complete
  CHECKGLFBOERROR();
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "FBO creation error!" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

G3d::FrameBufferObject::~FrameBufferObject()
{
  if (mFboHandle != 0)
    glDeleteFramebuffers(1, &mFboHandle);
}

void G3d::FrameBufferObject::use() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, mFboHandle);
}

G3d::TexturePtr G3d::FrameBufferObject::getTexture(size_t index)
{
  if (index < mColorAttachments.size())
  {
    return mColorAttachments[index];
  }
  return nullptr;
}

G3d::TexturePtr G3d::FrameBufferObject::getDepthComponent()
{
  return mDepthComponent;
}

unsigned int G3d::FrameBufferObject::getWidth() const { return mWidth; }

unsigned int G3d::FrameBufferObject::getHeight() const { return mHeight; }
