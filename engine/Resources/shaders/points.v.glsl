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

uniform mat4 VP;

void main() {
    gl_Position = VP * ssbo[SSBOIndex()].model * in_vertex;
}
