#version 330 core
layout (location = 0) in vec3 coord;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;

/*
we now define the uniform in the vertex shader and pass the 'view space' viewLightPos to the fragment shader. viewlightPos is currently in world space
*/
uniform vec3 viewLightPos; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(coord, 1.0);
    FragPos = vec3(view * model * vec4(coord, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    LightPos = vec3(view * vec4(viewLightPos, 1.0)); // Transform world-space light position to view-space light position
}

/*
lay out(location = 0) in vec3 coord;
uniform mat4 pvm;

void main()
{
	gl_Position = pvm * vec4(coord, 1.0);
}
*/
