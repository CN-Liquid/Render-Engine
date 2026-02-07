#include "IndexedMesh.hpp"
#include <iostream>
#include <unordered_map>
#include <chrono>
static void normalize(float &x, float &y, float &z)
{
    float length = std::sqrt(x * x + y * y + z * z);
    if (length > 1e-6f)
    {
        x /= length;
        y /= length;
        z /= length;
    }
}

IndexedMesh::IndexedMesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices) : Mesh(vertices)
{
    m_indices.resize(indices.size());
    m_indices = indices;
}

IndexedMesh::IndexedMesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, std::vector<GLfloat> normals) : Mesh(vertices), m_indices(indices)
{
    setNormals(normals);
}

IndexedMesh::IndexedMesh(IndexedMesh &&other) noexcept : m_indices(other.m_indices), Mesh(std::move(other))
{
    other.m_vertexArrayObject.setID(0);
    other.m_vertexBufferObject.setID(0);
    other.m_normalBufferObject.setID(0);
    other.m_indexBufferObject.setID(0);
}

IndexedMesh &IndexedMesh::operator=(IndexedMesh &&other) noexcept
{
    if (this != &other)
    {
        m_vertices = std::move(other.m_vertices);
        m_normals = std::move(other.m_normals);
        m_indices = std::move(other.m_indices);

        m_vertexArrayObject = std::move(other.m_vertexArrayObject);
        m_vertexBufferObject = std::move(other.m_vertexBufferObject);
        m_normalBufferObject = std::move(other.m_normalBufferObject);
        m_indexBufferObject = std::move(other.m_indexBufferObject);

        other.m_vertexArrayObject.setID(0);
        other.m_vertexBufferObject.setID(0);
        other.m_normalBufferObject.setID(0);
        other.m_indexBufferObject.setID(0);
    }
    return *this;
}

void IndexedMesh::uploadGPU()
{
    std::cout << "Mesh : " << " => " << " Uploading to the gpu" << std::endl;
    m_vertexArrayObject.generate();

    m_vertexBufferObject.generate();
    m_vertexBufferObject.populate(m_vertices.data(), m_vertices.size());
    m_vertexArrayObject.linkVBO(m_vertexBufferObject, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    m_vertexArrayObject.linkVBO(m_vertexBufferObject, 2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 3);

    m_normalBufferObject.generate();
    m_normalBufferObject.populate(m_normals.data(), m_normals.size());
    m_vertexArrayObject.linkVBO(m_normalBufferObject, 1);

    m_indexBufferObject.generate();
    m_indexBufferObject.populate(m_indices.data(), m_indices.size());

    m_vertexArrayObject.bind();
    m_indexBufferObject.bind();
}
void IndexedMesh::computeNormals()
{
    auto start = std::chrono::high_resolution_clock::now();
    size_t vertexCount = m_vertices.size() / 5;
    if (vertexCount == 0)
    {
        std::cout << "Vertices list zero cannot compute normals" << std::endl;
        return;
    }
    else
    {
        std::cout << "Mesh => Computing Normals" << std::endl;
    }

    m_normals.clear();
    m_normals.resize(vertexCount * 3, 0.0f);

    // Custom hash function for glm::vec3 as a lambda
    auto vec3Hash = [](const glm::vec3 &v)
    {
        std::size_t h1 = std::hash<float>{}(v.x);
        std::size_t h2 = std::hash<float>{}(v.y);
        std::size_t h3 = std::hash<float>{}(v.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    };

    // Custom equality function for glm::vec3 as a lambda
    auto vec3Equal = [](const glm::vec3 &a, const glm::vec3 &b)
    {
        return glm::all(glm::epsilonEqual(a, b, 1e-6f));
    };

    std::unordered_map<glm::vec3, glm::vec3, decltype(vec3Hash), decltype(vec3Equal)> positionNormals(10, vec3Hash, vec3Equal);

    // First pass: Compute face normals and accumulate for each unique position
    for (size_t i = 0; i < m_indices.size(); i += 3)
    {
        unsigned int i0 = m_indices[i];
        unsigned int i1 = m_indices[i + 1];
        unsigned int i2 = m_indices[i + 2];

        if (i0 >= vertexCount || i1 >= vertexCount || i2 >= vertexCount)
        {
            continue;
        }

        glm::vec3 v0(m_vertices[5 * i0], m_vertices[5 * i0 + 1], m_vertices[5 * i0 + 2]);
        glm::vec3 v1(m_vertices[5 * i1], m_vertices[5 * i1 + 1], m_vertices[5 * i1 + 2]);
        glm::vec3 v2(m_vertices[5 * i2], m_vertices[5 * i2 + 1], m_vertices[5 * i2 + 2]);

        glm::vec3 e1 = v1 - v0;
        glm::vec3 e2 = v2 - v0;
        glm::vec3 faceNormal = glm::cross(e1, e2);

        positionNormals[v0] += faceNormal;
        positionNormals[v1] += faceNormal;
        positionNormals[v2] += faceNormal;
    }

    // Second pass: Assign the normalized normal to all vertices based on their position
    for (size_t i = 0; i < vertexCount; ++i)
    {
        glm::vec3 currentPos(m_vertices[5 * i], m_vertices[5 * i + 1], m_vertices[5 * i + 2]);

        auto it = positionNormals.find(currentPos);
        if (it != positionNormals.end())
        {
            glm::vec3 averagedNormal = glm::normalize(it->second);
            m_normals[3 * i] = averagedNormal.x;
            m_normals[3 * i + 1] = averagedNormal.y;
            m_normals[3 * i + 2] = averagedNormal.z;
        }
        else
        {
            m_normals[3 * i] = 0.0f;
            m_normals[3 * i + 1] = 0.0f;
            m_normals[3 * i + 2] = 0.0f;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;
    std::cout << "The function took " << duration.count() << " seconds" << std::endl;
}
void IndexedMesh::draw(Shader &shader) const
{
    shader.use();

    shader.setUniformMat4("model", model);

    uploadMaterialUniforms(shader);

    m_vertexArrayObject.bind();

    GLCALL(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0));

    m_vertexArrayObject.unbind();
}

void IndexedMesh::setIndices(std::vector<GLuint> indices)
{
    m_indices.clear();
    m_indices = indices;
}
