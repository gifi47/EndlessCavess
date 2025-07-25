#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    vec4 color = texture(texture_diffuse1, TexCoords);
    color.a = 0.5f;
    FragColor = color;
}