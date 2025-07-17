#version 330 core

in vec2 texCoord;
in float colorR;

out vec4 color;

void main(){
	vec2 cord = texCoord - vec2(0.5, 0.5);
    if (sqrt(cord.x * cord.x + cord.y * cord.y) > 0.5) discard;
    //FragColor = texture(texture_diffuse1, TexCoords);
    color = vec4(colorR, 0.0, 0.0, 1.0);
}