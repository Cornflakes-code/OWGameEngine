#version 460 core

// textStaticBillboard.v.glsl
layout(location = 0) in vec4 in_vertex;

struct SSBO
{
	vec4 pos;
	vec4 colour;
	vec2 bbSize;
};

layout(binding = 1, std430) readonly buffer SSBOBuffer {
	SSBO ssbo[];
};

int SSBOIndex()
{
	return max(gl_InstanceID, gl_DrawID);
}

// Output data ; will be interpolated for each fragment.
out int jfw_ndx;
out vec2 uv;
out vec3 jfw_CameraRight_worldspace;
out vec3 jfw_CameraUp_worldspace;
out mat4 jfw_pv;
out vec3 jfw_BillboardPos;
out vec2 jfw_BillboardSize;
out vec4 ssb_colour;
out vec4 jfw_pos;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 pv; // Model-View-Projection matrix, but without the Model (the position is in particleCenter_wordspace; the orientation depends on the camera)


void main()
{
	int ndx = SSBOIndex();
	ssb_colour = ssbo[ndx].colour;
	//if (all(equal(ssb_colour, vec4(0,0,0,0))))
	//	ssb_colour = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 BillboardPos = vec3(ssbo[ndx].pos.xyz); // Position of the center of the billboard
	vec2 BillboardSize = vec2(ssbo[ndx].bbSize.xy); // Size of the billboard, in world units 
	jfw_BillboardPos = BillboardPos;
	jfw_CameraRight_worldspace = CameraRight_worldspace;
	jfw_CameraUp_worldspace = CameraUp_worldspace;
	jfw_pv = pv;
	jfw_ndx = ndx;
	jfw_pos = ssbo[ndx].pos;
	jfw_BillboardSize = BillboardSize;

	gl_Position = pv * vec4(BillboardPos, 1.0f);
	gl_Position /= gl_Position.w;
	gl_Position.xy += in_vertex.xy * BillboardSize; 

	// Or, if BillboardSize is in percentage of the screen size (1,1 for fullscreen) :
	//gl_Position = pv * vec4(particleCenter_wordspace, 1.0f); // Get the screen-space position of the particle's center
	
	// Or, if BillboardSize is in pixels : 
	// Same thing, just use (ScreenSizeInPixels / BillboardSizeInPixels) instead of BillboardSizeInScreenPercentage.

	uv = in_vertex.zw;
}
