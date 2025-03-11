#version 460 core

// textDynamicBillboard.v.glsl
layout(location = 0) in vec4 coord;

layout(binding = 2, std430) readonly buffer ssbo1 {
	vec4 BillboardPos_SSB[];
};

// Output data ; will be interpolated for each fragment.
out vec2 uv;
out int dummy1;
out vec4 jfw_ssb;
out vec3 jfw_bbp;
out int jfw_draw_id;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform vec3 BillboardPos; // Position of the center of the billboard
uniform vec2 BillboardSize; // Size of the billboard, in world units (probably meters)

void main()
{
	//vec3 particleCenter_wordspace = BillboardPos;
	vec4 temp = BillboardPos_SSB[gl_DrawID];
	vec3 particleCenter_wordspace = vec3(temp.xyz);
	jfw_ssb = BillboardPos_SSB[gl_DrawID];
	jfw_bbp = BillboardPos;
	jfw_draw_id = gl_DrawID;
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
