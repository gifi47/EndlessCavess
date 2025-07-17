#version 330 core

layout(location = 0) in vec2 in_vertexPos;

uniform float uTime;

uniform mat4 MVP;

mat4 rotationMatrixY(float angle) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    
    return mat4(
        cosAngle, 0.0, sinAngle, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sinAngle, 0.0, cosAngle, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

// Initialize rotation matrix around the X axis
mat4 rotationMatrixX(float angle) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, cosAngle, -sinAngle, 0.0,
        0.0, sinAngle, cosAngle, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

// Initialize rotation matrix around the Z axis
mat4 rotationMatrixZ(float angle) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    
    return mat4(
        cosAngle, -sinAngle, 0.0, 0.0,
        sinAngle, cosAngle, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

void main(){
    vec4 pos = MVP * vec4(in_vertexPos.x, in_vertexPos.y, 0, 1);
    vec4 new_pos = pos;
    new_pos.x = pos.x + fract(pos.x * 22 + pos.y * 12 + uTime) * 0.03 * abs(cos(pos.x * pos.y * 12 - uTime * 0.7));
    new_pos.y = pos.y + fract(pos.y * 22 + pos.x * 12 + uTime) * 0.03 * abs(cos(pos.x * pos.y * 12 - uTime * 0.5));
    //new_pos.x = pos.x + sin(pos.x * 22 + pos.y * 12 + uTime) * 0.03 * abs(cos(pos.x * pos.y * 12 - uTime * 0.7));
    //new_pos.y = pos.y + sin(pos.y * 22 + pos.x * 12 + uTime) * 0.03 * abs(cos(pos.x * pos.y * 12 - uTime * 0.5));
    gl_Position = new_pos;
    //gl_Position = MVP * vec4(in_vertexPos.x, in_vertexPos.y, 0, 1);
}