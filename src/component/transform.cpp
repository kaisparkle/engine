#include <glm/gtx/quaternion.hpp>
#include <optick.h>
#include <component/transform.h>

namespace Component {
    Transform::Transform(Entity::Entity *entity) : IComponent(entity) {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        rotation = glm::identity<glm::quat>();
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    glm::mat4 Transform::get_matrix() {
        OPTICK_EVENT();
        glm::mat4 matrix = glm::mat4{1.0f};

        // get TRS matrices
        glm::mat4 transMat = glm::translate(matrix, position);
        glm::mat4 rotMat = glm::toMat4(rotation);
        glm::mat4 scaleMat = glm::scale(matrix, scale);

        return transMat * rotMat * scaleMat;
    }
}