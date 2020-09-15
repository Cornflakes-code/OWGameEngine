#version 330 core

in vec2 uv;
uniform sampler2D tex;
uniform vec4 textcolor;

void main(void) {
  gl_FragColor = vec4(1, 1, 1, texture(tex, uv).r) * textcolor;
  //gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * textcolor;
}
