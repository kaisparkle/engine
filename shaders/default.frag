#version 450 core

layout (location = 0) in vec3 fNormal;

layout (location = 0) out vec4 outColor;

void main() {
    outColor = vec4(fNormal.x, fNormal.y, fNormal.z, 1.0f);
}