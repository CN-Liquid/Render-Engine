#include "TexturedMesh.hpp"
#include <chrono>
#include <unordered_map>
TexturedMesh::TexturedMesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices) : IndexedMesh(vertices, indices)
{
}

TexturedMesh::TexturedMesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, std::vector<GLfloat> normals) : IndexedMesh(vertices, indices, normals)
{
}

void TexturedMesh::uploadGPU()
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
    m_vertexArrayObject.unbind();
}

void TexturedMesh::draw(Shader &shader)
{
    shader.use();

    setMaterial(glm::vec3(0.4f), glm::vec3(0.9f), glm::vec3(0.0f), 1.0f);

    uploadMaterialUniforms(shader);

    shader.setUniformMat4("model", model);

    if (m_texture != nullptr)
    {
        m_texture->bind();
    }
    bind();

    GLCALL(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr));

    unbind();
    if (m_texture != nullptr)
    {
        m_texture->unbind();
    }
}

inline glm::vec3 getVertex(unsigned int index, std::vector<float> &vertices)
{
    return glm::vec3(vertices.at(5 * index), vertices.at(5 * index + 1), vertices.at(5 * index + 2));
}

inline glm::vec3 getNormal(unsigned int index, std::vector<float> &normals)
{
    return glm::vec3(normals.at(3 * index), normals.at(3 * index + 1), normals.at(3 * index + 2));
}

inline void setNormal(unsigned int index, std::vector<float> &normals, glm::vec3 normalData)
{
    normals[3 * index] = normalData.x;
    normals[3 * index + 1] = normalData.y;
    normals[3 * index + 2] = normalData.z;
}
inline void accumulateNormal(unsigned int index, std::vector<float> &normals, glm::vec3 normalData)
{
    normals[3 * index] += normalData.x;
    normals[3 * index + 1] += normalData.y;
    normals[3 * index + 2] += normalData.z;
}

struct Vec3Hash
{
    std::size_t operator()(const glm::vec3 &v) const noexcept
    {
        std::size_t h1 = std::hash<float>{}(v.x);
        std::size_t h2 = std::hash<float>{}(v.y);
        std::size_t h3 = std::hash<float>{}(v.z);

        // boost::hash_combine-like
        std::size_t seed = h1;
        seed ^= h2 + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
        seed ^= h3 + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
        return seed;
    }
};

void TexturedMesh::computeNormals()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<glm::vec3> faceNormals;
    m_normals.resize(m_vertices.size() * 3 / 5, 0.0f);

    std::unordered_map<glm::vec3, std::vector<GLuint>, Vec3Hash> uniqueVertexToIndices;
    std::vector<std::string> keyRepo;

    for (int i = 0; i < m_indices.size(); i = i + 3)
    {
        glm::vec3 vertex1 = getVertex(m_indices.at(i), m_vertices);
        glm::vec3 vertex2 = getVertex(m_indices.at(i + 1), m_vertices);
        glm::vec3 vertex3 = getVertex(m_indices.at(i + 2), m_vertices);

        glm::vec3 normal = glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));

        faceNormals.push_back(normal);

        uniqueVertexToIndices[vertex1].push_back(i / 3);

        uniqueVertexToIndices[vertex2].push_back(i / 3);

        uniqueVertexToIndices[vertex3].push_back(i / 3);
    }

    for (int i = 0; i < m_vertices.size() / 5; i++)
    {
        glm::vec3 vertex = getVertex(i, m_vertices);
        std::vector<GLuint> &faces = uniqueVertexToIndices.at(vertex);
        for (int j = 0; j < faces.size(); j++)
        {
            accumulateNormal(i, m_normals, faceNormals.at(faces.at(j)));
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;

    std::cout << "Thes function took " << duration.count() << " seconds" << std::endl;
}
void TexturedMesh::linkTexture(Texture *texture)
{
    if (texture == nullptr)
    {
        std::cout << "No textured linked" << __FILE__ << " : " << __LINE__ << std::endl;
    }
    else
    {
        m_texture = texture;
    }
}