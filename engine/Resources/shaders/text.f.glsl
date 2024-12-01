#version 330 core

in vec2 uv;
uniform sampler2D textureImageId;
uniform vec4 textcolor;

void main(void) {
	float textLevel = texture(textureImageId, uv).r;
	gl_FragColor = vec4(1, 1, 1, textLevel) * textcolor;
	
	
	// https://stackoverflow.com/questions/42364244/stacking-partly-transparent-font-textures-is-broken-by-changing-render-order
	if (textLevel < 0.5)
		gl_FragDepth = 1;
	else
		gl_FragDepth = gl_FragCoord.z;
		
  //gl_FragColor = vec4(1,1,1, 1) * gl_FragColor;
  //gl_FragColor = vec4(1, 1, 1, texture2D(textureImageId, texpos).a) * textcolor;
}

