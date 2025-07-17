#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in float aShadow;

out vec2 TexCoords;
out float Shadow;

uniform mat4 MVP;

void main()
{
    TexCoords = aTexCoords;
    Shadow = aShadow;
    gl_Position = MVP * vec4(aPos, 1.0f); 
}