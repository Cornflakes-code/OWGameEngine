#version 330 core

in vec2 uv;
uniform sampler2D textureImageId;
uniform vec4 textcolor;

void main(void) {
  gl_FragColor = vec4(1, 1, 1, texture(textureImageId, uv).r) * textcolor;
  //gl_FragColor = vec4(1, 1, 1, texture2D(textureImageId, texpos).a) * textcolor;
}
