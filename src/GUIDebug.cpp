#include "MyApp.hpp"
#include "VBO.hpp"
#include "imgui.h"
#include <glm/ext/vector_float3.hpp>
float globalAmbient = 0.8f;

void MyApp::update() {
  ImGui::Begin("Control"); // Create a window called "My New Window"

  if (ImGui::Checkbox("Enable Texture",
                      &m_useTexture)) // Create a button labeled "Click Me"
  {
    if (m_useTexture) {

      m_shader.setUniformBool("useTexture", m_useTexture);
    } else {
      m_shader.setUniformBool("useTexture", m_useTexture);
    }
  }

  if (ImGui::Checkbox("Skeleton", &m_wireFrame)) {
    if (m_wireFrame) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
  }

  ImGui::PushItemWidth(100);
  if (ImGui::DragFloat("FOV", &m_camera.viewFustrum.FOV, 0.1f, 5.0f, 180.0f)) {
    m_camera.updateProjection();
    m_shader.use();
    m_shader.setUniformMat4("projection", m_camera.getProjection());
    m_lightShader.use();
    m_lightShader.setUniformMat4("projection", m_camera.getProjection());
  }

  if (ImGui::DragFloat("Light Strength", &m_lightStrength, 10.0f, 10000.0f,
                       50000.0f, "%.2f")) {
    m_shader.use();
    m_shader.setUniformFloat("lightStrength", m_lightStrength);
  }

  if (ImGui::DragFloat("Far Plane", &m_camera.viewFustrum.farPlane, 1.0f,
                       100.0f, 5000.0f)) {
    m_camera.updateProjection();
    m_shader.use();
    m_shader.setUniformMat4("projection", m_camera.getProjection());
    m_lightShader.use();
    m_lightShader.setUniformMat4("projection", m_camera.getProjection());
  }

  if (ImGui::DragFloat("Camera Speed", &m_cameraSpeed, 0.1f, 5.0f, 50.0f)) {
  }

  ImGui::End();

  ImGui::Begin("Object");
  ImGui::InputText("source", m_buffer, 512);

  static float x = 0.0f;
  static float y = 0.0f;
  static float z = 0.0f;

  if (m_models.size() != 0) {
    Model &selected = m_models.at(selectedModel);
  }
  if (ImGui::Button("Add object")) {
    Model model;
    model.loadModel(m_buffer);
    for(int i=0;i<model.getMeshes().size();i++)
    {
        model.getMeshes().at(i).uploadGPU();
    }
    m_models.push_back(std::move(model));
  }
  ImGui::PushItemWidth(50);
  if (ImGui::DragFloat("X", &x, 0.1f)) {
    if (m_models.size() != 0) {
        m_models.at(selectedModel).position = glm::vec3(x,y,z);
      m_models.at(selectedModel).updateModel();
    }
  }
  ImGui::SameLine(0.0f, 20.0f);
  if (ImGui::DragFloat("Y", &y, 0.1f)) {
    if (m_models.size() != 0) {
m_models.at(selectedModel).position = glm::vec3(x,y,z);
m_models.at(selectedModel).updateModel();
    }
  }
  ImGui::SameLine(0.0f, 20.0f);
  if (ImGui::DragFloat("Z", &z, 0.1f)) {
    if (m_models.size() != 0) {
        m_models.at(selectedModel).position = glm::vec3(x,y,z);
 m_models.at(selectedModel).updateModel();
    }
  }



   if (ImGui::BeginListBox("Objects")) {
    for (int i = 0; i < m_models.size(); i++) {
      bool is_selected = (selectedModel == i);
      if (ImGui::Selectable(std::to_string(i).c_str(), is_selected)){
        selectedModel = i;
        x = m_models.at(selectedModel).position.x;
        y = m_models.at(selectedModel).position.y;
        z = m_models.at(selectedModel).position.z;

      }

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

  for(int i=0 ; i<m_models.size();i++)
  {
      m_models.at(i).draw(m_shader);
  }

  pollKeyboard(m_deltaTime);

  static float fps[60] = {60.0f}; // initialized to 60 FPS
  static int index = 0;

  float currentFPS = (m_deltaTime > 0.0001f) ? (1.0f / m_deltaTime) : 0.0f;
  fps[index] = currentFPS;
  index = (index + 1) % 60;

  float avgFPS = 0.0f;
  for (int i = 0; i < 60; ++i) {
    avgFPS += fps[i];
  }
  avgFPS /= 60.0f;
  ImGui::Begin("Information");

  glm::vec3 pos = m_camera.getPosition();
  ImGui::Text("Camera x = %f \nCamera y = %f\nCamera z = %f\n", pos.x, pos.y,
              pos.z);
  ImGui::NewLine();
  ImGui::Text("FPS = %lf", avgFPS);
  ImGui::End();
}

int main() {
  MyApp app;
  app.run();
  return 0;
}
