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
        float sensitivity;
        float velocity;
        float cameraPitch;
        glm::vec3 up;
        glm::vec3 front;
        glm::vec3 right;
        glm::vec3 worldUp;

        // update vectors
        void update_vectors();
    };
}