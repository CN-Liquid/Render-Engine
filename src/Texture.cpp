#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Error.hpp"

Texture::Texture(const std::string &filePath) : m_filePath(filePath), m_localBuffer(nullptr), width(0), height(0), BPP(0)
{
    generate(filePath);
}

Texture::~Texture()
{
    if (m_ID != 0)
    {
        GLCALL(glDeleteTextures(1, &m_ID));
    }
    if (m_localBuffer)
    {
        stbi_image_free(m_localBuffer);
    }
}

void Texture::generate(const std::string &filePath)
{
    stbi_set_flip_vertically_on_load(1);

    m_localBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 0);

    if (m_localBuffer == nullptr)
    {
        std::cout << "Loading Texture Failed" << std::endl;
        return;
    }

    else
    {
        std::cout << "Loaded Texture successfully of width " << width << " height " << height << " bpp " << BPP << std::endl;
    }

    GLenum format;

    switch (BPP)
    {
    case 1:
        format = GL_RED;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        std::cerr << "Unsupported texture BPP: " << BPP << std::endl;
        stbi_image_free(m_localBuffer);
        m_localBuffer = nullptr;
        return;
    }

    GLCALL(glGenTextures(1, &m_ID));

    GLCALL(glBindTexture(GL_TEXTURE_2D, m_ID));

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, m_localBuffer));

    GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

    GLCALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.5f));

    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::bind(GLuint slot) const
{
    if (m_ID != 0)
    {
        GLCALL(glActiveTexture(GL_TEXTURE0 + slot));

        GLCALL(glBindTexture(GL_TEXTURE_2D, m_ID));
    }
}

void Texture::unbind() const
{
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned char *Texture::data()
{
    return m_localBuffer;
}

bool Texture::isValid()
{
    if (m_localBuffer == nullptr)
    {
        return false;
    }
    else
    {
        return true;
    }
}