#include "MyApp.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Terrain.hpp"
#include "ObjLoader.hpp"
#include "Model.hpp"
float yaw = -90.0f, pitch = 0.0f;
float lastX = 400, lastY = 400;
bool firstMouse = true;

MyApp::MyApp() : m_camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f)),
                 m_shader("Shader/vertexShader.vert", "Shader/fragmentShader.frag"),
                 m_lightShader("Shader/lightVertexShader.vert", "Shader/lightFragmentShader.frag"),
                 m_brickWall("res/dirt.jpg"),
                 m_heightMap("res/snowdon.png"),
                 m_palmTree("res/1001_albedo.jpg"),
                 m_light(cubeIndexedVertices, cubeIndexedIndices),
                 m_terrain(5000, 500)
{
    model.loadModel("res/backpack.obj");

    for (int i = 0; i < model.getMeshes().size(); i++)
    {
        model.getMeshes().at(i).uploadGPU();
    }

    std::cout << "Model size = " << model.getMeshes().back().getVertices().size() << std::endl;
    m_light.computeNormals();
    m_light.uploadGPU();
    m_light.position = glm::vec3(1000.0f);
    m_light.updateModel();
    m_camera.update();

    m_terrain.linkTexture(&m_brickWall);
    m_terrain.setHeightMap(m_heightMap);
    m_terrain.computeNormals();
    m_terrain.setMaterial(glm::vec3(m_ambientLight), glm::vec3(0.9f), glm::vec3(0.0f), 1.0f);
    m_terrain.rotation = -90.0f;
    m_terrain.axis = glm::vec3(1.0f, 0.0f, 0.0f);
    m_terrain.updateModel();
    m_terrain.uploadGPU();

    m_shader.use();
    m_shader.setUniformVec3("lightPosition", m_light.position);
    m_shader.setUniformMat4("projection", m_camera.getProjection());
    m_shader.setUniformVec4("lightColor", glm::vec4(1.0f));
    m_shader.setUniformBool("useTexture", m_useTexture);

    m_lightShader.use();
    m_lightShader.setUniformVec4("color", glm::vec4(1.0f));
    m_lightShader.setUniformMat4("projection", m_camera.getProjection());

    glfwSetWindowUserPointer(m_window.getNativeWindow(), this);
    glfwSetFramebufferSizeCallback(m_window.getNativeWindow(), MyApp::windowResize);
    glfwSetKeyCallback(m_window.getNativeWindow(), MyApp::key_callback);
    glfwSetCursorPosCallback(m_window.getNativeWindow(), MyApp::processMouse);
    glfwSetInputMode(m_window.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    GLCALL(glEnable(GL_DEPTH_TEST));
    GLCALL(glClearColor(0.2, 0.34, 0.12, 1.0));
    GLCALL(glEnable(GL_CULL_FACE));
    GLCALL(glCullFace(GL_BACK));
}

void MyApp::loadMesh(std::vector<GLfloat> vertices, std::vector<GLfloat> texCoords, std::vector<GLfloat> normals, std::vector<GLuint> indices)
{
    std::vector<GLfloat> interleavedVerticesNormals;

    for (int i = 0; i < vertices.size() / 3; i++)
    {
        interleavedVerticesNormals.push_back(vertices.at(3 * i));
        interleavedVerticesNormals.push_back(vertices.at(3 * i + 1));
        interleavedVerticesNormals.push_back(vertices.at(3 * i + 2));

        interleavedVerticesNormals.push_back(texCoords.at(2 * i));
        interleavedVerticesNormals.push_back(texCoords.at(2 * i + 1));
    }

    m_meshes.emplace_back(interleavedVerticesNormals, indices);
    m_meshes.back().setMaterial(glm::vec3(m_ambientLight), glm::vec3(0.9f), glm::vec3(0.0f), 1.0f);
    m_meshes.back().uploadMaterialUniforms(m_shader);
    m_meshes.back().linkTexture(&m_palmTree);
    m_meshes.back().setNormals(normals);
    m_meshes.back().uploadGPU();
    selectedMesh = m_meshes.size() - 1;
}

MyApp::~MyApp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void MyApp::processMouse(GLFWwindow *window, double xpos, double ypos)
{

    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
    {
        firstMouse = true;
        return; // ImGui consumes the mouse, don't process camera
    }

    MyApp *app = static_cast<MyApp *>(glfwGetWindowUserPointer(window));

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        static float sensitivity = 0.5f;

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        pitch = glm::clamp(pitch, -89.0f, 89.0f);

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        app->m_camera.setFront(glm::normalize(direction));
        app->m_camera.update();
    }
    else
    {
        firstMouse = true;
    }
}

void MyApp::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureKeyboard)
    {
        return; // ImGui consumes the mouse, don't process camera
    }

    MyApp *app = static_cast<MyApp *>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(app->m_window.getNativeWindow(), true);
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        std::cout << "Space key was pressed\n";
}

void MyApp::pollKeyboard(double dt)
{
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureKeyboard)
    {
        return; // ImGui consumes the mouse, don't process camera
    }
    float speed = m_cameraSpeed * float(dt);
    if (glfwGetKey(m_window.getNativeWindow(), GLFW_KEY_W) == GLFW_PRESS)
    {
        m_camera.setPosition(m_camera.getPosition() + speed * m_camera.getFront());
    }
    if (glfwGetKey(m_window.getNativeWindow(), GLFW_KEY_S) == GLFW_PRESS)
    {
        m_camera.setPosition(m_camera.getPosition() - speed * m_camera.getFront());
    }
    if (glfwGetKey(m_window.getNativeWindow(), GLFW_KEY_A) == GLFW_PRESS)
    {
        m_camera.setPosition(m_camera.getPosition() - speed * m_camera.getRight());
    }
    if (glfwGetKey(m_window.getNativeWindow(), GLFW_KEY_D) == GLFW_PRESS)
    {
        m_camera.setPosition(m_camera.getPosition() + speed * m_camera.getRight());
    }
    if (glfwGetKey(m_window.getNativeWindow(), GLFW_KEY_UP) == GLFW_PRESS)
    {
        m_camera.setPosition(m_camera.getPosition() + speed * m_camera.getUp());
    }
    if (glfwGetKey(m_window.getNativeWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        m_camera.setPosition(m_camera.getPosition() - speed * m_camera.getUp());
    }
    m_camera.update();
}
void MyApp::windowResize(GLFWwindow *window, int width, int height)
{
    // Get the app instance
    MyApp *app = static_cast<MyApp *>(glfwGetWindowUserPointer(window));

    // Safety checks
    if (!app || width == 0 || height == 0)
        return;

    // CRITICAL: Update the OpenGL viewport first
    // This tells OpenGL the new rendering area dimensions
    GLCALL(glViewport(0, 0, width, height));

    app->m_camera.viewFustrum.aspectRatio = float(width) / float(height);
    app->m_camera.updateProjection();

    // Update the light shader with new projection matrix
    app->m_lightShader.use();
    app->m_lightShader.setUniformMat4("projection", app->m_camera.getProjection());

    // Update the main shader with new projection matrix
    app->m_shader.use();
    app->m_shader.setUniformMat4("projection", app->m_camera.getProjection());
}

void MyApp::startUP()
{
    // your logic
}
