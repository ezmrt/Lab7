#ifndef MODEL_H
#define MODEL_H

#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include <iostream>
#include <vector>
#include <string>


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Model {
public:
    std::vector<Mesh> meshes;
    std::string directory;

    Model(std::string const& path) {
        loadModel(path);
    }

  
    void Draw(Shader& shader, glm::mat4 baseModel, float a1, float a2, float a3, float a4) {

        
        glm::vec3 pivot1(0.0f, 0.0f, -0.83f);
        glm::vec3 pivot2(-2.6f, 1.86f, -0.82f);
        glm::vec3 pivot3(-2.6f, 5.4f, 1.32f);
        glm::vec3 pivot4(-2.6f, 6.75f, -3.0f);

        
        glm::mat4 m1 = baseModel;
        m1 = glm::translate(m1, pivot1);
        m1 = glm::rotate(m1, glm::radians(a1), glm::vec3(0.0f, 1.0f, 0.0f));
        m1 = glm::translate(m1, -pivot1);

        glm::mat4 m2 = m1;
        m2 = glm::translate(m2, pivot2);
        m2 = glm::rotate(m2, glm::radians(a2), glm::vec3(1.0f, 0.0f, 0.0f));
        m2 = glm::translate(m2, -pivot2);

        glm::mat4 m3 = m2;
        m3 = glm::translate(m3, pivot3);
        m3 = glm::rotate(m3, glm::radians(a3), glm::vec3(1.0f, 0.0f, 0.0f));
        m3 = glm::translate(m3, -pivot3);

        glm::mat4 m4 = m3;
        m4 = glm::translate(m4, pivot4);
        m4 = glm::rotate(m4, glm::radians(a4), glm::vec3(1.0f, 0.0f, 0.0f));
        m4 = glm::translate(m4, -pivot4);

        for (unsigned int i = 0; i < meshes.size(); i++) {
            glm::mat4 currentModelMatrix;

            switch (i) {
            case 1: 
                currentModelMatrix = m1;
                break;
            case 2: 
                currentModelMatrix = m2;
                break;
            case 3: 
                currentModelMatrix = m3;
                break;
            case 4: 
                currentModelMatrix = m4;
                break;
            default:
                currentModelMatrix = baseModel;
                break;
            }

            shader.setMat4("model", currentModelMatrix);

            glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(currentModelMatrix)));
            shader.setMat3("normalMatrix", normalMatrix);

            meshes[i].Draw(shader);
        }
    }

private:
    void loadModel(std::string const& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            std::cout << "Mesh [" << meshes.size() << "] name: " << mesh->mName.C_Str() << std::endl;
            meshes.push_back(processMesh(mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            if (mesh->HasNormals()) {
                vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            }
            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        return Mesh(vertices, indices);
    }
};
#endif