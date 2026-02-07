#include "VBO.hpp"
#include "Error.hpp"
#include <iostream>

VBO::VBO(GLfloat *vertices, GLsizei size) { populate(vertices, size); }

void VBO::bind() const
{
  if (m_ID == 0)
  {
    std::cout << "Unable to bind buffer" << std::endl;
  }

  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
}

void VBO::unbind() const
{
  if (m_ID == 0)
  {
    std::cout << "Unable to unbind buffer" << std::endl;
  }

  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VBO::del() const
{
  if (m_ID != 0)
  {
    GLCALL(glDeleteBuffers(1, &m_ID));
  }
}

void VBO::populate(GLfloat *vertices, GLsizei size)
{

  if (m_ID == 0)
  {
    std::cout << "Gl buffer has not been created or is corrupt , unable to populate" << std::endl;
  }

  bind();
  GLCALL(glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), vertices, GL_STATIC_DRAW));
  unbind();
}

void VBO::generate()
{
  if (m_ID == 0)
  {
    GLCALL(glGenBuffers(1, &m_ID));
  }
  else
  {
    std::cout << "Unable to generate VBO , VBO already exists or is corrupt" << std::endl;
  }
}

VBO &VBO::operator=(VBO &&other) noexcept
{
  if (this != &other)
  {
    m_ID = other.m_ID;
    other.m_ID = 0;
  }
  return *this;
}

VBO::VBO(VBO &&other) noexcept
{
  m_ID = other.m_ID;
  other.m_ID = 0;
}
GLuint VBO::getID()
{
  return m_ID;
}
void VBO::setID(GLuint ID)
{
  m_ID = ID;
}