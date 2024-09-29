#version 430 core

// https://blog.42yeah.is/rendering/opengl/2023/12/18/perfect-ellipsoid.html

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
uniform mat4 pvm;

out vec3 worldPos;

void main() {
    vec4 wPos = model * vec4(aPos, 1.0);
    worldPos = vec3(wPos);
    gl_Position = perspective * view * wPos;
}
