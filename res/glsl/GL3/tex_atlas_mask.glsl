#vertex shader
#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 2) in vec2 in_TexCoord;

uniform mat4 MVP;
out vec2 TexCoord;

void main() {

	gl_Position = MVP*vec4(in_Position, 1.0);
	TexCoord    = in_TexCoord.xy;

}

#fragment shader
#version 330 core

precision highp float;
out vec4 fragColor;
in  vec2 TexCoord;

uniform sampler2D Texture;
uniform sampler2D Mask;
uniform vec2      Offset;
uniform vec2      Size;
uniform vec2      MaskOffset;
uniform vec2      MaskSize;
uniform float     Alpha;

void main() {

	vec2 texc     = vec2      (TexCoord.x*Size[0]+Offset[0], TexCoord.y*Size[1]+Offset[1]);
	vec4 tex      = texture2D (Texture, texc);

	vec2 atexc    = vec2      (TexCoord.x*MaskSize[0]+MaskOffset[0], TexCoord.y*MaskSize[1]+MaskOffset[1]);
	vec4 maskTex  = texture2D (Mask, atexc);

	fragColor = vec4(vec3(tex), tex.a*maskTex.r*Alpha);

}
