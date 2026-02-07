#pragma once
#include <vector>
#include <glad/glad.h>

static std::vector<GLfloat> cubeVertices = {
    -0.5f, -0.5f, 0.5f, // 0: Bottom-left
    0.5f, -0.5f, 0.5f,  // 1: Bottom-right
    0.5f, 0.5f, 0.5f,   // 2: Top-right
    0.5f, 0.5f, 0.5f,   // 2: Top-right
    -0.5f, 0.5f, 0.5f,  // 3: Top-left
    -0.5f, -0.5f, 0.5f, // 0: Bottom-left
                        // Back face (-Z)
    -0.5f, -0.5f,
    -0.5f,               // 4: Bottom-left (when looking from front)
    -0.5f, 0.5f, -0.5f,  // 5: Top-left
    0.5f, 0.5f, -0.5f,   // 6: Top-right
    0.5f, 0.5f, -0.5f,   // 6: Top-right
    0.5f, -0.5f, -0.5f,  // 7: Bottom-right
    -0.5f, -0.5f, -0.5f, // 4: Bottom-left
                         // Left face (-X)
    -0.5f, 0.5f, 0.5f,   // 8: Top-front
    -0.5f, -0.5f, 0.5f,  // 9: Bottom-front
    -0.5f, -0.5f, -0.5f, // 10: Bottom-back
    -0.5f, -0.5f, -0.5f, // 10: Bottom-back
    -0.5f, 0.5f, -0.5f,  // 11: Top-back
    -0.5f, 0.5f, 0.5f,   // 8: Top-front
                         // Right face (+X)
    0.5f, 0.5f, 0.5f,    // 12: Top-front
    0.5f, -0.5f, -0.5f,  // 13: Bottom-back
    0.5f, -0.5f, 0.5f,   // 14: Bottom-front
    0.5f, 0.5f, -0.5f,   // 15: Top-back
    0.5f, -0.5f, -0.5f,  // 13: Bottom-back
    0.5f, 0.5f, 0.5f,    // 12: Top-front
                         // Top face (+Y)
    -0.5f, 0.5f, 0.5f,   // 16: Front-left
    0.5f, 0.5f, 0.5f,    // 17: Front-right
    0.5f, 0.5f, -0.5f,   // 18: Back-right
    0.5f, 0.5f, -0.5f,   // 18: Back-right
    -0.5f, 0.5f, -0.5f,  // 19: Back-left
    -0.5f, 0.5f, 0.5f,   // 16: Front-left
                         // Bottom face (-Y)
    -0.5f, -0.5f, 0.5f,  // 20: Front-left
    0.5f, -0.5f, -0.5f,  // 21: Back-right
    0.5f, -0.5f, 0.5f,   // 22: Front-right
    0.5f, -0.5f, -0.5f,  // 21: Back-right
    -0.5f, -0.5f, 0.5f,  // 20: Front-left
    -0.5f, -0.5f, -0.5f};

static std::vector<GLfloat> cubeTexCoords = {
    // Face 1 (Front - Z+)
    0.0f, 0.0f, 0.0f, // Bottom-left
    1.0f, 0.0f, 0.0f, // Bottom-right
    1.0f, 1.0f, 0.0f, // Top-right
    0.0f, 1.0f, 0.0f, // Top-left

    // Face 2 (Back - Z-)
    0.0f, 0.0f, 0.0f, // Bottom-left
    1.0f, 0.0f, 0.0f, // Bottom-right
    1.0f, 1.0f, 0.0f, // Top-right
    0.0f, 1.0f, 0.0f, // Top-left

    // Face 3 (Right - X+)
    0.0f, 0.0f, 0.0f, // Bottom-left
    1.0f, 0.0f, 0.0f, // Bottom-right
    1.0f, 1.0f, 0.0f, // Top-right
    0.0f, 1.0f, 0.0f, // Top-left

    // Face 4 (Left - X-)
    0.0f, 0.0f, 0.0f, // Bottom-left
    1.0f, 0.0f, 0.0f, // Bottom-right
    1.0f, 1.0f, 0.0f, // Top-right
    0.0f, 1.0f, 0.0f, // Top-left

    // Face 5 (Top - Y+)
    0.0f, 0.0f, 0.0f, // Bottom-left
    1.0f, 0.0f, 0.0f, // Bottom-right
    1.0f, 1.0f, 0.0f, // Top-right
    0.0f, 1.0f, 0.0f, // Top-left

    // Face 6 (Bottom - Y-)
    0.0f, 0.0f, 0.0f, // Bottom-left
    1.0f, 0.0f, 0.0f, // Bottom-right
    1.0f, 1.0f, 0.0f, // Top-right
    0.0f, 1.0f, 0.0f  // Top-left
};

// 8 unique vertices of a cube
static std::vector<GLfloat> cubeIndexedVertices = {
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
static std::vector<GLuint> cubeIndexedIndices = {
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