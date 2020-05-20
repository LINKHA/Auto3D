$input v_position

#include "../common.sh"

SAMPLER2D(s_equirectangularMap, 0);

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan2(v.z, v.x), asint(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}



void main()
{
	vec2 uv = SampleSphericalMap(normalize(v_position));

    vec3 color = texture2D(s_equirectangularMap, uv).rgb;

	gl_FragColor = vec4(color, 1.0);
}
