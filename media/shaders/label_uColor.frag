#version 330 core

in vec2 texCoord;

out vec4 color;

uniform vec3 uColor;

uniform sampler2D tex;

void main(){
	vec4 texColor = texture(tex, texCoord);
	if (texColor.a < 0.1) discard;
	color = vec4(uColor, 1.0);
}