#version 330 core

// Input vertex data, different for all executions of this shader.
// E:\Apps\OpenGL\ogl\tutorial18_billboards_and_particles
// file:///E:/Apps/OpenGL/ogl/tutorial18_billboards_and_particles/Billboard.vertexshader
// http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/

layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // Position of the center of the particle
layout(location = 2) in vec4 color;

// Output data will be interpolated for each fragment.
out vec2 particleCenter;
out vec4 particlecolor;
out vec4 frag_Position;
// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform vec2 u_resolution;

void main()
{
	float particleSize = 1.0;
	vec3 particleCenter_worldspace = xyzs.xyz;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_worldspace
		+ CameraRight_worldspace * squareVertices.x * particleSize
		+ CameraUp_worldspace * squareVertices.y * particleSize;

	// Output position of the vertex
	frag_Position = VP * vec4(vertexPosition_worldspace, 1.0f);

    //vec2 ndcPos = gl_Position.xy / gl_Position.w;
    //UV = u_resolution * (ndcPos * 0.5 + 0.5);

	vec4 temp = VP * vec4(xyzs.xyz, 1.0f);
	vec2 tempPos = temp.xy / temp.w;
	particleCenter = u_resolution * (tempPos * 0.5 + 0.5);
	particlecolor = color;
}