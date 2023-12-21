#pragma once

#include <asset/asset.h>
#include <core/mesh.h>

namespace Asset {
    class Model : public IAsset {
    public:
        Model(uint32_t assetId, const std::string& assetName) : IAsset(assetId, assetName) {}

        std::vector<Core::Mesh*> meshes;
    };
}