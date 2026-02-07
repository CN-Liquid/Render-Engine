#include "VAO.hpp"
#include "glm/glm.hpp"
#include "Error.hpp"
#include <iostream>

void VAO::linkVBO(VBO &vbo, GLuint layout , GLuint size  , GLenum type, GLboolean normalized,
                   GLsizei stride, GLuint offset ) const
{
  if (vbo.getID() == 0)
  {
    std::cout << "Unable to link vbo" << std::endl;
    return;
  }
  bind();

  vbo.bind();

  GLCALL(glVertexAttribPointer(layout, size, type, normalized,stride,(void *)(offset*sizeof(GLfloat))));

  GLCALL(glEnableVertexAttribArray(layout));

  vbo.unbind();
  
  unbind();
}

void VAO::generate()
{
  if (m_ID == 0)
  {
    GLCALL(glGenVertexArrays(1, &m_ID));
  }
  else
  {
    std::cout << "Vertex Generation failed" << std::endl;
  }
}

void VAO::linkModels(VBO &vbo, GLuint startingLayout) const
{
  if (vbo.getID() == 0)
  {
    std::cout << "Unable to link vbo" << std::endl;
    return;
  }
  bind();
  vbo.bind();

  for (int i = 0; i < 4; i++)
  {

    GLCALL(glVertexAttribPointer(startingLayout + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4) * i)));
    
    GLCALL(glEnableVertexAttribArray(startingLayout + i));
    
    GLCALL(glVertexAttribDivisor(startingLayout + i, 1));
  }

  vbo.unbind();
  unbind();
}

void VAO::bind() const
{
  if (m_ID == 0)
  {
    std::cout << "Unable to bind vao" << std::endl;
  }

  GLCALL(glBindVertexArray(m_ID));
}
void VAO::unbind() const
{
  if (m_ID == 0)
  {
    std::cout << "Unable to unbind vao" << std::endl;
  }

  GLCALL(glBindVertexArray(0));
}
void VAO::del() const
{
  if (m_ID != 0)
  {
    GLCALL(glDeleteVertexArrays(1, &m_ID));
  }
}

VAO::VAO(VAO &&other) noexcept
{
  m_ID = other.m_ID;
  other.m_ID = 0;
}

VAO &VAO::operator=(VAO &&other) noexcept
{
  if (this != &other)
  {
    m_ID = other.m_ID;
    other.m_ID = 0;
  }
  return *this;
}

GLuint VAO::getID()const
{
  return m_ID;
}

void VAO::setID(GLuint ID)
{
  m_ID = ID;
}