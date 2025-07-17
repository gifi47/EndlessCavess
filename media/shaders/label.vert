#version 330 core

layout(location = 0) in vec2 in_vertexPos;
layout(location = 1) in vec2 in_texCoord;

out vec2 texCoord;

uniform mat4 MVP;

void main(){
	texCoord = in_texCoord;
	gl_Position =  MVP * vec4(in_vertexPos.x, in_vertexPos.y, 0, 1);
}