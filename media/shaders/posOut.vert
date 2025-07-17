#version 330 core

layout(location = 0) in vec2 in_vertexPos;

out vec3 pos;

uniform mat4 MVP;

void main(){
	vec4 worldPos = MVP * vec4(in_vertexPos.x, in_vertexPos.y, 0, 1);
	pos = worldPos.xyz;
	gl_Position = worldPos;
}