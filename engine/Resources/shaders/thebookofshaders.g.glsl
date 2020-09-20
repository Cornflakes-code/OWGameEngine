#version 330 core
layout(points) in;
//layout(line_strip, max_vertices = 200) out;
layout(triangle_strip, max_vertices = 200) out;

#define M_PI 3.141592653589793238462643383279
#define M2_PI 6.28318530718
#define M_PI_4 0.78539816339
#define M_PI_8 0.39269908169
#define X_SCALE 1.0
void doCircle(vec4 pos, float scale)
{
	//generate vertices at positions on the circumference from 0 to 2*pi
	float delta = M_PI_8/2.0f;
    for(float i = 0; i < 6.29 ; i += delta)
	{
	    //circle parametric equation
        gl_Position = pos;
		EmitVertex();      
        gl_Position = pos + vec4(scale * cos(i) * X_SCALE, scale * sin(i), 0.0, 0.0);
		EmitVertex();      
        gl_Position = pos + vec4(scale * cos(i+delta/2) * X_SCALE, scale * sin(i+delta/2), 0.0, 0.0);
		EmitVertex();      
	}
}

void main()
{
	doCircle(gl_in[0].gl_Position, 50);
}
