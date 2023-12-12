// How to achieve and control a simple distance glow effect based on several Shadertoy examples
// Things to try:
//  * Make the radius and intensity pulse in time or to input
//	* Time varying colour
//  * Animate several points and add the glow values for each to create metaballs
//  * Generate more complex shapes
// https://www.shadertoy.com/view/4lG3zV

#version 330 core
in vec2 UV;
in vec4 particlecolor;
in vec2 particleCenter;
out vec4 colourOut;
uniform float cutoffRadius;
uniform vec2 u_resolution;
uniform float u_time;
uniform float iTimeDelta;

float circle( vec2 coord, vec2 center, float radius, float blur ) 
{
    vec2 dist = coord - center;
	return 1.0 - smoothstep(pow(radius - blur, 2.0), pow(radius + blur, 2.0), dot(dist,dist));
}

float star( vec2 coord, vec2 center, float radius, float points, float blur, float rot) {
    vec2 p = center - coord;
    
    //Polar coordinates
    float d = length(p) * 2.0;
    float a = atan(p.y, p.x);
    
    float f = (cos(a * points + rot) + 1.0) / 4.0  + 0.5;
    return 1.0 - smoothstep(f * radius - blur / 2.0, f * radius + blur / 2.0, d);
}

void mainImage( vec4 fragColor, vec2 pos )
{
	//float a = circle(fragCoord.xy, vec2(200.0, 100.), 80.0, 10.0);
    //float b = circle(fragCoord.xy, vec2(300.0, 100.), 80.0, 10.0);
    
	//fragColor = vec4(1.0, 0.0, 1.0, 1.0) * a;
    //fragColor += vec4(0.0, 1.0, 0.0, 1.0) * b;
    
    //float radius = 200.0;
    //float blur = 100.0;
    
    //float size = min( particleCenter.x, particleCenter.y ) * 0.85;
    float size = 100.0;
    
	float star1 = star( pos.xy, particleCenter, size * 0.7, 12.0, 300.0, u_time );
    float star2 = star( pos.xy, particleCenter, size, 18.0, 300.0, -u_time * 1.2 );
    float star3 = star( pos.xy, particleCenter, size, 24.0, 300.0, u_time * 1.1 );
    float star4 = star( pos.xy, particleCenter, size * 0.8, 9.0, 200.0, -u_time );

    vec4 c1 = vec4( 0.1, 0.6, 1.0, 1.0 );
    vec4 c2 = vec4( 0.0, 0.8, 1.0, 1.0 );
    vec4 c3 = vec4( 0.2, 1.0, 0.5, 1.0 );
    vec4 c4 = vec4( 0.3, 1.0, 1.0, 1.0 );
    colourOut = c1 * star1 + c2 * star2 + c3 * star3 + c4 * star4; 
      
}
void main()
{
	mainImage(particlecolor, gl_FragCoord.xy);
}