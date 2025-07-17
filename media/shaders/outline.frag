#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
	vec3 col = (texture2D(texture_diffuse1, TexCoords)).rgb;
	
	float CameraFacingPercentage = dot(vec3(0, 0, 1), Normal);

	FragColor = vec4(col * CameraFacingPercentage + (vec3(0, 1, 0) * (1 - CameraFacingPercentage)), 1);
}