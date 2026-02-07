#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
public:
  const char *vertexShaderSource;
  const char *fragmentShaderSource;

  unsigned int ID = 0;

  Shader(const char *vertexShaderSource, const char *fragmentShaderSource);
  ~Shader();

  void setUniformFloat(const char *name, float value)const;
  void setUniformMat4(const char *name, glm::mat4 value)const;
  void setUniformVec4(const char *name, glm::vec4 value)const;
  void setUniformVec3(const char *name, glm::vec3 value)const;
  void setUniformInt(const char *name, int value)const;
  void setUniformBool(const char *name , bool value)const;

  void use()const;
};