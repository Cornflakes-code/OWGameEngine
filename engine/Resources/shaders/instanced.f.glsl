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


// YUV to RGB matrix
mat3 yuv2rgb = mat3(1.0, 0.0, 1.13983,
                    1.0, -0.39465, -0.58060,
                    1.0, 2.03211, 0.0);

// RGB to YUV matrix
mat3 rgb2yuv = mat3(0.2126, 0.7152, 0.0722,
                    -0.09991, -0.33609, 0.43600,
                    0.615, -0.5586, -0.05639);

float random (vec2 st) 
{
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

void main()
{
	vec2 center = vec2(0.0, 0.0);
    float R = 4.0;
    float dist = distance(UV, center);
    if (dist > R) {
       discard;
    }
    mat3 yuv2rgb_hack = mat3(1.0, 0.0, 1.0 - particlecolor.x,
                    1.0, -0.39465, -0.58060,
                    1.0, 2.03211, 0.0);

    vec2 st = UV * 2;
    // UV values goes from -1 to 1
    // So we need to remap st (0.0 to 1.0)
    //st -= 0.5;  // becomes -0.5 to 0.5
    //st *= 2.0;  // becomes -1.0 to 1.0

    // we pass st as the y & z values of
    // a three dimensional vector to be
    // properly multiply by a 3x3 matrix
    vec3 color = rgb2yuv * yuv2rgb_hack * vec3(0.5, st.x, st.y);
    colorOut = vec4(color.x, color.y, abs(sin(u_time)), 1.0);

    float rnd = random( st );
    vec2 v2 = gl_FragCoord.xy/u_resolution.xy;
    colorOut = vec4(rnd * (v2.x + v2.y), rnd, rnd, 1.0);

}    
    //colorOut = vec4(UV.x * 4 * abs(cos(u_time)), UV.y * 4 * abs(cos(u_time)), abs(sin(u_time)), 1.0);
    //colorOut = vec4(particlecolor.x * 4 * abs(cos(u_time)), particlecolor.y * 4 * abs(sin(u_time)), abs(sin(u_time)), 1.0);
    //colorOut = particlecolor * dist * dist * 4;//particlecolor;
    //colorOut = vec4(particlecolor * dist / 0.15);//particlecolor;
    // Output color = color of the texture at the specified UV
//	color = texture( myTextureSampler, UV ) * particlecolor;