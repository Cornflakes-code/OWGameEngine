#version 330 core
uniform vec2 u_resolution;
layout(points) in;
//layout(line_strip, max_vertices = 200) out;
layout(triangle_strip, max_vertices = 200) out;

#define M_PI 3.141592653589793238462643383279
#define M2_PI 6.28318530718
#define M_PI_4 0.78539816339
#define M_PI_8 0.39269908169

void doSquare(vec4 pos, float x, float y, float aspect)
{
	float offset = 0.01;
	gl_Position = vec4(-x + offset, y - offset, pos.z, pos.w);
	EmitVertex();      
	gl_Position = vec4(x - offset, -y + offset, pos.z, pos.w);
	EmitVertex();      
	gl_Position = vec4(-x + offset, -y + offset, pos.z, pos.w);
	EmitVertex();      

	gl_Position = vec4(-x + offset, y - offset, pos.z, pos.w);
	EmitVertex();      
	gl_Position = vec4(x - offset, y- offset, pos.z, pos.w);
	EmitVertex();      
	gl_Position = vec4(x - offset, -y + offset, pos.z, pos.w);
	EmitVertex();      
}

void main()
{
	vec4 v4 = vec4(0.0, 0.0, 0.0, 1.0);
	
	doSquare(v4, 1.0, 1.0, u_resolution.x / u_resolution.y);
//	doSquare(gl_in[0].gl_Position, 1.0, 1.0, u_resolution.x / u_resolution.y);
}
