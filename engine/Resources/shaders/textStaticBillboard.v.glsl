#version 460 core

// textStaticBillboard.v.glsl
layout(location = 0) in vec4 coord;

struct PosColour
{
	vec4 pos;
	vec4 colour;
};

layout(binding = 1, std430) readonly buffer ssbo1 {
	PosColour ps[];
};

// Output data ; will be interpolated for each fragment.
out vec2 uv;
out vec3 jfw_CameraRight_worldspace;
out vec3 jfw_CameraUp_worldspace;
out mat4 jfw_VP;
out vec3 jfw_BillboardPos;
out vec2 jfw_BillboardSize;
out vec4 ssb_colour;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in particleCenter_wordspace; the orientation depends on the camera)
//uniform vec3 BillboardPos; // Position of the center of the billboard
uniform vec2 BillboardSize; // Size of the billboard, in world units 

void main()
{
	ssb_colour = ps[gl_DrawID].colour;
	vec3 BillboardPos = vec3(ps[gl_DrawID].pos.xyz);
	jfw_BillboardPos = BillboardPos;
	jfw_CameraRight_worldspace = CameraRight_worldspace;
	jfw_CameraUp_worldspace = CameraUp_worldspace;
	jfw_VP = VP;
	jfw_BillboardSize = BillboardSize;

	gl_Position = VP * vec4(BillboardPos, 1.0f);
	gl_Position /= gl_Position.w;
	gl_Position.xy += coord.xy * BillboardSize; 

	// Or, if BillboardSize is in percentage of the screen size (1,1 for fullscreen) :
	//gl_Position = VP * vec4(particleCenter_wordspace, 1.0f); // Get the screen-space position of the particle's center
	
	// Or, if BillboardSize is in pixels : 
	// Same thing, just use (ScreenSizeInPixels / BillboardSizeInPixels) instead of BillboardSizeInScreenPercentage.

	uv = coord.zw;
}
