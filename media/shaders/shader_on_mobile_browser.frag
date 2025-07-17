/ Author: gifi47
// Title:

#ifdef GL_ES
precision mediump float;

#endif

in vec2 uv;

out vec4 o_color;

uniform float time;
const vec2 start = vec2(0.5, 0);
const vec2 end = vec2(0.5, 1);
const vec2 end1 = vec2(0, 0.5);

void main() {
    //vec2 st = gl_FragCoord.xy/u_resolution.xy;
    //st.x = u_resolution.x/u_resolution.y;

vec2 st = uv;

vec2 dir = normalize(end - start);
vec2 dir1 = normalize(end1 - start);
float m = length(end - st);
float m1 = length(end1 - st);
float d = m / (m + m1);
float d1 = m1 / (m + m1);

vec2 dm = (-dir d);
vec2 dm1 = (-dir1 * d1);
vec3 color = vec3(0.);
vec2 new_uv = st + (dm + dm1) * u_time;
//vec2 new_uv = st + (-dir * d * u_time * 0.1);
 color = vec3(fract(new_uv.x), fract(new_uv.y), 0);

    o_color = vec4(color,1.0);
}