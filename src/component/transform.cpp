#include <optick.h>
#include <component/transform.h>

namespace Component {
    Transform::Transform(Entity::Entity *entity) : IComponent(entity) {
        position[0] = 0.0f;
        position[1] = 0.0f;
        position[2] = 0.0f;
        rotation[0] = 0.0f;
        rotation[1] = 0.0f;
        rotation[2] = 0.0f;
        scale[0] = 1.0f;
        scale[1] = 1.0f;
        scale[2] = 1.0f;
    }

    glm::mat4 Transform::get_matrix() {
        OPTICK_EVENT();
        glm::mat4 matrix = glm::mat4{1.0f};

        // translate
        matrix = glm::translate(matrix, glm::vec3(position[0], position[1], position[2]));

        // rotate by each XYZ value
        matrix = glm::rotate(matrix, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

        // scale
        matrix = glm::scale(matrix, glm::vec3(scale[0], scale[1], scale[2]));

        return matrix;
    }
}