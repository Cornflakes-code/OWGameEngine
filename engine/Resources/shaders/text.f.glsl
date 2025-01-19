#version 330 core
// text.f.glsl
in vec2 uv;
uniform sampler2D textureImageId;
uniform vec4 textcolor;

void main(void) {
	float opacity = texture(textureImageId, uv).r;
	//vec4 t = texture(textureImageId, uv);
	gl_FragColor = vec4(1, 1, 1, opacity) * textcolor;
	//gl_FragColor = mix(textcolor, t, t.a);
	
	// https://stackoverflow.com/questions/42364244/stacking-partly-transparent-font-textures-is-broken-by-changing-render-order
	if (true)
	{
	if (opacity < 0.5)
		gl_FragDepth = 1;
	else
		gl_FragDepth = gl_FragCoord.z;
	}
		
  //gl_FragColor = vec4(1,1,1, 1) * gl_FragColor;
  //gl_FragColor = vec4(1, 1, 1, texture2D(textureImageId, texpos).a) * textcolor;
}

