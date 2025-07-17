#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;

uniform mat4 MVP;

void main()
{
	vec3 n_pos = vertexPosition_modelspace + vertexNormal * 0.1f;
	
	//n_pos.z -= 1.0f;
	vec4 pos = (MVP * vec4(n_pos, 1));
	gl_Position = pos;
}