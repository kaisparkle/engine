#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;

layout (location = 0) out vec2 fUV;

layout (location = 0) uniform mat4 matrix_mvp;

void main() {
    fUV = vUV;
    gl_Position = matrix_mvp * vec4(vPos, 1.0f);
}