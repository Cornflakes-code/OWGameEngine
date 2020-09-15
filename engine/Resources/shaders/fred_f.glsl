#version 330 core

in vec4 texpos;

uniform vec4 uColour;

void main() {
    gl_FragColor = vec4(0.2,0.4,0.5,1);
    //gl_FragColor = uColour;
}
