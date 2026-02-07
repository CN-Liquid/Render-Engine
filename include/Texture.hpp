#pragma once
#include "glad/glad.h"
#include <iostream>
class Texture
{
private:
    GLuint m_ID = 0;
    std::string m_filePath;
    unsigned char *m_localBuffer;

public:
    GLint width, height, BPP;
    Texture(const std::string &filePath);
    Texture() = default;
    ~Texture();

    void bind(GLuint slot = 0) const;
    void unbind() const;
    void generate(const std::string &filePath);
    bool isValid();
    unsigned char *data();
};