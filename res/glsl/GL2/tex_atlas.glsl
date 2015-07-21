#vertex shader

uniform mat4 MVP;

void main() {
	gl_Position = MVP*gl_Vertex;
	gl_TexCoord [0] = gl_MultiTexCoord0;
}

#fragment shader

uniform sampler2D Texture;
uniform vec2 Offset;
uniform vec2 Size;
uniform float Alpha;

void main() {
	vec2 texc = vec2(gl_TexCoord[0].x*Size[0]+Offset[0], gl_TexCoord[0].y*Size[1]+Offset[1]);
	vec4 tex  = texture2D (Texture, texc);
	gl_FragColor = vec4(tex.rgb, tex.a*Alpha);
}
