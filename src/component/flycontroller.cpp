#include <SDL.h>
#include <glm/glm.hpp>
#include <optick.h>
#include <component/transform.h>
#include <component/camera.h>
#include <component/flycontroller.h>

namespace Component {
    FlyController::FlyController(Entity::Entity *entity) : IController(entity) {
        // TODO: cvars
        velocity = 100.0f;
        sensitivity = 0.1f;
        cameraPitch = 0.0f;
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        worldUp = up;
    }

    void FlyController::process_mouse(float dx, float dy) {
        OPTICK_EVENT();
        // scale by sensitivity
        dx *= sensitivity;
        dy *= sensitivity;
        // update yaw
        get_parent()->get_component<Component::Transform>()->rotation[1] += dx;
        // update camera pitch
        cameraPitch += -dy;
        if (cameraPitch > 89.0f) cameraPitch = 89.0f;
        if (cameraPitch < -89.0f) cameraPitch = -89.0f;

        // update vectors
        update_vectors();
        get_parent()->get_component<Component::Camera>()->update_vectors(up, front);
    }

    void FlyController::process_keyboard(double delta, const uint8_t *keystate) {
        OPTICK_EVENT();
        auto vel = (float) (velocity * (delta / 1000));
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        if (keystate[SDL_SCANCODE_W]) position += front * vel;
        if (keystate[SDL_SCANCODE_A]) position -= right * vel;
        if (keystate[SDL_SCANCODE_S]) position -= front * vel;
        if (keystate[SDL_SCANCODE_D]) position += right * vel;
        if (keystate[SDL_SCANCODE_Q]) position -= up * vel;
        if (keystate[SDL_SCANCODE_E]) position += up * vel;
        get_parent()->get_component<Component::Transform>()->position[0] += position.x;
        get_parent()->get_component<Component::Transform>()->position[1] += position.y;
        get_parent()->get_component<Component::Transform>()->position[2] += position.z;
    }

    void FlyController::update_vectors() {
        OPTICK_EVENT();
        glm::vec3 newFront;
        float yaw = get_parent()->get_component<Component::Transform>()->rotation[1];
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(cameraPitch));
        newFront.y = sin(glm::radians(cameraPitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(cameraPitch));
        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
}