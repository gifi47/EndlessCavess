#version 330 core

layout(location = 0) in vec2 in_vertexPos;
layout(location = 1) in vec3 in_color;

out vec3 aColor;

uniform mat4 MVP;

void main(){
	gl_Position = MVP * vec4(in_vertexPos.x, in_vertexPos.y, 0, 1);
	aColor = in_color;
}