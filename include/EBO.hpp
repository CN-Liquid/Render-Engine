#pragma once
#include "glad/glad.h"

class EBO
{

    GLuint m_ID = 0;

public:
    EBO() = default;
    EBO(GLuint *vertices, GLsizei size);

    EBO(const EBO &) = delete;
    EBO &operator=(const EBO &) = delete;

    EBO(EBO &&other) noexcept;
    EBO &operator=(EBO &&other) noexcept;

    void generate();

    void bind() const;
    void unbind() const;
    void del() const;

    void populate(GLuint *vertices, GLsizei size);

    GLuint getID()const;
    void setID(GLuint ID);
};