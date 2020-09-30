#version 330 core
// https://thebookofshaders.com/

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;
out vec4 myOutputColor;

void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = vec3(0.0);

    vec2 pos = vec2(0.5)-st;

    float r = length(pos)*2.0;
    float a = atan(pos.y,pos.x);

    float f = cos(a*3.);
     f = abs(cos(a*3.));
     f = abs(cos(a*2.5))*.5+.3;
     f = abs(cos(a*12.)*sin(u_time*0.3))*.2+.1;
     f = smoothstep(-.5,1., cos(a*10.))*0.2+0.5;

    color = vec3( 1.-smoothstep(f,f+0.160,r) );
   	color.x -=0.4* u_time;
    color.y -=0.* u_time;

    myOutputColor = vec4(color, 1.0);
//    myOutputColor = vec4(0.2, 0.3, 0.4, 0.0);//vec4(color, 1.0);
//    myOutputColor = vec4(0.2,0.4,0.5,1.0);
}
