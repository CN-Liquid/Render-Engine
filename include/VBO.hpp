#pragma once
#include "glad/glad.h"

class VBO
{

  GLuint m_ID = 0;

public:
  VBO() = default;
  VBO(GLfloat *vertices, GLsizei size);

  VBO(const VBO &) = delete;
  VBO &operator=(const VBO &) = delete;

  VBO(VBO &&other) noexcept;
  VBO &operator=(VBO &&other) noexcept;

  void generate();

  void bind() const;
  void unbind() const;
  void del() const;

  void populate(GLfloat *vertices, GLsizei size);

  GLuint getID();
  void setID(GLuint ID);
};