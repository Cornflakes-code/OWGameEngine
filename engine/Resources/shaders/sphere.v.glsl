#version 460 core

// https://blog.42yeah.is/rendering/opengl/2023/12/18/perfect-ellipsoid.html
//sphere.v.glsl
layout (location = 0) in vec3 vertex;

struct SSBO
{
	vec4 model;
};

layout(binding = 1, std430) readonly buffer SSBOBuffer {
	SSBO ssbo[];
};

int SSBOIndex()
{
	return max(gl_InstanceID, gl_DrawID);
}

uniform mat4 view;
uniform mat4 perspective;

out vec3 worldPos;

int SSBOIndex()
{
	return max(gl_InstanceID, gl_DrawID);
}

void main() {
	int ndx = SSBOIndex();
	mat4 model = ssbo[ndx].model;
    vec4 wPos = model * vec4(vertex, 1.0);
    worldPos = vec3(wPos);
    gl_Position = perspective * view * wPos;
}
