#version 330 core

layout (location = 0) in vec4 coord;

uniform mat4 pvm;
uniform float scale;
uniform float zed;

out vec2 uv;

void main(void) {
  gl_Position = pvm * vec4(coord.xy, zed, scale);
  uv = coord.zw;
}
