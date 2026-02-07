#pragma once
#include"imgui.h"
#include"Window.hpp"
#include<GLFW/glfw3.h>
class App
{
    protected:
        Window m_window;
        ImVec4 m_clearColor;
        ImGuiIO* m_io;
        float m_deltaTime;
        float m_lastFrame;
    public:
    App();
    void run();
    virtual void startUP() = 0;
    virtual void update() = 0;
};