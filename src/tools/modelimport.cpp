#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <entities/entitymanager.h>
#include <components/mesh.h>
#include <core/vertex.h>
#include <tools/modelimport.h>

namespace Tools {
    void import_model(const std::string &path) {
        Assimp::Importer importer;
        const aiScene* modelScene = importer.ReadFile(path,
                                                      aiProcess_Triangulate |
                                                      aiProcess_FlipUVs |
                                                      aiProcess_CalcTangentSpace);

        if(!modelScene || modelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !modelScene->mRootNode) {
            std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
            return;
        }

        process_assimp_node(modelScene->mRootNode, modelScene);
    }

    void process_assimp_node(aiNode* node, const aiScene* scene) {
        for(size_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            process_assimp_mesh(mesh, scene);
        }
        for(size_t i = 0; i < node->mNumChildren; i++) {
            process_assimp_node(node->mChildren[i], scene);
        }
    }

    void process_assimp_mesh(aiMesh* mesh, const aiScene* scene) {
        uint32_t entId = Entity::EntityManager::get_instance()->create_entity();
        Entity::Entity* ent = Entity::EntityManager::get_instance()->get_entity(entId);
        ent->set_name(std::string(mesh->mName.C_Str()));
        ent->add_component<Component::Mesh>();
        auto* meshComp = ent->get_component<Component::Mesh>();

        for(size_t i = 0; i < mesh->mNumVertices; i++) {
            Core::Vertex newVertex{};
            newVertex.position.x = mesh->mVertices[i].x;
            newVertex.position.y = mesh->mVertices[i].y;
            newVertex.position.z = mesh->mVertices[i].z;
            if (mesh->HasNormals()) {
                newVertex.normal.x = mesh->mNormals[i].x;
                newVertex.normal.y = mesh->mNormals[i].y;
                newVertex.normal.z = mesh->mNormals[i].z;
            }
            if (mesh->mTextureCoords[0]) {
                newVertex.uv.x = mesh->mTextureCoords[0][i].x;
                newVertex.uv.y = mesh->mTextureCoords[0][i].y;
            }
            if (mesh->HasTangentsAndBitangents()) {
                newVertex.tangent.x = mesh->mTangents[i].x;
                newVertex.tangent.y = mesh->mTangents[i].y;
                newVertex.tangent.z = mesh->mTangents[i].z;
                newVertex.bitangent.x = mesh->mBitangents[i].x;
                newVertex.bitangent.y = mesh->mBitangents[i].y;
                newVertex.bitangent.z = mesh->mBitangents[i].z;
            }
            meshComp->vertices.push_back(newVertex);
        }
        for (size_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; j++) {
                meshComp->indices.push_back(face.mIndices[j]);
            }
        }
    }
}