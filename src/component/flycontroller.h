#pragma once

#include <glm/glm.hpp>
#include <component/controller.h>

namespace Component {
    class FlyController : public IController {
    public:
        FlyController(Entity::Entity* entity);

        // take in mouse input
        void process_mouse(float dx, float dy);
        // take in keyboard input
        void process_keyboard(double delta, const uint8_t *keystate);
    private:
        float pitch;
        float yaw;
        float sensitivity;
        float velocity;
    };
}