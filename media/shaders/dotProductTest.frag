#version 330 core

in vec3 pos; 

uniform vec3 p1;
uniform vec3 p2;
uniform vec3 p3;

out vec4 colorOut;

void main(){
	vec2 t1 = vec2(pos.x - p1.x, pos.y - p1.y);
	vec2 t2 = vec2(p2.x - p1.x, p2.y - p1.y);
	vec2 t3 = vec2(pos.x - p2.x, pos.y - p2.y);

	vec2 t4 = vec2(p3.x - p1.x, p3.y - p1.y);
	vec2 t5 = vec2(pos.x - p3.x, pos.y - p3.y);
	//vec2 t2 = vec2(pos.x - p2.x, pos.y - p2.y);
	float dotProduct = dot(t1, t2); //t1.x * t2.x + t1.y * t2.y;
	vec3 color;
	if (dotProduct < 0 || (dot(t3, t2) > 0)
		|| (dot(t1, t4) < 0) || (dot(t5, t4) > 0)){
		color = vec3(1.0, 0.1, 0.1);
	} else {
		color = vec3(0.0, 1.0, 0.23);
	}
	/*
	else{
		//float a_cos = dotProduct / length(t2);
		vec2 n_n = t2;//= normalize(t2);
		n_n.x *= dotProduct;//a_cos;
		n_n.y *= dotProduct;//a_cos;
		vec2 proj = p1.xy + n_n.xy / pow(length(t2), 2.0);
		if (distance(pos.xy, proj) < 0.3){
			color = vec3(0.1, 1.0, 0.1);
		} else {
			color = vec3(1.0, 0.1, 0.1);
		}
	}*/
	colorOut = vec4(color, 1.0);
}