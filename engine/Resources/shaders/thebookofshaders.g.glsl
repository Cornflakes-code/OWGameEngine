#version 330 core
uniform vec2 u_resolution;
layout(points) in;
//layout(line_strip, max_vertices = 200) out;
layout(triangle_strip, max_vertices = 200) out;

#define M_PI 3.141592653589793238462643383279
#define M2_PI 6.28318530718
#define M_PI_4 0.78539816339
#define M_PI_8 0.39269908169

void doCircle(vec4 pos, float aspect, float scale)
{
	//generate vertices at positions on the circumference from 0 to 2*pi
	float delta = M_PI_8/2.0f;
    for(float i = 0; i < 6.29 ; i += delta)
	{
	    //circle parametric equation
        gl_Position = pos;
		EmitVertex();      
        gl_Position = pos + vec4(scale * cos(i) * aspect, scale * sin(i), 0.0, 0.0);
		EmitVertex();      
        gl_Position = pos + vec4(scale * cos(i+delta/2) * aspect, scale * sin(i+delta/2), 0.0, 0.0);
		EmitVertex();      
	}
}

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
	doSquare(gl_in[0].gl_Position, 1.0, 1.0, u_resolution.x / u_resolution.y);
	//doCircle(gl_in[0].gl_Position, u_resolution.y/u_resolution.x, 1.0);
}
