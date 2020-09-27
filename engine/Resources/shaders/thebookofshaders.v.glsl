#version 330 core

layout (location = 0) in vec4 pointpos;

uniform mat4 pvm = mat4(1.0);

void main() {
    gl_Position = pvm * pointpos;
}
