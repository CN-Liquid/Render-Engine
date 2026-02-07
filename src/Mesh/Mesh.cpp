#include "Mesh.hpp"
#include <iostream>
#include <cmath>
void normalize(float &x, float &y, float &z)
{
    float length = std::sqrt(x * x + y * y + z * z);
    if (length > 1e-6f)
    {
        x /= length;
        y /= length;
        z /= length;
    }
}

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<GLfloat> vertices)
{
    m_vertices = vertices;
    m_normals.resize(m_vertices.size());
}

Mesh::Mesh(Mesh &&other) noexcept : m_vertices(std::move(other.m_vertices)), m_normals(std::move(m_normals)),
                                    m_vertexArrayObject(std::move(other.m_vertexArrayObject)),
                                    m_vertexBufferObject(std::move(other.m_vertexBufferObject)),
                                    m_normalBufferObject(std::move(other.m_normalBufferObject))
{
    other.m_vertexArrayObject.setID(0);
    other.m_vertexBufferObject.setID(0);
    other.m_normalBufferObject.setID(0);
}

Mesh &Mesh::operator=(Mesh &&other) noexcept
{
    if (this != &other)
    {
        m_vertices = std::move(other.m_vertices);
        m_normals = std::move(other.m_normals);

        m_vertexArrayObject = std::move(other.m_vertexArrayObject);
        m_vertexBufferObject = std::move(other.m_vertexBufferObject);
        m_normalBufferObject = std::move(other.m_normalBufferObject);

        other.m_vertexArrayObject.setID(0);
        other.m_vertexBufferObject.setID(0);
        other.m_normalBufferObject.setID(0);
    }
    return *this;
}

void Mesh::computeNormals()
{
    if (m_vertices.empty())
    {
        std::cout << "Mesh :  = > " << " Unable to calculate normals, vertices empty " << std::endl;
    }
    std::cout << "Mesh :  => " << " Computing Normals" << std::endl;

    for (size_t i = 0; i < m_vertices.size(); i += 9)
    {
        // Get the 3 vertices of the triangle
        float x0 = m_vertices[i], y0 = m_vertices[i + 1], z0 = m_vertices[i + 2];
        float x1 = m_vertices[i + 3], y1 = m_vertices[i + 4], z1 = m_vertices[i + 5];
        float x2 = m_vertices[i + 6], y2 = m_vertices[i + 7], z2 = m_vertices[i + 8];

        // Edge vectors
        float ux = x1 - x0, uy = y1 - y0, uz = z1 - z0;
        float vx = x2 - x0, vy = y2 - y0, vz = z2 - z0;

        // Cross product (normal)
        float nx = uy * vz - uz * vy;
        float ny = uz * vx - ux * vz;
        float nz = ux * vy - uy * vx;

        normalize(nx, ny, nz); // Normalize the face normal

        // Assign the same normal to all 3 vertices (flat shading)
        for (int j = 0; j < 3; ++j)
        {
            m_normals[i + j * 3 + 0] = nx;
            m_normals[i + j * 3 + 1] = ny;
            m_normals[i + j * 3 + 2] = nz;
        }
    }
}

void Mesh::uploadGPU()
{
    if (m_vertices.empty())
    {
        std::cout << "Mesh : " << " => Unable to upload : Data Empty" << std::endl;
        return;
    }

    std::cout << "Mesh : " << " => " << " Uploading to the gpu" << std::endl;

    m_vertexArrayObject.generate();

    m_vertexBufferObject.generate();
    m_vertexBufferObject.populate(m_vertices.data(), m_vertices.size());
    m_vertexArrayObject.linkVBO(m_vertexBufferObject, 0);

    m_normalBufferObject.generate();
    m_normalBufferObject.populate(m_normals.data(), m_normals.size());
    m_vertexArrayObject.linkVBO(m_normalBufferObject, 1);
}
void Mesh::bind() const
{
    m_vertexArrayObject.bind();
}
void Mesh::unbind() const
{
    m_vertexArrayObject.unbind();
}

void Mesh::updateModel()
{
    model = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), axis) * glm::scale(glm::mat4(1.0f), scale);
}

void Mesh::draw(Shader &shader) const
{
    shader.use();

    uploadMaterialUniforms(shader);

    shader.setUniformMat4("model", model);

    m_vertexArrayObject.bind();

    GLCALL(glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 3));

    m_vertexArrayObject.unbind();
}

void Mesh::setNormals(std::vector<GLfloat> normals)
{
    if (normals.empty())
    {
        std::cout << "Mesh : " << "=> Unable to set normals : list empty" << std::endl;
        return;
    }
    m_normals = normals;
}

void Mesh::setMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess)
{
    material.ambient = ambient;
    material.diffuse = diffuse;
    material.specular = specular;
    material.shininess = shininess;
}

void Mesh::uploadMaterialUniforms(Shader &shader) const
{
    shader.use();
    shader.setUniformVec3("material.ambient", material.ambient);
    shader.setUniformVec3("material.diffuse", material.diffuse);
    shader.setUniformVec3("material.specular", material.specular);
    shader.setUniformFloat("material.shininess", material.shininess);
}

void Mesh::setVertices(std::vector<GLfloat> vertices)
{
    if (vertices.empty())
    {
        std::cout << "Mesh : " << "=> Unable to set vertices : list empty" << std::endl;
    }
    m_vertices.clear();
    m_vertices = vertices;
}

std::vector<GLfloat> Mesh::getNormals()
{
    return m_normals;
}

std::vector<GLfloat> Mesh::getVertices()
{
    return m_vertices;
}