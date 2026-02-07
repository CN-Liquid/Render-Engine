#include "Shader.hpp"
#include "glad/glad.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Error.hpp"

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
  std::string vertexShaderSource;
  std::string fragmentShaderSource;
  try
  {
    std::ifstream vertexFile(vertexShaderPath, std::ios::binary);
    if (!vertexFile.is_open())
    {
      std::cout << "Failed to open vertex shader file" << '\n';
      exit(-1);
    }
    std::stringstream vShaderStream;
    vShaderStream << vertexFile.rdbuf();
    vertexShaderSource = vShaderStream.str();
    vertexFile.close();

    std::ifstream fragmentFile(fragmentShaderPath, std::ios::binary);
    if (!fragmentFile.is_open())
    {
      std::cout << "Failed to open fragment shader file" << '\n';
      exit(-1);
    }
    std::stringstream fShaderStream;
    fShaderStream << fragmentFile.rdbuf();
    fragmentShaderSource = fShaderStream.str();
    fragmentFile.close();
  }
  catch (std::exception &e)
  {
    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what()
              << std::endl;

    exit(-1);
  }

  const char *vertexSource = vertexShaderSource.c_str();
  const char *fragmentSource = fragmentShaderSource.c_str();

  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  GLCALL(vertex = glCreateShader(GL_VERTEX_SHADER));
  GLCALL(glShaderSource(vertex, 1, &vertexSource, NULL));
  GLCALL(glCompileShader(vertex));
  GLCALL(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
  if (!success)
  {
    GLCALL(glGetShaderInfoLog(vertex, 512, NULL, infoLog));
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    exit(-1);
  }

  GLCALL(fragment = glCreateShader(GL_FRAGMENT_SHADER));
  GLCALL(glShaderSource(fragment, 1, &fragmentSource, NULL));
  GLCALL(glCompileShader(fragment));
  GLCALL(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
  if (!success)
  {
    GLCALL(glGetShaderInfoLog(fragment, 512, NULL, infoLog));
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    exit(-1);
  }

  GLCALL(ID = glCreateProgram());
  GLCALL(glAttachShader(ID, vertex));
  GLCALL(glAttachShader(ID, fragment));
  GLCALL(glLinkProgram(ID));
  GLCALL(glGetProgramiv(ID, GL_LINK_STATUS, &success));
  if (!success)
  {
    GLCALL(glGetProgramInfoLog(ID, 512, NULL, infoLog));
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    exit(-1);
  }

  GLCALL(glDeleteShader(vertex));
  GLCALL(glDeleteShader(fragment));
}

void Shader::setUniformFloat(const char *name, float value) const
{
  GLCALL(glUniform1f(glGetUniformLocation(this->ID, name), value));
}

void Shader::setUniformMat4(const char *name, glm::mat4 value) const
{
  GLCALL(glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(value)));
}
void Shader::setUniformVec4(const char *name, glm::vec4 value) const
{
  GLCALL(glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w));
}
void Shader::setUniformVec3(const char *name, glm::vec3 value) const
{
  GLCALL(glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z));
}
void Shader::setUniformInt(const char *name, int value) const
{
  GLCALL(glUniform1i(glGetUniformLocation(this->ID, name), value));
}
void Shader::setUniformBool(const char *name, bool value) const
{
  GLCALL(glUniform1i(glGetUniformLocation(this->ID, name), value));
}
void Shader::use() const
{
  GLCALL(glUseProgram(this->ID));
}
Shader::~Shader()
{
  if (ID != 0)
  {
    GLCALL(glDeleteProgram(ID));
  }
}
