#include "MyApp.hpp"
float globalAmbient = 0.8f;

void MyApp::update()
{
    ImGui::Begin("Control"); // Create a window called "My New Window"

    if (ImGui::Checkbox("Enable Texture", &m_useTexture)) // Create a button labeled "Click Me"
    {
        if (m_useTexture)
        {

            m_shader.setUniformBool("useTexture", m_useTexture);
        }
        else
        {
            m_shader.setUniformBool("useTexture", m_useTexture);
        }
    }

    if (ImGui::Checkbox("Skeleton", &m_wireFrame))
    {
        if (m_wireFrame)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    ImGui::PushItemWidth(100);
    if (ImGui::DragFloat("FOV", &m_camera.viewFustrum.FOV, 0.1f, 5.0f, 180.0f))
    {
        m_camera.updateProjection();
        m_shader.use();
        m_shader.setUniformMat4("projection", m_camera.getProjection());
        m_lightShader.use();
        m_lightShader.setUniformMat4("projection", m_camera.getProjection());
    }

    if (ImGui::DragFloat("Light Strength", &m_lightStrength, 10.0f, 10000.0f, 50000.0f, "%.2f"))
    {
        m_shader.use();
        m_shader.setUniformFloat("lightStrength", m_lightStrength);
    }

    if (ImGui::DragFloat("Far Plane", &m_camera.viewFustrum.farPlane, 1.0f, 100.0f, 5000.0f))
    {
        m_camera.updateProjection();
        m_shader.use();
        m_shader.setUniformMat4("projection", m_camera.getProjection());
        m_lightShader.use();
        m_lightShader.setUniformMat4("projection", m_camera.getProjection());
    }

    if (ImGui::DragFloat("Camera Speed", &m_cameraSpeed, 0.1f, 5.0f, 50.0f))
    {
    }

    ImGui::End();

    ImGui::Begin("Object");
    ImGui::InputText("source", m_buffer, 512);

    static float x = 0.0f;
    static float y = 0.0f;
    static float z = 0.0f;

    if (m_meshes.size() != 0)
    {
        TexturedMesh &selected = m_meshes.at(selectedMesh);
    }
    if (ImGui::Button("Add object"))
    {
        m_loader.loadMesh(std::string(m_buffer));
        loadMesh(m_loader.vertices, m_loader.texCoords, m_loader.normals, m_loader.indices);
    }
    ImGui::PushItemWidth(50);
    if (ImGui::DragFloat("X", &x, 0.1f))
    {
        if (m_meshes.size() != 0)
        {
            m_meshes.back().position.x = x;
            m_meshes.back().updateModel();
        }
    }
    ImGui::SameLine(0.0f, 20.0f);
    if (ImGui::DragFloat("Y", &y, 0.1f))
    {
        if (m_meshes.size() != 0)
        {

            m_meshes.back().position.y = y;
            m_meshes.back().updateModel();
        }
    }
    ImGui::SameLine(0.0f, 20.0f);
    if (ImGui::DragFloat("Z", &z, 0.1f))
    {
        if (m_meshes.size() != 0)
        {

            m_meshes.back().position.z = z;
            m_meshes.back().updateModel();
        }
    }

    if (ImGui::BeginListBox("Objects"))
    {
        for (int i = 0; i < m_meshes.size(); i++)
        {
            bool is_selected = (selectedMesh == i);
            if (ImGui::Selectable(std::to_string(i).c_str(), is_selected))
                selectedMesh = i;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    ImGui::End();

    m_lightShader.use();
    m_lightShader.setUniformMat4("view", m_camera.getView());
    m_light.draw(m_lightShader);

    m_shader.use();
    m_shader.setUniformMat4("view", m_camera.getView());
    m_shader.setUniformVec3("viewPosition", m_camera.getPosition());

    m_terrain.draw(m_shader);
    for (int i = 0; i < m_meshes.size(); i++)
    {
        m_meshes.at(i).draw(m_shader);
    }
    m_palmTree.bind();
    model.draw(m_shader);
    m_palmTree.unbind();

    pollKeyboard(m_deltaTime);

    static float fps[60] = {60.0f}; // initialized to 60 FPS
    static int index = 0;

    float currentFPS = (m_deltaTime > 0.0001f) ? (1.0f / m_deltaTime) : 0.0f;
    fps[index] = currentFPS;
    index = (index + 1) % 60;

    float avgFPS = 0.0f;
    for (int i = 0; i < 60; ++i)
    {
        avgFPS += fps[i];
    }
    avgFPS /= 60.0f;
    ImGui::Begin("Information");

    glm::vec3 pos = m_camera.getPosition();
    ImGui::Text("Camera x = %f \nCamera y = %f\nCamera z = %f\n", pos.x, pos.y, pos.z);
    ImGui::NewLine();
    ImGui::Text("FPS = %lf", avgFPS);
    ImGui::End();
}

int main()
{
    MyApp app;
    app.run();
    return 0;
}