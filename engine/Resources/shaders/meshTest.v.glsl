#version 460 core
layout (location = 0) in vec3 in_vertex;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

struct SSBO
{
	vec4 model;
	vec4 pos;
};

layout(binding = 1, std430) readonly buffer SSBOBuffer {
	SSBO ssbo[];
};

int SSBOIndex()
{
	return max(gl_InstanceID, gl_DrawID);
}

uniform mat4 pv;

out vec2 TexCoords;

void main()
{
	int ndx = SSBOIndex();
    TexCoords = ssbo[ndx].pos.xy;
    gl_Position = pv * ssbo[ndx].model * vec4(in_vertex, 1.0);
}