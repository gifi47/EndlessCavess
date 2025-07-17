#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in float Shadow;

uniform sampler2D texture_diffuse1;

void main()
{
    vec4 color = texture(texture_diffuse1, TexCoords);
    color.rgb = color.rgb * (1.0f - Shadow * 3.33f);
    /*if (Shadow > 0.2f){
        color.rgb = vec3(1.0f, 0.0f, 0.0f);
    }
    else if (Shadow > 0.0f){
        color.rgb = vec3(0.75f, 0.75f, 0.0f);
    } else{
        color.rgb = vec3(0.0f, 1.0f, 0.0f);
    }
    if (!(TexCoords.x > 0.3f && TexCoords.y > 0.3f && TexCoords.x < 0.7f && TexCoords.y < 0.7f))
        color.rgb = color.rgb * (1.0f - Shadow * 2.35f);//vec3(1, 1, 1) - vec3(Shadow, Shadow, Shadow) * 3.0f;*/
    FragColor = color;
}