#version 330 core

// Interpolated values from the vertex shaders
// E:\Apps\OpenGL\ogl\tutorial18_billboards_and_particles
// http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
in vec2 UV;
in vec4 particlecolor;

// Ouput data
out vec4 colorOut;

uniform sampler2D myTextureSampler;

void main(){
	vec2 center = vec2(0.5, 0.5);
    float R = 0.2;
    float dist = distance(UV, center);
    if (dist > R) {
       discard;
    }	// Output color = color of the texture at the specified UV
//	color = texture( myTextureSampler, UV ) * particlecolor;
	colorOut = particlecolor;

}