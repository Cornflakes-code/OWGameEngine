#version 330 core
// https://thebookofshaders.com/
uniform vec4 pointColour;

out vec4 myOutputColor;

void main() {
	vec2 st = gl_FragCoord.xy/800;
	myOutputColor= vec4(st.x,st.y,0.0,0.9);
}

