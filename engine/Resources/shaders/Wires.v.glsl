layout(location = 0) in vec3 aPos;
uniform mat4 pvm;
out vec3 wireColor;

void main()
{
	wireColor = vec3(1.0f, 0.5f, 0.31f);
	gl_Position = pvm * vec4(aPos, 1.0);
}