#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;

out vec3 Normal;
out vec2 TexCoords;

uniform mat4 MVP;

void main()
{
	vec3 n_pos = vertexPosition_modelspace;
	
	gl_Position = MVP * vec4(n_pos, 1);

	Normal = vertexNormal;
	TexCoords = vertexTexCoord;
}