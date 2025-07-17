#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 MVP;

void main()
{
    TexCoords = aTexCoords;
    vec4 pos = MVP * vec4(aPos, 1.0f);
    vec4 new_pos = pos;
    new_pos.x = pos.x + sin(pos.x * 22 + pos.y * 12) * 0.03 * abs(cos(pos.x * pos.y * 12));
    new_pos.y = pos.y + sin(pos.y * 22 + pos.x * 12) * 0.03 * abs(cos(pos.x * pos.y * 12));
    gl_Position = new_pos;
}