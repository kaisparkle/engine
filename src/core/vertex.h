#pragma once

#include <glm/glm.hpp>

namespace Core {
    struct Vertex {
        glm::vec3 position __attribute__((aligned (16)));
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };
}