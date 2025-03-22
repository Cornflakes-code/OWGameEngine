#version 330 core

in vec4 texpos;

in vec4 uColour;

void main() {
    gl_FragColor = uColour;
}
