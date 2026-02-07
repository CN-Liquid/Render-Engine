#pragma once
#include "VBO.hpp"
#include "glad/glad.h"

class VAO
{

  GLuint m_ID = 0;

public:
  VAO() = default;
  VAO(const VAO &) = delete;
  VAO &operator=(VAO &) = delete;

  VAO(VAO &&other) noexcept;
  VAO &operator=(VAO &&other) noexcept;

  void generate();

  void linkVBO(VBO &vbo, GLuint layout , GLuint size = 3 , GLenum type = GL_FLOAT , GLboolean normalized = GL_FALSE ,
                   GLsizei stride = 3*sizeof(GLfloat) , GLuint offset = 0) const;
  void linkModels(VBO &vbo, GLuint startingLayout) const;

  GLuint getID()const;
  void setID(GLuint ID);

  void bind() const;
  void unbind() const;
  void del() const;
};