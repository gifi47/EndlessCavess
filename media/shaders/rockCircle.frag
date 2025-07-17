#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 color;
uniform sampler2D texture_diffuse1;

void main()
{
    vec2 cord = TexCoords - vec2(0.5, 0.5);
    if (sqrt(cord.x * cord.x + cord.y * cord.y) > 0.5) discard;
    //FragColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(color, 0.0);
}