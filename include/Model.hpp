#pragma once
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
    void draw(Shader &shader);
    void loadModel(std::string directory);

    std::vector<TexturedMesh> &getMeshes();
};