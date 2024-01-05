#include <component/rigidbodycontroller.h>
#include <component/transform.h>
#include <component/camera.h>
#include <physics/physicsmanager.h>
#include <physics/layers.h>
#include <physics/collider/capsule.h>
#include <SDL.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Component {
    RigidbodyController::RigidbodyController(Entity::Entity *entity) : IController(entity) {
        float height = 5.0f;
        float radius = 1.5f;
        pitch = 0.0f;
        yaw = 0.0f;
        characterSpeed = 10.0f;
        characterJumpSpeed = 10.0f;
        sensitivity = 0.1f;

        // create capsule collider
        auto* body = get_parent()->get_component<Component::Rigidbody>();
        auto* collider = new Physics::Collider::CapsuleCollider(body, height, radius);
        body->add_collider(collider);

        // create shape ourselves instead of initializing
        JPH::ShapeRefC capsule = collider->get_shape_settings()->Create().Get();

        // set up Jolt character controller
        characterSettings = new JPH::CharacterSettings();
        characterSettings->mMaxSlopeAngle = JPH::DegreesToRadians(45.0f);
        characterSettings->mLayer = Physics::Layers::MOVING;
        characterSettings->mShape = capsule;
        characterSettings->mFriction = 0.5f;
        characterSettings->mSupportingVolume = JPH::Plane(JPH::Vec3::sAxisY(), -radius);
        auto* transform = get_parent()->get_component<Component::Transform>();
        JPH::Vec3 position = {transform->position.x, transform->position.y, transform->position.z};
        JPH::Quat rotation = {transform->rotation.x, transform->rotation.y, transform->rotation.z, transform->rotation.w};

        character = Physics::PhysicsManager::get_instance()->create_character(characterSettings, collider, position, rotation);
    }

    void RigidbodyController::process_mouse(float dx, float dy) {
        OPTICK_EVENT();
        // scale by sensitivity
        dx *= sensitivity;
        dy *= sensitivity;

        pitch += -dy;
        if(pitch > 89.9f) pitch = 89.9f;
        if(pitch < -89.9f) pitch = -89.9f;
        yaw += dx;

        get_parent()->get_component<Component::Camera>()->update_vectors(pitch, yaw);
    }

    void RigidbodyController::process_keyboard(double delta, const uint8_t *keystate) {
        // use forward not front vector to calculate direction, we want to ignore camera pitch
        glm::vec3 forward = get_parent()->get_component<Component::Camera>()->get_forward();
        glm::vec3 right = get_parent()->get_component<Component::Camera>()->get_right();
        glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
        if (keystate[SDL_SCANCODE_W]) direction += forward;
        if (keystate[SDL_SCANCODE_A]) direction -= right;
        if (keystate[SDL_SCANCODE_S]) direction -= forward;
        if (keystate[SDL_SCANCODE_D]) direction += right;

        // convert to Jolt vec3
        JPH::Vec3 jDirection = {direction.x, direction.y, direction.z};

        // check if we're on a slope
        JPH::Character::EGroundState groundState = character->GetGroundState();
        if(groundState == JPH::Character::EGroundState::OnSteepGround || groundState == JPH::Character::EGroundState::NotSupported) {
            JPH::Vec3 normal = character->GetGroundNormal();
            normal.SetY(0.0f);
            float dot = normal.Dot(jDirection);
            if(dot < 0.0f) jDirection -= (dot * normal) / normal.LengthSq();
        }

        // calculate our velocity
        JPH::Vec3 currentVelocity = character->GetLinearVelocity();
        JPH::Vec3 desiredVelocity = characterSpeed * jDirection;
        desiredVelocity.SetY(currentVelocity.GetY());
        JPH::Vec3 newVelocity = 0.75f * currentVelocity + 0.25 * desiredVelocity;

        // jump if pressed and we're on solid ground
        if(keystate[SDL_SCANCODE_SPACE] && groundState == JPH::Character::EGroundState::OnGround) {
            newVelocity += JPH::Vec3(0.0f, characterJumpSpeed, 0.0f);
        }

        // set our new velocity
        character->SetLinearVelocity(newVelocity);
    }
}