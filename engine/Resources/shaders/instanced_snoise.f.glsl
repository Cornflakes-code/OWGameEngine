#version 330 core

// Interpolated values from the vertex shaders
// E:\Apps\OpenGL\ogl\tutorial18_billboards_and_particles
// http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
in vec2 UV;
in vec4 particlecolor;

// Ouput data
out vec4 colorOut;

//uniform sampler2D myTextureSampler;
uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;

// https://thebookofshaders.com/edit.php#11/2d-snoise-clear.frag

vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permute(vec3 x) { return mod289(((x*34.0)+1.0)*x); }

float snoise(vec2 v) {

    // Precompute values for skewed triangular grid
    const vec4 C = vec4(0.211324865405187,
                        // (3.0-sqrt(3.0))/6.0
                        0.366025403784439,
                        // 0.5*(sqrt(3.0)-1.0)
                        -0.577350269189626,
                        // -1.0 + 2.0 * C.x
                        0.024390243902439);
                        // 1.0 / 41.0

    // First corner (x0)
    vec2 i  = floor(v + dot(v, C.yy));
    vec2 x0 = v - i + dot(i, C.xx);

    // Other two corners (x1, x2)
    vec2 i1 = vec2(0.0);
    i1 = (x0.x > x0.y)? vec2(1.0, 0.0):vec2(0.0, 1.0);
    vec2 x1 = x0.xy + C.xx - i1;
    vec2 x2 = x0.xy + C.zz;

    // Do some permutations to avoid
    // truncation effects in permutation
    i = mod289(i);
    vec3 p = permute(
            permute( i.y + vec3(0.0, i1.y, 1.0))
                + i.x + vec3(0.0, i1.x, 1.0 ));

    vec3 m = max(0.5 - vec3(
                        dot(x0,x0),
                        dot(x1,x1),
                        dot(x2,x2)
                        ), 0.0);

    m = m*m ;
    m = m*m ;

    // Gradients:
    //  41 pts uniformly over a line, mapped onto a diamond
    //  The ring size 17*17 = 289 is close to a multiple
    //      of 41 (41*7 = 287)

    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;

    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt(a0*a0 + h*h);
    m *= 1.79284291400159 - 0.85373472095314 * (a0*a0+h*h);

    // Compute final noise value at P
    vec3 g = vec3(0.0);
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * vec2(x1.x,x2.x) + h.yz * vec2(x1.y,x2.y);
    return 130.0 * dot(m, g);
}

void main()
{
	vec2 center = vec2(0.0, 0.0);
    float R = 0.15;
    float dist = distance(UV, center);
    if (dist > R) {
       discard;
    }
    vec2 st = UV * 50;
    float val = gl_FragCoord.x/u_resolution.x;
    vec3 color = vec3(snoise(st)*.5 + .5);
    colorOut = vec4(color.x*abs(cos(u_time)), color.y, color.z*abs(sin(u_time)), 1.0);
    colorOut = vec4(val);

}    
    //colorOut = vec4(UV.x * 4 * abs(cos(u_time)), UV.y * 4 * abs(cos(u_time)), abs(sin(u_time)), 1.0);
    //colorOut = vec4(particlecolor.x * 4 * abs(cos(u_time)), particlecolor.y * 4 * abs(sin(u_time)), abs(sin(u_time)), 1.0);
    //colorOut = particlecolor * dist * dist * 4;//particlecolor;
    //colorOut = vec4(particlecolor * dist / 0.15);//particlecolor;
    // Output color = color of the texture at the specified UV
//	color = texture( myTextureSampler, UV ) * particlecolor;