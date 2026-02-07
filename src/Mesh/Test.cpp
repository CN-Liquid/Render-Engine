#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <chrono>
#include <unordered_map>

static std::vector<float> cubeIndexedVertices = {
    // Front face (x, y, z, u, v)
    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // 0: bottom-left
    1.0f, -1.0f, 1.0f, 1.0f, 0.0f,  // 1: bottom-right
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   // 2: top-right
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  // 3: top-left

    // Back face
    1.0f, -1.0f, -1.0f, 2.0f, 1.0f,  // 4: bottom-right
    -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, // 5: bottom-left
    -1.0f, 1.0f, -1.0f, 1.0f, 2.0f,  // 6: top-left
    1.0f, 1.0f, -1.0f, 2.0f, 2.0f,   // 7: top-right

    // Top face
    -1.0f, 1.0f, -1.0f, 2.0f, 3.0f, // 8: back-left
    -1.0f, 1.0f, 1.0f, 2.0f, 2.0f,  // 9: front-left
    1.0f, 1.0f, 1.0f, 3.0f, 2.0f,   // 10: front-right
    1.0f, 1.0f, -1.0f, 3.0f, 3.0f,  // 11: back-right

    // Bottom face
    -1.0f, -1.0f, 1.0f, 3.0f, 4.0f,  // 12: front-left
    -1.0f, -1.0f, -1.0f, 3.0f, 3.0f, // 13: back-left
    1.0f, -1.0f, -1.0f, 4.0f, 3.0f,  // 14: back-right
    1.0f, -1.0f, 1.0f, 4.0f, 4.0f,   // 15: front-right

    // Right face
    1.0f, -1.0f, 1.0f, 4.0f, 4.0f,  // 16: front-bottom
    1.0f, 1.0f, 1.0f, 4.0f, 5.0f,   // 17: front-top
    1.0f, 1.0f, -1.0f, 5.0f, 5.0f,  // 18: back-top
    1.0f, -1.0f, -1.0f, 5.0f, 4.0f, // 19: back-bottom

    // Left face
    -1.0f, -1.0f, -1.0f, 5.0f, 5.0f, // 20: back-bottom
    -1.0f, 1.0f, -1.0f, 5.0f, 6.0f,  // 21: back-top
    -1.0f, 1.0f, 1.0f, 6.0f, 6.0f,   // 22: front-top
    -1.0f, -1.0f, 1.0f, 6.0f, 5.0f   // 23: front-bottom
};

// Indices for counter-clockwise winding order when viewed from outside
static std::vector<uint> cubeIndexedIndices = {
    // Front face (+Z) (CCW)
    0,
    1,
    2,
    0,
    2,
    3,
    // Back face (-Z) (CCW)
    4,
    5,
    6,
    4,
    6,
    7,
    // Top face (+Y) (CCW)
    8,
    9,
    10,
    8,
    10,
    11,
    // Bottom face (-Y) (CCW)
    12,
    13,
    14,
    12,
    14,
    15,
    // Right face (+X) fixed
    16,
    18,
    17,
    16,
    19,
    18,

    // Left face (-X) fixed
    20,
    22,
    21,
    20,
    23,
    22,
};

glm::vec3 getVertex(unsigned int index, std::vector<float> &vertices, int mode = 0)
{
    if (mode == 0)
    {
        return glm::vec3(vertices.at(5 * index), vertices.at(5 * index + 1), vertices.at(5 * index + 2));
    }
    else
    {
        return glm::vec3(vertices.at(3 * index), vertices.at(3 * index + 1), vertices.at(3 * index + 2));
    }
}

glm::vec3 getNormal(unsigned int index, std::vector<float> &normals)
{
    return glm::vec3(normals.at(3 * index), normals.at(3 * index + 1), normals.at(3 * index + 2));
}

void setNormal(unsigned int index, std::vector<float> &normals, glm::vec3 normalData)
{
    normals[3 * index] = normalData.x;
    normals[3 * index + 1] = normalData.y;
    normals[3 * index + 2] = normalData.z;
}

void accumulateNormal(unsigned int index, std::vector<float> &normals, glm::vec3 normalData)
{
    normals[3 * index] += normalData.x;
    normals[3 * index + 1] += normalData.y;
    normals[3 * index + 2] += normalData.z;
}
std::string vec3ToString(const glm::vec3 &v)
{
    return "(" + std::to_string(v.x) + ", " +
           std::to_string(v.y) + ", " +
           std::to_string(v.z) + ")";
}
int main()
{
    std::vector<float> m_vertices = cubeIndexedVertices;
    std::vector<unsigned int> m_indices = cubeIndexedIndices;
    std::vector<float> m_normals;
    std::vector<unsigned int> temp_indices;
    std::vector<float> temp_normals;
    std::vector<float> temp_vertices;

    std::vector<std::vector<unsigned int>> vertexToFace;

    temp_normals.reserve(m_indices.size());
    m_normals.resize(m_vertices.size() * 3 / 5, 0.0f);

    int indicesSize = m_indices.size();

    vertexToFace.resize(m_vertices.size() / 5);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < indicesSize; i = i + 3)
    {
        glm::vec3 vertex1 = getVertex(m_indices.at(i), m_vertices);
        glm::vec3 vertex2 = getVertex(m_indices.at(i + 1), m_vertices);
        glm::vec3 vertex3 = getVertex(m_indices.at(i + 2), m_vertices);

        vertexToFace.at(m_indices.at(i)).push_back(i / 3);
        vertexToFace.at(m_indices.at(i + 1)).push_back(i / 3);
        vertexToFace.at(m_indices.at(i + 2)).push_back(i / 3);

        glm::vec3 edge1 = vertex2 - vertex1;
        glm::vec3 edge2 = vertex3 - vertex1;

        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        temp_normals.push_back(normal.x);
        temp_normals.push_back(normal.y);
        temp_normals.push_back(normal.z);
    }

    for (int i = 0; i < vertexToFace.size(); i++)
    {
        for (int j = 0; j < vertexToFace.at(i).size(); j++)
        {
            glm::vec3 normal = getNormal(vertexToFace.at(i).at(j), temp_normals);
            accumulateNormal(i, m_normals, normal);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;

    std::cout << "Thes function took " << duration.count() << " seconds" << std::endl;

    return 0;
}