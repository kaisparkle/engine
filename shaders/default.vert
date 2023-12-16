#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

layout (location = 0) out vec3 fNormal;

layout (location = 0) uniform mat4 matrix_mvp;

void main() {
    fNormal = vNormal;
    gl_Position = matrix_mvp * vec4(vPos, 1.0f);
}