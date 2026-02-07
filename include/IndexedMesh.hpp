#pragma once
#include "Mesh.hpp"
#include "EBO.hpp"

class IndexedMesh : public Mesh
{
public:
    EBO m_indexBufferObject;
    std::vector<GLuint> m_indices;

public:
    IndexedMesh() = default;
    IndexedMesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
    IndexedMesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, std::vector<GLfloat> normals);

    IndexedMesh(const IndexedMesh &) = delete; // no copy
    IndexedMesh &operator=(const IndexedMesh &) = delete;

    IndexedMesh(IndexedMesh &&) noexcept; // yes move
    IndexedMesh &operator=(IndexedMesh &&) noexcept;

    void uploadGPU();
    void setIndices(std::vector<GLuint> indices);
    void computeNormals();
    void draw(Shader &shader) const;
};