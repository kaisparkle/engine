#include <optick.h>
#include <glm/gtx/transform.hpp>
#include <SDL.h>
#include <player/playermanager.h>
#include <components/camera.h>

namespace Component {
    Camera::Camera(Entity::Entity *entity) : IComponent(entity) {
        update_projection(90.0f, (float)1600/(float)900, 0.1f, 10000.0f);
        pitch = 0.0f;
        yaw = -90.0f;
        velocity = 100.0f;
        sensitivity = 0.1f;
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        worldUp = up;
        update_vectors();
    }

    void Camera::update_projection(float fovDeg, float aspectRatio, float nearPlane, float farPlane) {
        fov = fovDeg;
        aspect = aspectRatio;
        near = nearPlane;
        far = farPlane;
        projection = glm::perspective(glm::radians(fov), aspect, near, far);
    }

    glm::mat4 Camera::get_view_matrix() {
        OPTICK_EVENT();
        glm::vec3 position = Player::PlayerManager::get_instance()->get_player_transform()->position;
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 Camera::get_projection_matrix() {
        OPTICK_EVENT();
        return projection;
    }

    void Camera::process_mouse(float dx, float dy) {
        OPTICK_EVENT();
        dx *= sensitivity;
        dy *= sensitivity;
        yaw += dx;
        pitch += -dy;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        update_vectors();
    }

    void Camera::process_keyboard(double delta, const uint8_t *keystate) {
        OPTICK_EVENT();
        auto vel = (float) (velocity * (delta / 1000));
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        if (keystate[SDL_SCANCODE_W]) position += front * vel;
        if (keystate[SDL_SCANCODE_A]) position -= right * vel;
        if (keystate[SDL_SCANCODE_S]) position -= front * vel;
        if (keystate[SDL_SCANCODE_D]) position += right * vel;
        if (keystate[SDL_SCANCODE_Q]) position -= up * vel;
        if (keystate[SDL_SCANCODE_E]) position += up * vel;
        Player::PlayerManager::get_instance()->get_player_transform()->position += position;
    }

    void Camera::update_vectors() {
        OPTICK_EVENT();
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
}