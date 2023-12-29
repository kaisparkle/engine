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
        up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    void FlyController::process_mouse(float dx, float dy) {
        OPTICK_EVENT();
        // scale by sensitivity
        dx *= sensitivity;
        dy *= sensitivity;

        // offset the camera angles and update vectors
        get_parent()->get_component<Component::Camera>()->offset_angles(dy, dx);
        get_parent()->get_component<Component::Camera>()->update_vectors();
    }

    void FlyController::process_keyboard(double delta, const uint8_t *keystate) {
        OPTICK_EVENT();
        glm::quat orientation = get_parent()->get_component<Component::Camera>()->get_orientation();
        glm::quat frontQuat = orientation * glm::quat(0.0f, 0.0f, 0.0f, -1.0f) * glm::conjugate(orientation);
        glm::vec3 front = glm::vec3(frontQuat.x, frontQuat.y, frontQuat.z);
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

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