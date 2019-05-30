#version 150

#include "CommonCode.frag"
#include "BRDF.frag"


// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

in vec4 vWorldPos;
in vec3 vNormal;
in vec2 vTexCoord;

out vec4 fragColor;    

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, vTexCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(vec3(vWorldPos));
    vec3 Q2  = dFdy(vec3(vWorldPos));
    vec2 st1 = dFdx(vTexCoord);
    vec2 st2 = dFdy(vTexCoord);

    vec3 N   = normalize(vNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{		
	vec3 albedo     = pow(texture(albedoMap, vTexCoord).rgb, vec3(2.2));
    float metallic  = texture(metallicMap, vTexCoord).r;
    float roughness = texture(roughnessMap, vTexCoord).r;
    float ao        = texture(aoMap, vTexCoord).r;

	vec3 worldPos = vec3(vWorldPos);
    vec3 normal = getNormalFromMap();	
    vec3 viewVec = normalize(viewPosition - worldPos); 

	vec3 Lo = vec3(0.0);
	
	int lightNum = GetLightNum();
	
	for(int i = 0; i < lightNum; ++i)			
	{
		vec3 lightPosition = vec3(lightPositions[i]);
		vec3 lightColor = vec3(lightColors[i]);

		vec3 lightVec = normalize(lightPosition - worldPos);	 
		vec3 half = normalize(viewVec + lightVec);		
		
		float distance = length(lightPosition - worldPos);	
		
		vec3 BRDFValue = GetBRDF(lightVec,viewVec,normal,lightColor,distance,albedo,metallic,roughness);
		Lo += BRDFValue;
	}
	
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

	//HDR to LDR
    color = color / (color + vec3(1.0));
	//Gamma Correction
    color = pow(color, vec3(1.0/2.2)); 

    fragColor = vec4(color, 1.0);
}
