#pragma once

// Forward declaration of GLFWwindow data type
struct GLFWwindow;

// The window class
class Window {

public:
  GLFWwindow *window;
  int width, height;
  const char *name;

  // variable to host pointer to callback function for window resizing
  void (*windowSizeCallback)(GLFWwindow *, int, int) = nullptr;
  void setWindowSizeCallback(void (*callback)(GLFWwindow *, int, int));

  Window() = default;
  Window(int width, int height, const char *name);
  ~Window();

  bool shouldClose();
  void swapBuffers()const;
  void pollEvents()const;
  void setShouldClose(bool flag);
  void makeContextCurrent();

  void resize(bool flag)const;
  void updateWindowSize();

  GLFWwindow *getNativeWindow();
};