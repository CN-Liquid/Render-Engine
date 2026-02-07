#include "Model.hpp"
#include <iostream>

void Model::loadModel(std::string path)
{
    m_path = path;
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    // Extract directory path
    path = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{

    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

        if (!mesh)
        {
            std::cout << "ERROR: Mesh is null!" << std::endl;
            continue;
        }

        TexturedMesh processedMesh = processMesh(mesh, scene);
        m_meshes.push_back(std::move(processedMesh));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

TexturedMesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::cout << "Processing mesh with " << mesh->mNumVertices << " vertices" << std::endl;

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // Vertices
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        // Texture coordinates
        if (mesh->mTextureCoords[0])
        {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        // Normals
        if (mesh->mNormals)
        {
            normals.push_back(mesh->mNormals[i].x);
            normals.push_back(mesh->mNormals[i].y);
            normals.push_back(mesh->mNormals[i].z);
        }
        else
        {
            normals.push_back(0.0f);
            normals.push_back(1.0f);
            normals.push_back(0.0f);
        }

        if (false)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            aiString str;
            material->GetTexture(aiTextureType_DIFFUSE, i, &str);
            m_path = m_path.substr(0, m_path.find_last_of('/'));
            std::string fullPath = m_path + "/" + str.C_Str();
            Texture texture(fullPath);
            m_textures.push_back(texture);
        }
    }

    // Process faces
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return TexturedMesh(vertices, indices, normals);
}

std::vector<TexturedMesh> &Model::getMeshes()
{
    return m_meshes;
}

void Model::draw(Shader &shader)
{

    for (int i = 0; i < m_meshes.size(); i++)
    {
        if (false)
        {
            m_textures.at(i).bind();
            m_meshes.at(i).draw(shader);
            m_textures.at(i).unbind();
        }
        else
        {
            m_meshes.at(i).draw(shader);
        }
    }
}