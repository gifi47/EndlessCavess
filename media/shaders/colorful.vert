#version 330 core

layout(location = 0) in vec2 in_vertexPos;
layout(location = 0) in vec2 in_texCoord;

out vec2 uv;

uniform mat4 MVP;

void main(){
	uv = in_texCoord;
	gl_Position = MVP * vec4(in_vertexPos.x, in_vertexPos.y, 0, 1);
}