#version 330 core

// How to achieve and control a simple distance glow effect based on several Shadertoy examples
// Things to try:
//  * Make the radius and intensity pulse in time or to input
//	* Time varying colour
//  * Animate several points and add the glow values for each to create metaballs
//  * Generate more complex shapes
// https://www.shadertoy.com/view/3s3GDn

in vec2 UV;
in vec4 particlecolor;
in vec2 particleCenter;
out vec4 colourOut;
uniform float cutoffRadius;
uniform vec2 u_resolution;

void main()
{
	float R = cutoffRadius*4.5;
	float blend = 0.7;
	vec2 uv = particleCenter.xy - gl_FragCoord.xy;
    if (length(uv) > R) {
		//blend = 0;
       //discard;
    }
	// uv already centered around the origen, now make it between -0.5 and 0.5
    uv *= 0.5 / R;

    vec2 pos = uv;
    
    //**********         Glow        **********
    
    // Equation 1/x gives a hyperbola which is a nice shape to use for drawing glow as 
    // it is intense near 0 followed by a rapid fall off and an eventual slow fade
//    float dist = 1./(length(pos)*length(pos));
    float dist = 1.0/(length(pos));
    
    //**********        Radius       **********

    // Dampen the glow to control the radius
    dist *= 0.7;
    //**********       Intensity     **********

    // Raising the result to a power allows us to change the glow fade behaviour
    // See https://www.desmos.com/calculator/eecd6kmwy9 for an illustration
    // (Move the slider of m to see different fade rates)
    dist = pow(dist, 1.8);

    //Knowing the distance from a fragment to the source of the glow, the above can be written compactly as: 
    //	float getGlow(float dist, float radius, float intensity){
    //		return pow(radius/dist, intensity);
	//	}
    //The returned value can then be multiplied with a colour to get the final result

    // Get colour
//    vec3 col = dist * vec3(1.0, 0.5, 0.25);
    vec3 col = dist * particlecolor.xyz;

    // See comment by P_Malin
    col = 1.0 - exp( -col );

    // Output to screen
	//blend *= 1 - (dist);
    colourOut = vec4(col, blend);
}