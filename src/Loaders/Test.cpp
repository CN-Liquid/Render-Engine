#include "ObjLoader.hpp"

int main()
{
    ObjLoader loader("res/cube.obj");

    std::cout << loader.vertices.size() / 3 << std::endl;
    for (int i = 0; i < loader.vertices.size(); i = i + 3)
    {
        std::cout << loader.vertices.at(i) << "  " << loader.vertices.at(i + 1) << "  " << loader.vertices.at(i + 2) << std::endl;
    }
    std::cout << loader.normals.size() / 3 << std::endl;
    for (int i = 0; i < loader.normals.size(); i = i + 3)
    {
        std::cout << loader.normals.at(i) << "  " << loader.normals.at(i + 1) << "  " << loader.normals.at(i + 2) << std::endl;
    }
    std::cout << loader.texCoords.size() / 2 << std::endl;
    for (int i = 0; i < loader.texCoords.size(); i = i + 2)
    {
        std::cout << loader.texCoords.at(i) << "  " << loader.texCoords.at(i + 1) << std::endl;
    }
}