#version 460 core

// textDynamicBillboard.v.glsl
layout(location = 0) in vec4 coord;

layout(binding = 1, std430) readonly buffer ssbo1 {
	vec4 BillboardPos_SSB[];
};

// Output data ; will be interpolated for each fragment.
out vec2 uv;
out vec4 jfw_pos;
out int jfw_draw_id;
out int jfw_gl_VertexID;
out int jfw_gl_InstanceID;
//out vec4 jfw_ssb;
//out int jfw_draw_id;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform vec2 BillboardSize; // Size of the billboard, in world units (probably meters)

void main()
{
	vec3 particleCenter_wordspace = vec3(BillboardPos_SSB[gl_DrawID].xyz);
	jfw_pos = BillboardPos_SSB[gl_DrawID];
	jfw_draw_id = gl_DrawID;
	jfw_gl_VertexID = gl_VertexID;
	jfw_gl_InstanceID = gl_InstanceID;
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
