#vertex shader
#version 330 core

layout (location = 0) in vec3 in_Position;

uniform mat4 MVP;

void main() {
	gl_Position = MVP*vec4(in_Position, 1.0);
}

#fragment shader
#version 330 core

precision highp float;
out vec4 fragColor;

uniform vec4 Color;

void main() {
	fragColor = Color;
}
