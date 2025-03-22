#version 460 core

layout (location = 0) in vec3 in_vertex;
layout (location = 1) in vec3 aNormal;

uniform vec3 viewLightPos; 
uniform mat4 view;
uniform mat4 projection;

struct SSBO
{
	mat4 model;
	vec4 colour;
};

layout(binding = 1, std430) readonly buffer SSBOBuffer {
	SSBO ssbo[];
};

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;
out vec4 objectColor;
out vec3 jfw_in_vertex;
out mat4 jfw_model;
out mat4 jfw_view;
out mat4 jfw_projection;

/*
we now define the uniform in the vertex shader and pass the 'view space' viewLightPos to the fragment shader. viewlightPos is currently in world space
*/

int SSBOIndex()
{
	return max(gl_InstanceID, gl_DrawID);
}
void main()
{
	int ndx = SSBOIndex();
	mat4 model = ssbo[ndx].model;
	jfw_in_vertex = in_vertex;
	jfw_model = model;
	jfw_view = view;
	jfw_projection = projection;
	objectColor = ssbo[ndx].colour;
    gl_Position = projection * view * model * vec4(in_vertex, 1.0);
    FragPos = vec3(view * model * vec4(in_vertex, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    LightPos = vec3(view * vec4(viewLightPos, 1.0)); // Transform world-space light position to view-space light position
}

/*
lay out(location = 0) in vec3 in_vertex;
uniform mat4 pvm;

void main()
{
	gl_Position = pvm * vec4(in_vertex, 1.0);
}
*/
