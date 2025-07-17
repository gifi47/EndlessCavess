#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 color;

uniform sampler2D texture_diffuse1;

void main()
{
    float sm_step = smoothstep(0.1, 0.7, (abs(TexCoords.x - 0.5) * 2) * (abs(TexCoords.y - 0.5) * 2));
    vec4 rez_color = texture(texture_diffuse1, TexCoords) * (1 - sm_step) + vec4(color, 1.0) * sm_step;
    FragColor = rez_color;
}