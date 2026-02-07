#pragma once
#include <iostream>
#include <vector>
class ObjLoader
{
public:
    std::string filePath;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;

    ObjLoader(const std::string &filePath);
    ObjLoader() = default;
    void loadMesh(const std::string &filepath);
};