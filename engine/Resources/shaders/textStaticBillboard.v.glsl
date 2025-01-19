#version 330 core

// textStaticBillboard.v.glsl
layout(location = 0) in vec4 coord;

// Output data ; will be interpolated for each fragment.
out vec2 uv;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform vec3 BillboardPos; // Position of the center of the billboard
uniform vec2 BillboardSize; // Size of the billboard, in world units (probably meters)

void main()
{
	// Output position of the vertex
	gl_Position = VP * vec4(BillboardPos, 1.0f);
	gl_Position /= gl_Position.w;
	gl_Position.xy += coord.xy * BillboardSize; 

	// Or, if BillboardSize is in percentage of the screen size (1,1 for fullscreen) :
	//gl_Position = VP * vec4(BillboardPos, 1.0f); // Get the screen-space position of the particle's center
	
	// Or, if BillboardSize is in pixels : 
	// Same thing, just use (ScreenSizeInPixels / BillboardSizeInPixels) instead of BillboardSizeInScreenPercentage.

	uv = coord.zw;
}
