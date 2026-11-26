#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.hpp"
#include "Texture.hpp"
#include "TexturedMesh.hpp"
class Model
{
private:
    std::string m_path;
    std::vector<TexturedMesh> m_meshes;
    std::vector<Texture> m_textures;

    void processNode(aiNode *node, const aiScene *scene);

    TexturedMesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> loadMaterialTexture(aiMaterial *mat, aiTextureType type,
                                             std::string typeName);

public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f);
    float rotation = 0.0f;
    void draw(Shader &shader);
    void loadModel(std::string directory);
    void updateModel();

    std::vector<TexturedMesh> &getMeshes();
    // Temporary
    std::vector<Texture > &getTextures();
};
