#version 460 core
layout (location = 0) in vec3 in_vertex;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

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

out vec2 TexCoords;
out int jfw_ndx;
out mat4 jfw_model;
out mat4 jfw_pv;

void main()
{
	int ndx = SSBOIndex();
	jfw_ndx = ndx;
	jfw_model = ssbo[ndx].model;
	jfw_pv = pv;
    TexCoords = aTexCoords;
    gl_Position = pv * ssbo[ndx].model * vec4(in_vertex, 1.0);
}