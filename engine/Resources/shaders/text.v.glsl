#version 460 core

layout (location = 0) in vec4 in_vertex;

struct SSBO
{
	mat4 model;
};

layout(binding = 1, std430) readonly buffer SSBOBuffer {
	SSBO ssbo[];
};

int SSBOIndex()
{
	return max(gl_InstanceID, gl_DrawID);
}

uniform mat4 pv;

uniform float scale;
uniform float zed;

out vec2 uv;

void main(void) {
  gl_Position = pv * ssbo[SSBOIndex()].model * vec4(in_vertex.xy, zed, scale);
  uv = in_vertex.zw;
}
