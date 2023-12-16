#include <optick.h>
#include <components/transform.h>

namespace Component {
    Transform::Transform(Entity::Entity *entity) : IComponent(entity) {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    glm::mat4 Transform::get_matrix() {
        OPTICK_EVENT();
        glm::mat4 matrix = glm::mat4{1.0f};

        // translate
        matrix = glm::translate(matrix, glm::vec3(position.x, position.y, position.z));

        // rotate by each XYZ value
        matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        // scale
        matrix = glm::scale(matrix, glm::vec3(scale.x, scale.y, scale.z));

        return matrix;
    }
}