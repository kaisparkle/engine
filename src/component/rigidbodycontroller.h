#pragma once
#include <physics/internal.h>
#include <Jolt/Physics/Character/Character.h>
#include <component/controller.h>

namespace Component {
    class RigidbodyController : public IController {
    public:
        RigidbodyController(Entity::Entity* entity);

        // take in mouse input
        void process_mouse(float dx, float dy);
        // take in keyboard input
        void process_keyboard(double delta, const uint8_t *keystate);
    private:
        JPH::CharacterSettings* characterSettings;
        JPH::Character* character;

        float pitch;
        float yaw;
        float characterSpeed;
        float characterJumpSpeed;
        float sensitivity;
    };
}