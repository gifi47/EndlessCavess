#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform float time;

uniform sampler2D texture_diffuse1;

void main()
{
    int cf = 0;
    float q = sin(time);
    float c = cos(time);
    if (q > 0){
        if (c > 0){
            cf = 1;
        } else {
            cf = 2;
        }
    } else {
        if (c < 0){
            cf = 3;
        }
    }

    ivec2 sus = ivec2(TexCoords.x * 16 * 10, TexCoords.y * 16 * 10);
    if (sus.x > 9) {
        sus.x -= 10;
    }
    if (sus.y > 9){
        sus.y -= 10;
    }

    sus.y += cf;
    if (sus.y > 9){
        sus.y -= 10;
        sus.y += 2;
    }

    for (int i = 0; i < 2; i++){
        if (sus.y == i+0 || sus.y == i+4 || sus.y == i+8){
            if (sus.x == i+2 || sus.x == i+3 || sus.x == i+6 || sus.x == i+7 || (i == 1 && sus.x == 0)) discard;
        }
        if (sus.y == i+2 || sus.y == i+6){
            if (sus.x == i+0 || sus.x == i+1 || sus.x == i+4 || sus.x == i+5 || sus.x == i+8 || sus.x == i+9) discard;
        }
    }

    vec4 color = texture(texture_diffuse1, TexCoords);
    color.a = 0.5f;
    FragColor = color;
}