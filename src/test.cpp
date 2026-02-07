#include <iostream>
#include "Model.hpp"

int main()
{
    Model model;
    model.loadModel("res/cube.obj");

    std::vector<GLfloat> vertices = model.getMeshes().back().getVertices();

    for (int i = 0; i < vertices.size(); i = i + 3)
    {
        std::cout << vertices.at(i) << "  " << vertices.at(i + 1) << "  " << vertices.at(i + 2) << std::endl;
    }
    return 0;
}