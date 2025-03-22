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

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 pv; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform vec2 u_resolution;

float circle(vec2 coord, vec2 center, float radius, float blur) {
	vec2 dist = coord - center;
	return 1.0 - smoothstep(pow(radius - blur, 2.0), pow(radius + blur, 2.0), dot(dist, dist));
}

float star(vec2 coord, vec2 center, float radius, float points, float blur, float rot) {
	vec2 p = center - coord;

	//Polar coordinates
	float d = length(p) * 2.0;
	float a = atan(p.y, p.x);

	float f = (cos(a * points + rot) + 1.0) / 4.0 + 0.5;
	return 1.0 - smoothstep(f * radius - blur / 2.0, f * radius + blur / 2.0, d);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{

	//float a = circle(fragCoord.xy, vec2(200.0, 100.), 80.0, 10.0);
	//float b = circle(fragCoord.xy, vec2(300.0, 100.), 80.0, 10.0);

	//fragColor = vec4(1.0, 0.0, 1.0, 1.0) * a;
	//fragColor += vec4(0.0, 1.0, 0.0, 1.0) * b;

	float radius = 200.0;
	float blur = 100.0;

	vec4 c1 = vec4(0.1, 0.6, 1.0, 1.0);
	vec4 c2 = vec4(0.0, 0.8, 1.0, 1.0);
	vec4 c3 = vec4(0.2, 1.0, 0.5, 1.0);
	vec4 c4 = vec4(0.3, 1.0, 1.0, 1.0);

	vec2 pos = iResolution.xy / 2.0 - fragCoord.xy;

	float size = min(iResolution.x, iResolution.y) * 0.85;

	float star1 = star(fragCoord.xy, iResolution.xy / 2.0, size * 0.7, 12.0, 300.0, iTime);
	float star2 = star(fragCoord.xy, iResolution.xy / 2.0, size, 18.0, 300.0, -iTime * 1.2);
	float star3 = star(fragCoord.xy, iResolution.xy / 2.0, size, 24.0, 300.0, iTime * 1.1);
	float star4 = star(fragCoord.xy, iResolution.xy / 2.0, size * 0.8, 9.0, 200.0, -iTime);
	fragColor = c1 * star1 + c2 * star2 + c3 * star3 + c4 * star4;


}

void main()
{
	float particleSize = 1.0;
	vec3 particleCenter_worldspace = xyzs.xyz;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_worldspace
		+ CameraRight_worldspace * squareVertices.x * particleSize
		+ CameraUp_worldspace * squareVertices.y * particleSize;

	// Output position of the vertex
	gl_Position = pv * vec4(vertexPosition_worldspace, 1.0f);

    //vec2 ndcPos = gl_Position.xy / gl_Position.w;
    //UV = u_resolution * (ndcPos * 0.5 + 0.5);

	vec4 temp = pv * vec4(xyzs.xyz, 1.0f);
	vec2 tempPos = temp.xy / temp.w;
	particleCenter = u_resolution * (tempPos * 0.5 + 0.5);
	particlecolor = color;
}