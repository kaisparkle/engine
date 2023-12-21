#pragma once

#include <vector>
#include <asset/texture.h>
#include <core/vertex.h>

namespace Core {
    struct Mesh {
        std::vector<Core::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Asset::Texture*> textures;
        void* apiObjects = nullptr;
    };
}