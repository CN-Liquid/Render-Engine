#pragma once
#include "glad/glad.h"
#include "VAO.hpp"
#include "VBO.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include "Shader.hpp"
#include "Error.hpp"

struct Material
{
    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;
    float shininess;
};

class Mesh
{
protected:
    VAO m_vertexArrayObject;
    VBO m_vertexBufferObject;
    VBO m_normalBufferObject;

    std::vector<GLfloat> m_vertices;
    std::vector<GLfloat> m_normals;

public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f);
    float rotation = 0.0f;

    Material material;

    glm::mat4 model = glm::mat4(1.0f);

    Mesh();

    Mesh(std::vector<GLfloat> vertices);

    // deleted copy operation
    Mesh(Mesh &other) = delete;
    Mesh &operator=(Mesh &other) = delete;

    Mesh(Mesh &&other) noexcept;
    Mesh &operator=(Mesh &&other) noexcept;

    void setMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess);
    void setNormals(std::vector<GLfloat> normals);
    void setVertices(std::vector<GLfloat> vertices);
    std::vector<GLfloat> getNormals();
    std::vector<GLfloat> getVertices();

    void computeNormals();

    void updateModel();

    void uploadMaterialUniforms(Shader &shader) const;
    void uploadGPU();

    void bind() const;
    void unbind() const;

    void draw(Shader &shader) const;
};