#include "ObjLoader.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>
struct VertexKey
{
    float px, py, pz;
    float tx, ty;
    float nx, ny, nz;
    bool operator==(const VertexKey &o) const
    {
        return px == o.px && py == o.py && pz == o.pz &&
               tx == o.tx && ty == o.ty &&
               nx == o.nx && ny == o.ny && nz == o.nz;
    }
};

struct VertexKeyHash
{
    std::size_t operator()(const VertexKey &v) const noexcept
    {
        auto h1 = std::hash<float>{}(v.px) ^ (std::hash<float>{}(v.py) << 1) ^ (std::hash<float>{}(v.pz) << 2);
        auto h2 = std::hash<float>{}(v.tx) ^ (std::hash<float>{}(v.ty) << 1);
        auto h3 = std::hash<float>{}(v.nx) ^ (std::hash<float>{}(v.ny) << 1) ^ (std::hash<float>{}(v.nz) << 2);
        return h1 ^ h2 ^ h3;
    }
};
ObjLoader::ObjLoader(const std::string &filePath)
{
    loadMesh(filePath);
}

void ObjLoader::loadMesh(const std::string &filePath)
{
    this->filePath = filePath;

    vertices.clear();
    texCoords.clear();
    normals.clear();
    indices.clear();

    std::ifstream meshFile(filePath);
    if (!meshFile.is_open())
    {
        std::cout << "Failed to open mesh file" << std::endl;
        return;
    }
    std::string line;
    std::stringstream tokens;
    std::string type;
    float data;
    char discard;
    unsigned int index;

    std::vector<float> tempVertices;
    std::vector<float> tempTexCoords;
    std::vector<float> tempNormals;
    std::vector<unsigned int> vertexIndices;
    std::vector<unsigned int> texCoordIndices;
    std::vector<unsigned int> normalIndices;

    while (std::getline(meshFile, line))
    {
        tokens.str(line);
        tokens >> type;
        if (type == "v")
        {
            tokens >> data;
            tempVertices.push_back(data);
            tokens >> data;
            tempVertices.push_back(data);
            tokens >> data;
            tempVertices.push_back(data);
        }
        else if (type == "vt")
        {
            tokens >> data;
            tempTexCoords.push_back(data);
            tokens >> data;
            tempTexCoords.push_back(data);
        }
        else if (type == "vn")
        {
            tokens >> data;
            tempNormals.push_back(data);
            tokens >> data;
            tempNormals.push_back(data);
            tokens >> data;
            tempNormals.push_back(data);
        }

        else if (type == "f")
        {
            while (tokens >> index)
            {

                vertexIndices.push_back(index - 1);
                if (tokens.peek() == '/')
                {
                    tokens >> discard;
                    if (tokens.peek() == '/')
                    {
                        tokens >> discard;
                        tokens >> index;
                        normalIndices.push_back(index - 1);
                    }
                    else
                    {
                        tokens >> index;
                        texCoordIndices.push_back(index - 1);

                        if (tokens.peek() == '/')
                        {
                            tokens >> discard;
                            tokens >> index;
                            normalIndices.push_back(index - 1);
                        }
                    }
                }
            }
        }

        tokens.str("");
        tokens.clear();
    }
    std::unordered_map<VertexKey, unsigned int, VertexKeyHash> vertexMap;

    for (size_t i = 0; i < vertexIndices.size(); i++)
    {
        VertexKey key;
        key.px = tempVertices[3 * vertexIndices[i] + 0];
        key.py = tempVertices[3 * vertexIndices[i] + 1];
        key.pz = tempVertices[3 * vertexIndices[i] + 2];

        if (tempTexCoords.size() != 0)
        {
            key.tx = tempTexCoords[2 * texCoordIndices[i] + 0];
            key.ty = tempTexCoords[2 * texCoordIndices[i] + 1];
        }
        else
        {
            key.tx = 0.0f;
            key.ty = 0.0f;
        }

        if (tempNormals.size() != 0)
        {
            key.nx = tempNormals[3 * normalIndices[i] + 0];
            key.ny = tempNormals[3 * normalIndices[i] + 1];
            key.nz = tempNormals[3 * normalIndices[i] + 2];
        }
        else
        {
            key.nx = 0.0f;
            key.ny = 0.0f;
            key.nz = 0.0f;
        }

        if (vertexMap.count(key) == 0)
        {
            unsigned int newIndex = static_cast<unsigned int>(vertices.size() / 3);

            vertices.push_back(key.px);
            vertices.push_back(key.py);
            vertices.push_back(key.pz);

            texCoords.push_back(key.tx);
            texCoords.push_back(key.ty);

            normals.push_back(key.nx);
            normals.push_back(key.ny);
            normals.push_back(key.nz);

            vertexMap[key] = newIndex;
            indices.push_back(newIndex);
        }
        else
        {
            indices.push_back(vertexMap[key]);
        }
    }
}
