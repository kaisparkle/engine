#pragma once

#include <vector>
#include <asset/model.h>
#include <component/component.h>

namespace Component {
    class Model : public IComponent {
    public:
        Model(Entity::Entity* entity) : IComponent(entity) {}

        uint32_t get_asset();
        void set_asset(uint32_t id);
    private:
        uint32_t assetId;
    };
};