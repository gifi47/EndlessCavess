#version 330 core

layout(location = 0) in vec2 in_vertexPos;
layout(location = 1) in vec2 in_texCoord;
layout(location = 2) in float in_colorR;

out vec2 texCoord;
out float colorR;

uniform mat4 MVP;

void main(){
	gl_Position = MVP * vec4(in_vertexPos.x, in_vertexPos.y, 0, 1);
	colorR = in_colorR;
	texCoord = in_texCoord;
}