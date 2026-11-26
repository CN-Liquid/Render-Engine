#include "Model.hpp"
#include "Texture.hpp"
#include "TexturedMesh.hpp"
#include <assimp/defs.h>
#include <assimp/material.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <iostream>

static std::string fixSlashes(std::string s) {
  for (char &c : s)
    if (c == '\\')
      c = '/';
  return s;
}

void Model::loadModel(std::string path) {
  m_path = path;
  Assimp::Importer import;
  const aiScene *scene =
      import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                aiProcess_JoinIdenticalVertices);

  if(scene == nullptr){
      std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
      return;
  }

  m_textures.reserve(scene->mNumMaterials);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }

  // Extract directory path
  path = path.substr(0, path.find_last_of('/'));

  processNode(scene->mRootNode, scene);

  m_textures.at(0).saveTextureAsPNG("text.png");
}

void Model::processNode(aiNode *node, const aiScene *scene) {

  // process all the node's meshes (if any)
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

    if (!mesh) {
      std::cout << "ERROR: Mesh is null!" << std::endl;
      continue;
    }

    TexturedMesh processedMesh = processMesh(mesh, scene);
    m_meshes.push_back(std::move(processedMesh));
  }

  // then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

TexturedMesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::cout << "Processing mesh with " << mesh->mNumVertices << " vertices"
            << std::endl;

  std::vector<GLfloat> vertices;
  std::vector<GLfloat> normals;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    // Vertices
    vertices.push_back(mesh->mVertices[i].x);
    vertices.push_back(mesh->mVertices[i].y);
    vertices.push_back(mesh->mVertices[i].z);

    // Texture coordinates
    if (mesh->mTextureCoords[0]) {
      vertices.push_back(mesh->mTextureCoords[0][i].x);
      vertices.push_back(mesh->mTextureCoords[0][i].y);
    } else {
      vertices.push_back(0.0f);
      vertices.push_back(0.0f);
    }

    // Normals
    if (mesh->mNormals) {
      normals.push_back(mesh->mNormals[i].x);
      normals.push_back(mesh->mNormals[i].y);
      normals.push_back(mesh->mNormals[i].z);
    } else {
      normals.push_back(0.0f);
      normals.push_back(1.0f);
      normals.push_back(0.0f);
    }
  }

  // Process faces
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
  TexturedMesh tm = TexturedMesh(vertices, indices, normals);
  if (scene->mMaterials && mesh->mMaterialIndex < scene->mNumMaterials) {

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    ai_int textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
    std::cout << "textures size = " << textureCount << std::endl;

    if (textureCount > 0) {
      aiString str;
      material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
      std::string path = m_path.substr(0, m_path.find_last_of('/'));
      std::string fullPath = fixSlashes(path) + "/" + fixSlashes(str.C_Str());

      std::cout << "Texture Path = " << str.C_Str() << std::endl;
      Texture texture(fullPath);
      m_textures.push_back(std::move(texture));
      tm.linkTexture(&m_textures.back());
    }
  }
  return tm;
}

std::vector<TexturedMesh> &Model::getMeshes() { return m_meshes; }

std::vector<Texture> &Model::getTextures() { return m_textures; }

void Model::draw(Shader &shader) {

  for (int i = 0; i < m_meshes.size(); i++) {

    m_meshes.at(i).draw(shader);
  }
}

void Model::updateModel(){
    for(int i=0;i<m_meshes.size();i++)
    {
        m_meshes.at(i).position = position;
        m_meshes.at(i).scale = scale;
        m_meshes.at(i).rotation=rotation;
        m_meshes.at(i).axis = axis;

        m_meshes.at(i).updateModel();
    }
}
