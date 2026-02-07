#include "Terrain.hpp"
Terrain::Terrain(GLuint length, GLuint chunks) : m_length(length), m_chunks(chunks)
{
    generate(length, chunks);
}

void Terrain::generate(GLuint length, GLuint chunks)
{

    m_length = length;
    m_chunks = chunks;

    float step = length / (float)chunks;
    float texX = 0.0f;
    float texY = 0.0f;
    // Loop using integers for precise control
    for (int i = 0; i <= chunks; i++)
    {
        float y = (float)i * step;
        texY = float(i) / 8.0f;
        for (int j = 0; j <= chunks; j++)
        {

            // Calculate the x, y, z coordinates based on integer indices
            float x = (float)j * step;
            // Using 'y' for the vertical axis of the grid, could also be 'z'

            texX = float(j) / 8.0f;

            // Calculate the height value for the grid
            // float z = 20 * exp(-0.001 * (((x - length / 2) * (x - length / 2)) + ((y - length / 2) * (y - length / 2))));
            float z = 0.0f;
            m_vertices.push_back(x);
            m_vertices.push_back(y);
            m_vertices.push_back(z);

            m_vertices.push_back(texX);
            m_vertices.push_back(texY);
        }
    }

    // Assuming chunks represents the number of quads per row/column
    for (int i = 0; i < chunks; i++)
    {
        for (int j = 0; j < chunks; j++)
        {
            // We'll define the four corners of the quad
            // These are the vertex indices
            int p0 = i * (chunks + 1) + j;       // Top-left
            int p1 = p0 + 1;                     // Top-right
            int p2 = (i + 1) * (chunks + 1) + j; // Bottom-left
            int p3 = p2 + 1;                     // Bottom-right

            // First triangle (upper-left half)
            m_indices.push_back(p0);
            m_indices.push_back(p3);
            m_indices.push_back(p2);

            // Second triangle (bottom-right half)
            m_indices.push_back(p3);
            m_indices.push_back(p0);
            m_indices.push_back(p1);
        }
    }
}

void Terrain::setHeightMap(std::vector<GLfloat> &heightMap, GLuint width, GLuint height)
{

    std::cout << "M_vertices size = " << m_vertices.size() << std::endl;

    if (width == m_chunks && height == m_chunks)
    {
        for (int i = 0; i < m_vertices.size() / 5; i++)
        {
            m_vertices[i * 5 + 2] += (10 * heightMap.at(i)) * (10 * heightMap.at(i)) * (10 * heightMap.at(i));
        }
    }
    else
    {
        std::cout << "Heightmap Mismatch" << std::endl;
    }
}

void Terrain::setHeightMap(Texture &heightMap)
{
    if (heightMap.isValid())
    {
        const unsigned char *data = heightMap.data();
        int stepCountY = m_chunks;
        int stepCountX = m_chunks;
        float step = heightMap.width / (m_chunks + 1);
        std::vector<GLfloat> heightVector;

        for (int iy = 0; iy <= stepCountY; ++iy)
        {
            float y = iy * step;
            for (int ix = 0; ix <= stepCountX; ++ix)
            {
                float x = ix * step;
                int idx = (int(y) * heightMap.width + int(x)) * 3;
                heightVector.push_back(data[idx] / 255.0f);
            }
        }

        setHeightMap(heightVector, m_chunks, m_chunks);
    }
    else
    {
        std::cout << "Invalid tecture" << std::endl;
    }
}
