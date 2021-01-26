#version 330 core

// Interpolated values from the vertex shaders
// E:\Apps\OpenGL\ogl\tutorial18_billboards_and_particles
// http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
in vec4 particlecolor;
in vec2 particleCenter;
// Ouput data
out vec4 colorOut;

//uniform sampler2D myTextureSampler;
uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;
uniform float cutoffRadius;


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
	float R = cutoffRadius * 2;
	vec2 uv = particleCenter.xy - gl_FragCoord.xy;
    if (length(uv) > R/1.5) {
		//blend = 0;
       discard;
    }
    vec2 st = uv * 0.5 / R;

    mat3 yuv2rgb_hack = mat3(1.0, 0.0, 1.0 - particlecolor.x,
                    1.0, -0.39465, -0.58060,
                    1.0, 2.03211, 0.0);


    // we pass st as the y & z values of
    // a three dimensional vector to be
    // properly multiply by a 3x3 matrix
    vec3 color = rgb2yuv * yuv2rgb_hack * vec3(0.5, st.x, st.y);
    colorOut = vec4(color.x, color.y, abs(sin(u_time)), 1.0);

    float rnd = random( st );
    colorOut = vec4(rnd * (st.x + st.y), rnd, rnd, 1.0);

}    
    //colorOut = vec4(UV.x * 4 * abs(cos(u_time)), UV.y * 4 * abs(cos(u_time)), abs(sin(u_time)), 1.0);
    //colorOut = vec4(particlecolor.x * 4 * abs(cos(u_time)), particlecolor.y * 4 * abs(sin(u_time)), abs(sin(u_time)), 1.0);
    //colorOut = particlecolor * dist * dist * 4;//particlecolor;
    //colorOut = vec4(particlecolor * dist / 0.15);//particlecolor;
    // Output color = color of the texture at the specified UV
//	color = texture( myTextureSampler, UV ) * particlecolor;