#pragma once
#include "Window.hpp"
#include "Camera.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "Shader.hpp"
#include "imgui.h"
#include "Mesh.hpp"
#include "Error.hpp"
#include "IndexedMesh.hpp"
#include "Terrain.hpp"
#include "Texture.hpp"
#include "Model.hpp"
#include "TexturedMesh.hpp"
#include "Shapes.hpp"
#include "ObjLoader.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include "App.hpp"
class MyApp : public App
{
private:
    Camera m_camera;
    Shader m_shader;
    Shader m_lightShader;

    std::vector<TexturedMesh> m_meshes;
    Terrain m_terrain;
    IndexedMesh m_light;
    Texture m_brickWall;
    Texture m_heightMap;
    Texture m_palmTree;

    Model model;

    GLuint selectedMesh;

    ObjLoader m_loader;

    bool m_useTexture = true;
    bool m_wireFrame = false;
    float m_cameraSpeed = 10.0f;
    float m_ambientLight = 0.4f;
    float m_lightStrength = 10000.0f;
    char m_buffer[512];

public:
    MyApp();
    ~MyApp();

    virtual void startUP() final;
    virtual void update() final;

    static void processMouse(GLFWwindow *window, double xpos, double ypos);
    void pollKeyboard(double dt);
    static void windowResize(GLFWwindow *window, int width, int height);
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

    void loadMesh(std::vector<GLfloat> vertices, std::vector<GLfloat> texCoords, std::vector<GLfloat> normals, std::vector<GLuint> indices);
};