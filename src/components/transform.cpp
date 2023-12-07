#include <components/transform.h>

namespace Component {
    Transform::Transform(Entity::Entity *entity) : IComponent(entity) {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    glm::mat4 Transform::get_matrix() {
        // convert to matrices
        glm::mat4 transMat = glm::translate(glm::mat4(), position);
        glm::mat4 rotMat = glm::mat4_cast(rotation);
        glm::mat4 scaleMat = glm::scale(glm::mat4(), scale);

        // multiply to get TRS matrix
        return transMat * rotMat * scaleMat;
    }
}