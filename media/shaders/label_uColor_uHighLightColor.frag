#version 330 core

in vec2 texCoord;

out vec4 color;

uniform vec3 uColor;

uniform vec4 uHighLightColor = vec4(0, 0, 0, 0);

uniform sampler2D tex;

void main(){
	vec4 texColor = texture(tex, texCoord);
	if (texColor.a < 0.1) {
		if (uHighLightColor.a < 0.1)
			discard;
		else
			color = uHighLightColor;
	}
	else if (uHighLightColor.a > 0.1){
		color = vec4(vec3(1.0, 1.0, 1.0) - uColor, 1.0);
	} else {
		color = vec4(uColor, 1.0);
	}
}