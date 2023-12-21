#pragma once

#include <string>
#include <assimp/scene.h>
#include <components/model.h>

namespace Tools {
    Asset::Model* import_model(const std::string &path);
    void process_assimp_node(aiNode* node, const aiScene* scene, Asset::Model* model, const std::string &path);
    void process_assimp_mesh(aiMesh* mesh, const aiScene* scene, Asset::Model* model, const std::string &path);

}