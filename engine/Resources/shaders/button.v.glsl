#version 460 core

uniform mat4 pv;

layout (location = 0) in vec3 in_vertex; // matches gl::EnableVertexAttribArray(0);
layout (location = 1) in vec2 in_uv; // matches gl::EnableVertexAttribArray(1);

struct SSBO
{
	mat4 model;
};

uniform mat4 VP;

layout(binding = 1, std430) readonly buffer SSBOBuffer {
	SSBO ssbo[];
};

int SSBOIndex()
{
	return max(gl_InstanceID, gl_DrawID);
}

out vec2 uv;

void main()
{
	mat4 mvp = pv * ssbo[SSBOIndex].model;
    gl_Position = mvp * ssbo[SSBOIndex()].model * vec4(in_vertex, 1.0f);
    uv = in_uv;
}
