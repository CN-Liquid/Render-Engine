# Render Engine

A lightweight, hardware-accelerated 3D rendering engine built in C++ using OpenGL, GLFW, and Glad. This engine features asset loading via Assimp, custom terrain generation, and a debugging suite powered by Dear ImGui.

## Features

- **OpenGL Abstraction Layer:** Clean, object-oriented wrappers for Vertex Arrays (VAO), Vertex Buffers (VBO), Element Buffers (EBO), Textures, and Shaders.
- **Asset Loading Suite:** Integrated **Assimp** for parsing and loading complex 3D scenes alongside a specialized custom **ObjLoader**.
- **Dynamic Camera System:** Flexible perspective camera matrix transformations handling translation, orientation, and zoom tracking.
- **Mesh & Terrain Architecture:** Built-in structures for standard, indexed, and textured geometry meshes, with extended capabilities for custom terrain generation.
- **Immediate-Mode Debug GUI:** Feature-rich diagnostics and parameter manipulation tools overlay powered by **Dear ImGui** (using GLFW + OpenGL3 backends).

## Dependencies

### System Prerequisites

To build and run this engine, ensure you have a modern C++ compiler supporting `g++`, `make`, and the following libraries installed on your host system:

#### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install build-essential libglfw3-dev libassimp-dev libgl1-mesa-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev
```

#### Linux (Arch Linux)

```bash
sudo pacman -Syu base-devel glfw-x11 assimp mesa
```

### Bundled Third-Party Utilities

The following dependencies are vendor-packaged and directly compiled alongside the source codebase:

- **GLAD:** Multi-language OpenGL function loader.
- **Dear ImGui:** Bloat-free graphical user interface library for C++.

## Building and Running

This project uses a flat `Makefile` configuration pipeline.

```bash
make
./bin/my_program
```
