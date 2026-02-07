CXX = g++
CXXFLAGS = -Iinclude -pg
LDFLAGS = -lglfw -lassimp -ldl
DFLAGS = -DDEBUG
DGUIFLAGS = -lGL

SRCDIR = src
BINDIR = bin

SRCS:= $(SRCDIR)/glad.c \
	   $(SRCDIR)/Camera.cpp \
	   $(SRCDIR)/Shader.cpp \
	   $(SRCDIR)/BufferObjects/VAO.cpp \
	   $(SRCDIR)/BufferObjects/VBO.cpp \
	   $(SRCDIR)/BufferObjects/EBO.cpp \
	   $(SRCDIR)/Window.cpp \
	   $(SRCDIR)/Mesh/Mesh.cpp \
	   $(SRCDIR)/Mesh/IndexedMesh.cpp \
	   $(SRCDIR)/Mesh/TexturedMesh.cpp \
	   $(SRCDIR)/Model.cpp \
	   $(SRCDIR)/Texture.cpp\
	   $(SRCDIR)/Imgui/imgui.cpp\
	   $(SRCDIR)/Imgui/imgui_demo.cpp\
	   $(SRCDIR)/Imgui/imgui_draw.cpp\
	   $(SRCDIR)/Imgui/imgui_impl_glfw.cpp\
	   $(SRCDIR)/Imgui/imgui_impl_opengl2.cpp\
	   $(SRCDIR)/Imgui/imgui_impl_opengl3.cpp\
	   $(SRCDIR)/Imgui/imgui_tables.cpp\
	   $(SRCDIR)/Imgui/imgui_widgets.cpp\
	   $(SRCDIR)/main.cpp

GUISRCS:= $(SRCDIR)/glad.c \
	   $(SRCDIR)/Camera.cpp \
	   $(SRCDIR)/Shader.cpp \
	   $(SRCDIR)/BufferObjects/VAO.cpp \
	   $(SRCDIR)/BufferObjects/VBO.cpp \
	   $(SRCDIR)/BufferObjects/EBO.cpp \
	   $(SRCDIR)/Window.cpp \
	   $(SRCDIR)/Mesh/Mesh.cpp \
	   $(SRCDIR)/Mesh/IndexedMesh.cpp \
	   $(SRCDIR)/Mesh/TexturedMesh.cpp \
	   $(SRCDIR)/Model.cpp \
	   $(SRCDIR)/Texture.cpp\
	   $(SRCDIR)/Imgui/imgui.cpp\
	   $(SRCDIR)/Imgui/imgui_demo.cpp\
	   $(SRCDIR)/Imgui/imgui_draw.cpp\
	   $(SRCDIR)/Imgui/imgui_impl_glfw.cpp\
	   $(SRCDIR)/Imgui/imgui_impl_opengl2.cpp\
	   $(SRCDIR)/Imgui/imgui_impl_opengl3.cpp\
	   $(SRCDIR)/Imgui/imgui_tables.cpp\
	   $(SRCDIR)/Imgui/imgui_widgets.cpp\
	   $(SRCDIR)/App.cpp\
	   $(SRCDIR)/MyApp.cpp\
	   $(SRCDIR)/Mesh/Terrain.cpp\
	   $(SRCDIR)/Loaders/ObjLoader.cpp\
	   $(SRCDIR)/GUIDebug.cpp

TARGET = $(BINDIR)/my_program
DEBUG_TARGET = $(BINDIR)/my_debug_program
DEBUG_TARGET_GUI = $(BINDIR)/my_debug_gui_program

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS) | $(BINDIR)
	$(CXX) $(SRCS) $(CXXFLAGS) $(LDFLAGS) -o $@

debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(SRCS) | $(BINDIR)
	$(CXX) $(SRCS) $(CXXFLAGS) $(DFLAGS) $(LDFLAGS) -o $@

gui: $(DEBUG_TARGET_GUI)

$(DEBUG_TARGET_GUI): $(GUISRCS) | $(BINDIR)
	$(CXX) $(GUISRCS) $(CXXFLAGS) $(DFLAGS) $(DGUIFLAGS) $(LDFLAGS) -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	@echo "Cleaning up..."
	@rm -rf $(BINDIR)