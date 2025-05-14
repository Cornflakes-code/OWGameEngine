#version 460 core

layout (location = 0) in vec3 vPos;

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

uniform mat4 pv;

out float jfw_ndx;
out vec3 jfw_vpos;
out vec4 uColour;
out vec4 texpos;
out mat4 jfw_pv;
out mat4 jfw_model;
out mat4 jfw_projection;
out mat4 jfw_view;
int SSBOIndex()
{
	return max(gl_InstanceID, gl_DrawID);
}

void main() {
	int ndx = SSBOIndex();
	jfw_ndx = ndx;
	jfw_projection = projection;
	jfw_view = view;
	jfw_pv = pv;
	jfw_vpos = vPos;
	texpos = vec4(vPos, 1.0);
	jfw_model = ssbo[ndx].model;

    gl_Position = pv * ssbo[ndx].model * vec4(vPos, 1.0);
//    gl_Position = projection * view * ssbo[ndx].model * vec4(vPos, 1.0);
//	uColour = vec4(0.67,0.67,0.67, 0);// ssbo[ndx].colour;
	int col = 3;
	int row = 1;
	uColour = ssbo[ndx].colour;
	if (ssbo[ndx].model[col][row] == 200)
		uColour = vec4(0,1,0,1);
}
