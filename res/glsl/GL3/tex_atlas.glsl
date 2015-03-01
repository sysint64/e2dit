#vertex shader
#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TexCoord;

uniform mat4 MVP;
out vec2 TexCoord;

void main() {
	gl_Position = MVP*vec4(in_Position, 1.0);
	TexCoord = in_TexCoord.xy;
}

#fragment shader
#version 330 core

precision lowp float;
out vec4 fragColor;
in  vec2 TexCoord;

uniform sampler2D Texture;
uniform vec2  Offset;
uniform vec2  Size;
uniform float Alpha;

void main() {
	vec2 texc = vec2(TexCoord.x*Size[0]+Offset[0], TexCoord.y*Size[1]+Offset[1]);
	vec4 tex  = texture2D (Texture, texc);
	fragColor = vec4(tex.rgb, tex.a*Alpha);
}

