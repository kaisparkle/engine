#include <glm/gtx/transform.hpp>
#include <components/camera.h>

namespace Component {
    void Camera::update_projection(float fovDeg, float aspectRatio, float nearPlane, float farPlane) {
        fov = fovDeg;
        aspect = aspectRatio;
        near = nearPlane;
        far = farPlane;
        projection = glm::perspective(glm::radians(fov), aspect, near, far);
    }

    glm::mat4 Camera::get_matrix() {
        return projection;
    }
}