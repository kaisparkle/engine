#pragma once

#include <cstdint>
#include <component/component.h>

namespace Component {
    class IController : public IComponent {
    public:
        IController(Entity::Entity* entity) : IComponent(entity) {}

        // take in mouse input
        virtual void process_mouse(float dx, float dy) = 0;
        // take in keyboard input
        virtual void process_keyboard(double delta, const uint8_t *keystate) = 0;
    };
}