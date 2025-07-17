#version 330 core

in vec2 uv;

out vec4 color;

uniform float time;
uniform vec2 end;

const vec2 start = vec2(0.5, 0);
//const vec2 end = vec2(0.5, 1);
const vec2 end1 = vec2(0, 0.5);

// Description : Array - and textureless GLSL 2D simplex noise .
// Author : Ian McEwan , Ashima Arts . Version : 20110822
// Copyright (C) 2011 Ashima Arts . All rights reserved .
// Distributed under the MIT License . See LICENSE file .
// https :// github . com / ashima /webgl - noise
vec3 mod289(vec3 x) { return x - floor (x * (1.0 / 289.0) ) * 289.0; }
vec2 mod289(vec2 x) { return x - floor (x * (1.0 / 289.0) ) * 289.0; }
vec3 permute(vec3 x) { return mod289 ((( x *34.0) +1.0) *x); }

float snoise(vec2 v) {
	const vec4 C = vec4 (0.211324865405187 , // (3.0 - sqrt (3.0) ) /6.0
	0.366025403784439 , // 0.5*( sqrt (3.0) -1.0)
	-0.577350269189626 , // -1.0 + 2.0 * C.x
	0.024390243902439) ; // 1.0 / 41.0
	// First corner
	vec2 i = floor (v + dot(v, C.yy ) );
	vec2 x0 = v - i + dot (i, C.xx );
	// Other corners
	vec2 i1 = ( x0.x > x0.y) ? vec2 (1.0, 0.0) : vec2 (0.0, 1.0) ;
	vec4 x12 = x0.xyxy + C.xxzz;
	x12.xy -= i1;
	// Permutations
	i = mod289 (i) ; // Avoid truncation effects in permutation
	vec3 p = permute ( permute ( i.y + vec3 (0.0 , i1.y , 1.0 ))
	+ i.x + vec3 (0.0 , i1.x , 1.0 ));
	vec3 m = max (0.5 - vec3 (dot (x0 , x0 ) , dot(x12.xy, x12.xy ),
	dot(x12.zw, x12.zw)) , 0.0);
	m = m * m; m = m * m;
	// Gradients
	vec3 x = 2.0 * fract (p * C.www ) - 1.0;
	vec3 h = abs (x) - 0.5;
	vec3 a0 = x - floor (x + 0.5);
	// Normalise gradients implicitly by scaling m
	m *= 1.79284291400159 - 0.85373472095314 * ( a0 * a0 + h *h );
	// Compute final noise value at P
	vec3 g;
	g.x = a0.x * x0.x + h.x * x0.y;
	g.yz = a0.yz * x12.xz + h.yz * x12.yw;
	return 130.0 * dot (m, g) ;
}

void main(){
	/*float fbm=snoise(vec3(5.0*uv, 0.0))
 	+ 0.5*snoise(vec3(10.0*uv, 2.0))
 	+ 0.25*snoise(vec3(20.0*uv, 4.0))
 	+ 0.125*snoise(vec3(40.0*uv, 6.0))
 	+ 0.0625*snoise(vec3(80.0*uv, 8.0));*/


 	float cf = time;//sin(time);
 	//float fbm=snoise(uv+cf);
 	float fbm=snoise(5.0*uv+end*cf)
 	+ 0.5*snoise(10.0*uv+end*cf)
 	+ 0.25*snoise(20.0*uv+end*cf)
 	+ 0.125*snoise(40.0*uv+end*cf)
 	+ 0.0625*snoise(80.0*uv+end*cf);
	color = vec4(0.4*vec3(fbm) + 0.5, 1.0);
	//color = vec4(0.4*fbm + 0.5, 1.0, 1.0, 1.0);
	//color = vec4(fract(time), fract(time + 0.5), fract(time * 0.33), 1.0);

	  /*  float sin_time = abs(sin(time));

	    float sin_time2 = sin(time * 0.1) * 0.5;
	    float cos_time = abs(cos(time * 0.1));
	vec2 st = uv;
	//st.x += (fract(time) - st.x) * cos_time;
	//st.y += (fract(time) - st.y) * cos_time;
	st.x = st.x * 0.9 + 0.1 * abs(cos(st.x + time));
	st.y = st.y * 0.9 + 0.1 * abs(sin(st.y + time));

//vec2 end1 = vec2(0, 0.5) + vec2(cos_time, sin_time2);
vec2 dir = normalize(end - start);
vec2 dir1 = normalize(end1 - start);
float m = length(end - st);
float m1 = length(end1 - st);
float d = m / (m + m1);
float d1 = m1 / (m + m1);

vec2 dm = (-dir * d);
vec2 dm1 = (-dir1 * d1);
vec3 color3 = vec3(0.);
vec2 new_uv = st + (dm + dm1) * time;
//vec2 new_uv = st + (-dir * d * time * 0.1);
//cf = time;

float fbm=snoise(5.0*new_uv)
 	+ 0.5*snoise(10.0*new_uv)
 	+ 0.25*snoise(20.0*new_uv)
 	+ 0.125*snoise(40.0*new_uv)
 	+ 0.0625*snoise(80.0*new_uv);
 	
 	vec4 color_noise = vec4(0.4*fbm + 0.5, fract(new_uv.x), fract(new_uv.y), 1.0);//vec4(0.4*vec3(fbm) + 0.5, 1.0);
 //color3 = vec3(fract(new_uv.x), fract(new_uv.y), 0);
    //color = vec4(color3,1.0);

    vec4 color_sus1 = vec4(1.0);

    vec3 v_cf = vec3(sin_time, 1 - sin_time, st.x + fract(time) * st.y);


    color_sus1.r = color_noise.r * v_cf.r + color_noise.g * v_cf.g + color_noise.b * v_cf.b;
    color_sus1.g = color_noise.r * v_cf.b + color_noise.g * v_cf.g + color_noise.b * v_cf.r;
    color_sus1.b = color_noise.r * v_cf.b + color_noise.g * v_cf.r + color_noise.b * v_cf.g;

    color = color_sus1;*/


}