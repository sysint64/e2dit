#vertex shader

uniform mat4 MVP;

void main() {
	gl_Position = MVP*gl_Vertex;
	gl_TexCoord [0] = gl_MultiTexCoord0;
}

#fragment shader

uniform sampler2D Texture;
uniform sampler2D aTex;
uniform vec2 Offset;
uniform vec2 Size;
uniform vec2 aOffset;
uniform vec2 aSize;
uniform float Alpha;

void main() {
	/*vec2 texc = vec2(gl_TexCoord[0].x*Size[0]+Offset[0], gl_TexCoord[0].y*Size[1]+Offset[1]);
	vec4 tex  = texture2D (Texture, texc);
	//
	vec2 texc2     = vec2(gl_TexCoord[0].x*AlphaSize[0]+AlphaOffset[0], gl_TexCoord[0].y*AlphaSize[1]+AlphaOffset[1]);
	vec4 alphaTex  = texture2D (AlphaTex, texc2);
	//
	gl_FragColor = (vec3(tex), alphaTex.r);*/
	vec2 texc = vec2(gl_TexCoord[0].x*Size[0]+Offset[0], gl_TexCoord[0].y*Size[1]+Offset[1]);
	vec4 tex  = texture2D (Texture, texc);
	//
	vec2 atexc     = vec2(gl_TexCoord[0].x*aSize[0]+aOffset[0], gl_TexCoord[0].y*aSize[1]+aOffset[1]);
	vec4 alphaTex  = texture2D (aTex, atexc);
	//
	gl_FragColor = vec4(vec3(tex), tex.a*alphaTex.r*Alpha);
}
