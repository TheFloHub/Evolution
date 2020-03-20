#include "Graphics/GlInfo.h"
#include <gl/glew.h>
#include <iostream>
#include <string>

void evo::GlInfo::checkGlErrorImpl(int line, const char * file)
{
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
  {
    std::string errorString;
    switch (error)
    {
    case GL_INVALID_ENUM:
      errorString = "GL_INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      errorString = "GL_INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      errorString = "GL_INVALID_OPERATION";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
      break;
    }
    std::cout << "OpenGL error " << errorString << " in file " << file
              << " at line " << line << "." << std::endl;
  }
}

void evo::GlInfo::checkFboErrorImpl(int line, const char * file)
{
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
  {
    std::string errorString;
    switch (status)
    {
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      errorString = "GL_FRAMEBUFFER_UNSUPPORTED_EXT";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
      errorString = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
      errorString = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      errorString = "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      errorString = "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
      errorString = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
      errorString = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT";
      break;
    default:
      errorString = "unknown FBO state";
    }
    std::cout << "OpenGL frame buffer error " << errorString << " in file "
              << file << " at line " << line << "." << std::endl;
  }
}