/**
 * @author Florian Feuerstein
 * @date 25.07.2016
 *
 * A Shader loads glsl shader files, links
 * and compiles them, so the shader can be used
 * for a material.
 *
 */

#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

namespace G3d
{

class Shader
{

public:
  static int getGlobalInt(std::string const & name);
  static void setGlobalInt(std::string const & name, int value);

  static float getGlobalFloat(std::string const & name);
  static void setGlobalFloat(std::string const & name, float value);

  static glm::vec3 const & getGlobalFloatVec3(std::string const & name);
  static void setGlobalFloatVec3(std::string const & name,
                                 glm::vec3 const & value);

  static glm::mat3 const & getGlobalMat3(std::string const & name);
  static void setGlobalMat3(std::string const & name, glm::mat3 const & value);

  static glm::mat4 const & getGlobalMat4(std::string const & name);
  static void setGlobalMat4(std::string const & name, glm::mat4 const & value);

  Shader(std::string const & vertexShaderCode,
         std::string const & fragmentShaderCode);

  ~Shader();

  void use() const;

  GLint getUniformLocation(std::string const & name) const;

  GLuint getHandle() const;

private:
  Shader(Shader const &) = delete;

  Shader & operator=(Shader const &) = delete;

  GLuint createShaderComponent(GLuint const shaderType,
                               std::string const & shaderCode) const;

  GLuint mShaderProgram;

  // all global shader parameters
  static std::map<std::string, int> GlobalIntParameters;
  static std::map<std::string, float> GlobalFloatParameters;
  static std::map<std::string, glm::vec3> GlobalFloatVec3Parameters;
  static std::map<std::string, glm::mat3> GlobalMat3Parameters;
  static std::map<std::string, glm::mat4> GlobalMat4Parameters;
};

} // namespace G3d
