#include "Window.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

static bool isGLFWInit = false;

Window::Window(int width, int height, const char *name)
    : width(width), height(height)
{

  if (!isGLFWInit)
  {
    glfwInit();
    isGLFWInit = true;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(width, height, name, NULL, NULL);
  if (window == NULL)
  {
    throw std::runtime_error("Failed to create GLFW window");
    glfwTerminate();
    window = nullptr;
  }

  glfwSwapInterval(0);
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    window = nullptr;
  }
}
GLFWwindow *Window::getNativeWindow()
{
  if (window != nullptr)
  {
    return window;
  }
  else
  {
    std::cout << "Window pointer null " << std::endl;
    return nullptr;
  }
}

void Window::swapBuffers() const { glfwSwapBuffers(window); }

void Window::pollEvents() const { glfwPollEvents(); }

bool Window::shouldClose() { return glfwWindowShouldClose(window); }

void Window::setShouldClose(bool flag)
{

  glfwSetWindowShouldClose(window, flag);
}

void Window::updateWindowSize() { glfwGetWindowSize(window, &width, &height); }

void Window::setWindowSizeCallback(void (*callback)(GLFWwindow *, int, int))
{

  windowSizeCallback = callback;

  glfwSetWindowSizeCallback(window, windowSizeCallback);
  glfwSetFramebufferSizeCallback(window, windowSizeCallback);
}

void Window::makeContextCurrent()
{
  glfwMakeContextCurrent(window);
}

Window::~Window()
{

  if (window != nullptr)
  {
    glfwMakeContextCurrent(window);
    glfwDestroyWindow(window);
    window = nullptr;
  }
}