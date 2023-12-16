#pragma once

#include <string>
#include <assimp/scene.h>

namespace Tools {
    void import_model(const std::string &path);
    void process_assimp_node(aiNode* node, const aiScene* scene);
    void process_assimp_mesh(aiMesh* mesh, const aiScene* scene);

}