#version 460 core

// textStaticBillboard.v.glsl
layout(location = 0) in vec4 coord;

layout(binding = 1, std430) readonly buffer ssbo1 {
	vec4 SSB_pos[];
	//vec4 SSB_colour;
};

// Output data ; will be interpolated for each fragment.
out vec2 uv;
out vec3 jfw_CameraRight_worldspace;
out vec3 jfw_CameraUp_worldspace;
out mat4 jfw_VP;
out vec4 jfw_BillboardPos;
out vec2 jfw_BillboardSize;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in particleCenter_wordspace; the orientation depends on the camera)
//uniform vec3 BillboardPos; // Position of the center of the billboard
uniform vec2 BillboardSize; // Size of the billboard, in world units 

void main()
{
	vec3 particleCenter_wordspace = vec3(SSB_pos[gl_DrawID].xyz);
		jfw_CameraRight_worldspace = CameraRight_worldspace;
	jfw_CameraUp_worldspace = CameraUp_worldspace;
	jfw_VP = VP;
	jfw_BillboardPos = SSB_pos[gl_DrawID];
	jfw_BillboardSize = BillboardSize;
	//vec3 particleCenter_wordspace = BillboardPos;

	// Output position of the vertex
	gl_Position = VP * vec4(particleCenter_wordspace, 1.0f);
	gl_Position /= gl_Position.w;
	gl_Position.xy += coord.xy * BillboardSize; 

	// Or, if BillboardSize is in percentage of the screen size (1,1 for fullscreen) :
	//gl_Position = VP * vec4(particleCenter_wordspace, 1.0f); // Get the screen-space position of the particle's center
	
	// Or, if BillboardSize is in pixels : 
	// Same thing, just use (ScreenSizeInPixels / BillboardSizeInPixels) instead of BillboardSizeInScreenPercentage.

	uv = coord.zw;
}
