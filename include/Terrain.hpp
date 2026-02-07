#pragma once
#include "TexturedMesh.hpp"
#include "Texture.hpp"

class Terrain : public TexturedMesh
{
private:
    GLuint m_length, m_chunks;

public:
    Terrain(GLuint length, GLuint chunks);
    Terrain() = default;
    void generate(GLuint length, GLuint chunks);
    void setHeightMap(std::vector<GLfloat> &heightMap, GLuint width, GLuint height);
    void setHeightMap(Texture &heightMap);
};
