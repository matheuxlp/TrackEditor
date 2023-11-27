#version 440 core

layout (location = 0) in vec3 position;

uniform mat4 projection; // The orthographic projection matrix

void main() {
    gl_Position = projection * vec4(position, 1.0f);
}