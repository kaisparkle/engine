#pragma once

#include <asset/asset.h>

namespace Asset {
    class Texture : public IAsset {
    public:
        Texture(uint32_t assetId, const std::string& assetName) : IAsset(assetId, assetName) {}

        std::string filePath;
        void* apiObjects = nullptr;
    };
}