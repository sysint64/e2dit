#vertex shader
#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 2) in vec2 in_TexCoord;

uniform mat4 MVP;
out vec2 TexCoord;

void main() {
	gl_Position = MVP*vec4(in_Position, 1.0);
	TexCoord = in_TexCoord.xy;
}

#fragment shader
#version 330 core

precision highp float;
out vec4 fragColor;
in  vec2 TexCoord;

uniform sampler2D myTextureSampler;

void main() {
	vec4 tex = texture2D (myTextureSampler, TexCoord);
	fragColor = tex;
}
