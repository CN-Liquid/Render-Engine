#pragma once
#include "IndexedMesh.hpp"
#include "Texture.hpp"
#include "VBO.hpp"

class TexturedMesh : public IndexedMesh
{
private:
    Texture *m_texture = nullptr;

public:
    TexturedMesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
    TexturedMesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, std::vector<GLfloat> normals);
    TexturedMesh() = default;

    void populateTextureCoordinates(std::vector<GLfloat> textCoords);

    void linkTexture(Texture *texture);

    void uploadGPU();

    void computeNormals();

    void draw(Shader &shader);
};