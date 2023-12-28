#include <physics/physicsmanager.h>
#include <asset/assetmanager.h>
#include <component/model.h>
#include <component/rigidbody.h>

namespace Component {
    RigidbodyStatic::RigidbodyStatic(Entity::Entity *entity) : IComponent(entity) {
        auto* modelComp = get_parent()->get_component<Component::Model>();
        if(modelComp) {
            auto* modelAsset = Asset::AssetManager::get_instance()->get_asset<Asset::Model>(modelComp->get_asset());
            for(auto* mesh : modelAsset->meshes) {
                Physics::PhysicsManager::get_instance()->register_mesh(
                        get_parent()->get_component<Component::Transform>(), mesh, "static");
            }
        }
    }

    RigidbodyDynamic::RigidbodyDynamic(Entity::Entity *entity) : IComponent(entity) {
        auto* modelComp = get_parent()->get_component<Component::Model>();
        if(modelComp) {
            auto* modelAsset = Asset::AssetManager::get_instance()->get_asset<Asset::Model>(modelComp->get_asset());
            for(auto* mesh : modelAsset->meshes) {
                Physics::PhysicsManager::get_instance()->register_mesh(
                        get_parent()->get_component<Component::Transform>(), mesh, "dynamic");
            }
        }
    }
}