#version 460 core

// textDynamicBillboard.v.glsl
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
out vec4 jfw_pos;
out vec4 ssb_colour;
out vec3 jfw_CameraRight_worldspace;
out vec3 jfw_CameraUp_worldspace;
out vec2 jfw_BillboardSize;
out mat4 jfw_VP;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform vec2 BillboardSize; // Size of the billboard, in world units (probably meters)

void main()
{
	jfw_CameraRight_worldspace = CameraRight_worldspace;
	jfw_CameraUp_worldspace = CameraUp_worldspace;
	jfw_BillboardSize = BillboardSize;
	jfw_VP = VP;

	ssb_colour = ps[gl_DrawID].colour;
	vec3 particleCenter_wordspace = vec3(ps[gl_DrawID].pos.xyz);
	jfw_pos = ps[gl_DrawID].pos;
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CameraRight_worldspace * coord.x * BillboardSize.x
		+ CameraUp_worldspace * coord.y * BillboardSize.y;


	// Output position of the vertex
	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f);



	// Or, if BillboardSize is in percentage of the screen size (1,1 for fullscreen) :
	//vertexPosition_worldspace = particleCenter_wordspace;
	//gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f); // Get the screen-space position of the particle's center
	//gl_Position /= gl_Position.w; // Here we have to do the perspective division ourselves.
	//gl_Position.xy += coord.xy * vec2(0.2, 0.05); // Move the vertex in directly screen space. No need for CameraUp/Right_worlspace here.
	
	// Or, if BillboardSize is in pixels : 
	// Same thing, just use (ScreenSizeInPixels / BillboardSizeInPixels) instead of BillboardSizeInScreenPercentage.

	uv = coord.zw;
}
