#include <SDL.h>
#include <glm/glm.hpp>
#include <optick.h>
#include <component/transform.h>
#include <component/camera.h>
#include <component/flycontroller.h>

namespace Component {
    FlyController::FlyController(Entity::Entity *entity) : IController(entity) {
        // TODO: cvars
        velocity = 10.0f;
        sensitivity = 0.1f;
        pitch = 0.0f;
        yaw = 0.0f;
    }

    void FlyController::process_mouse(float dx, float dy) {
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

    void FlyController::process_keyboard(double delta, const uint8_t *keystate) {
        glm::vec3 front = get_parent()->get_component<Component::Camera>()->get_front();
        glm::vec3 right = get_parent()->get_component<Component::Camera>()->get_right();
        glm::vec3 up = get_parent()->get_component<Component::Camera>()->get_up();

        auto vel = (float) (velocity * (delta / 1000));
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        if (keystate[SDL_SCANCODE_W]) position += front * vel;
        if (keystate[SDL_SCANCODE_A]) position -= right * vel;
        if (keystate[SDL_SCANCODE_S]) position -= front * vel;
        if (keystate[SDL_SCANCODE_D]) position += right * vel;
        if (keystate[SDL_SCANCODE_Q]) position -= up * vel;
        if (keystate[SDL_SCANCODE_E]) position += up * vel;
        get_parent()->get_component<Component::Transform>()->position += position;
    }
}