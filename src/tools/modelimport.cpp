#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <asset/assetmanager.h>
#include <core/vertex.h>
#include <tools/modelimport.h>

namespace Tools {
    Asset::Model* import_model(const std::string &path) {
        Assimp::Importer importer;
        const aiScene* modelScene = importer.ReadFile(path,
                                                      aiProcess_Triangulate |
                                                      aiProcess_FlipUVs |
                                                      aiProcess_CalcTangentSpace);

        if(!modelScene || modelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !modelScene->mRootNode) {
            std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
            return nullptr;
        }

        auto* asset = Asset::AssetManager::get_instance()->create_asset<Asset::Model>(
                aiScene::GetShortFilename(path.c_str()));

        process_assimp_node(modelScene->mRootNode, modelScene, asset, path);

        return asset;
    }

    void process_assimp_node(aiNode* node, const aiScene* scene, Asset::Model* model, const std::string &path) {
        for(size_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            process_assimp_mesh(mesh, scene, model, path);
        }
        for(size_t i = 0; i < node->mNumChildren; i++) {
            process_assimp_node(node->mChildren[i], scene, model, path);
        }
    }

    void process_assimp_mesh(aiMesh* mesh, const aiScene* scene, Asset::Model* model, const std::string &path) {
        auto* newMesh = new Core::Mesh;
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
            newMesh->vertices.push_back(newVertex);
        }
        for (size_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; j++) {
                newMesh->indices.push_back(face.mIndices[j]);
            }
        }

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        if(material->GetTextureCount(aiTextureType_DIFFUSE)) {
            aiString str;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
            auto *texture = Asset::AssetManager::get_instance()->create_asset<Asset::Texture>(str.C_Str());
            texture->filePath = path.substr(0, path.find_last_of('/')) + '/' + str.C_Str();
            newMesh->textures.push_back(texture);
        }

        model->meshes.push_back(newMesh);
    }
}