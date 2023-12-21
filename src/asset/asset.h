#pragma once

#include <string>
#include <cstdint>

namespace Asset {
    class IAsset {
    public:
        IAsset(uint32_t assetId, const std::string& assetName) {id = assetId; name = assetName;}
        virtual ~IAsset() = default;

        const std::string& get_name() {return name;}
        uint32_t get_id() {return id;}
    private:
        std::string name;
        uint32_t id;
    };
}