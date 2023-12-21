#include <asset/assetmanager.h>

namespace Asset {
    AssetManager* AssetManager::create_instance() {
        assert(instance == nullptr);
        instance = new AssetManager;
        return instance;
    }

    void AssetManager::destroy_instance() {
        assert(instance != nullptr);
        delete instance;
        instance = nullptr;
    }

    AssetManager* AssetManager::get_instance() {
        assert(instance != nullptr);
        return instance;
    }

    void AssetManager::init() {
        for(uint32_t id = 0; id < MAX_ASSETS; id++) {
            availableIds.push(id);
        }

        assetCount = 0;
    }

    void AssetManager::cleanup() {
        assets.clear();
    }

    void AssetManager::destroy_asset(uint32_t id) {
        assets.erase(id);
    }

    std::vector<uint32_t> AssetManager::get_active_ids() {
        // TODO: this is probably piss slow
        OPTICK_EVENT();
        std::vector<uint32_t> ids;
        for(auto &asset : assets) {
            ids.push_back(asset.first);
        }
        return ids;
    }
}