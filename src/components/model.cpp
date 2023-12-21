#include <typeinfo>
#include <asset/model.h>
#include <asset/assetmanager.h>
#include <components/model.h>

namespace Component {
    uint32_t Model::get_asset() {
        return assetId;
    }

    void Model::set_asset(uint32_t id) {
        auto* asset = Asset::AssetManager::get_instance()->get_asset<Asset::Model>(id);
        if(asset) {
            auto* result = dynamic_cast<Asset::Model*>(asset);
            if(result) {
                assetId = result->get_id();
            }
        }
    }
}