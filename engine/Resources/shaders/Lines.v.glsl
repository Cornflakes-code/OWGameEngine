#version 460 core

layout (location = 0) in vec3 vPos;

struct SSBO
{
	mat4 model;
	vec4 colour;
};

layout(binding = 1, std430) readonly buffer SSBOBuffer {
	SSBO ssbo[];
};

uniform mat4 pv;
out vec4 uColour;
int SSBOIndex()
{
	return max(gl_InstanceID, gl_DrawID);
}

void main() {
	int ndx = SSBOIndex();
    gl_Position = pv * ssbo[ndx].model * vec4(vPos, 1.0);
	uColour = ssbo[ndx].colour;
}
