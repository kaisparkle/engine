#pragma once

#include <queue>
#include <map>
#include <cstdint>
#include <cassert>
#include <optick.h>
#include <asset/asset.h>

// TODO: cvars?
#define MAX_ASSETS 16384

namespace Asset {
    class AssetManager {
    public:
        // create the AssetManager singleton
        static AssetManager* create_instance();
        // destroy the AssetManager singleton
        static void destroy_instance();
        // return the AssetManager singleton
        static AssetManager* get_instance();

        // initialize the AssetManager
        void init();
        // clean up the AssetManager
        void cleanup();

        // add a new asset of the given type
        template<typename AssetType>
        AssetType* create_asset(const std::string& name) {
            OPTICK_EVENT();
            assert(assetCount < MAX_ASSETS);
            uint32_t id = availableIds.front();
            availableIds.pop();
            assetCount++;

            auto* asset = new AssetType(id, name);
            assets[id] = asset;
            return asset;
        }

        // destroy the asset of given ID
        void destroy_asset(uint32_t id);

        // returns a pointer to asset of given ID, if it exists
        template<typename AssetType>
        AssetType* get_asset(uint32_t id) {
            AssetType* result = nullptr;
            result = dynamic_cast<AssetType*>(assets.at(id));
            return result;
        }

        // returns a vector of all active IDs
        std::vector<uint32_t> get_active_ids();
    private:
        inline static AssetManager* instance;

        std::queue<uint32_t> availableIds;
        uint32_t assetCount;
        // TODO: is a map here a good idea?
        std::map<uint32_t, IAsset*> assets;
    };
}