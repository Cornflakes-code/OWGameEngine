// How to achieve and control a simple distance glow effect based on several Shadertoy examples
// Things to try:
//  * Make the radius and intensity pulse in time or to input
//	* Time varying colour
//  * Animate several points and add the glow values for each to create metaballs
//  * Generate more complex shapes
// https://www.shadertoy.com/view/3s3GDn

#version 330 core
layout(location = 0) in vec3 fragCoord;

uniform vec3 iResolution;
uniform vec4 colour;

out vec4 fragColor;

void main()
{
    
    //***********    Basic setup    **********
    
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord.xy/iResolution.xy;
    // The ratio of the width and height of the screen
    float widthHeightRatio = iResolution.x/iResolution.y;
    vec2 pos = uv;

    // Adjust y by ratio for uniform transforms
    pos.y /= widthHeightRatio;
    
    //**********         Glow        **********
    
    // Equation 1/x gives a hyperbola which is a nice shape to use for drawing glow as 
    // it is intense near 0 followed by a rapid fall off and an eventual slow fade
    float dist = 1./length(pos);
    
    //**********        Radius       **********
    
    // Dampen the glow to control the radius
    dist *= 0.1;
    
    //**********       Intensity     **********
    
    // Raising the result to a power allows us to change the glow fade behaviour
    // See https://www.desmos.com/calculator/eecd6kmwy9 for an illustration
    // (Move the slider of m to see different fade rates)
    dist = pow(dist, 0.8);
    
    //Knowing the distance from a fragment to the source of the glow, the above can be written compactly as: 
    //	float getGlow(float dist, float radius, float intensity){
    //		return pow(radius/dist, intensity);
	//	}
    //The returned value can then be multiplied with a colour to get the final result
       
    // Get colour
    vec3 col = dist * colour.xyz;
	
    // See comment by P_Malin
    col = 1.0 - exp( -col );
    
    // Output to screen
    fragColor = vec4(col, 1.0);
}