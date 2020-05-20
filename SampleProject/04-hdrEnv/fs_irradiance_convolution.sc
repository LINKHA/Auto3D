$input v_position

#include "../common.sh"

SAMPLERCUBE(s_equirectangulaCubeMap, 0);

void main()
{		
	float PI = 3.14159265359;

    vec3 N = normalize(v_position);

    vec3 irradiance = vec3_splat(0.0);   
    
    // tangent space calculation from origin point
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, N);
    up         = cross(N, right);
       
    float sampleDelta = 0.025;
    float nrSamples = 0.0f;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(mul(sin(theta), cos(phi)),  mul(sin(theta), sin(phi)), cos(theta));
            // tangent space to world
            vec3 sampleVec = mul(tangentSample.x, right) + mul(tangentSample.y, up) + mul(tangentSample.z, N); 

            irradiance += mul(mul(textureCube(s_equirectangulaCubeMap, sampleVec).rgb, cos(theta)), sin(theta));
            nrSamples++;
        }
    }
    irradiance = mul(mul(PI, irradiance), (1.0 / float(nrSamples)));
    
    gl_FragColor = vec4(irradiance, 1.0);
}
