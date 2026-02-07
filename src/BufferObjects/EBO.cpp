#include "EBO.hpp"
#include "Error.hpp"
#include <iostream>

EBO::EBO(GLuint *indices, GLsizei size) { populate(indices, size); }

void EBO::bind() const
{
    if (m_ID == 0)
    {
        std::cout << "Unable to bind element buffer" << std::endl;
    }

    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
}

void EBO::unbind() const
{
    if (m_ID == 0)
    {
        std::cout << "Unable to unbind buffer" << std::endl;
    }
    
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void EBO::del() const
{
    if (m_ID != 0)
    {
        GLCALL(glDeleteBuffers(1, &m_ID));
    }
}

void EBO::populate(GLuint *indices, GLsizei size)
{

    if (m_ID == 0)
    {
        std::cout << "Gl buffer has not been created or is corrupt , unable to populate" << std::endl;
    }

    bind();

    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), indices, GL_STATIC_DRAW));
    
    unbind();
}

void EBO::generate()
{
    if (m_ID == 0)
    {
        GLCALL(glGenBuffers(1, &m_ID));
    }
    else
    {
        std::cout << "Unable to generate EBO , EBO already exists or is corrupt" << std::endl;
    }
}

EBO &EBO::operator=(EBO &&other) noexcept
{
    if (this != &other)
    {
        m_ID = other.m_ID;
        other.m_ID = 0;
    }
    return *this;
}

EBO::EBO(EBO &&other) noexcept
{
    m_ID = other.m_ID;
    other.m_ID = 0;
}
GLuint EBO::getID() const
{
    return m_ID;
}
void EBO::setID(GLuint ID)
{
    m_ID = ID;
}