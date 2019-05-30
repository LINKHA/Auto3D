#version 150

#include "CommonCode.frag"
#include "BRDF.frag"

in vec4 vWorldPos;
in vec3 vNormal;
in vec4 vTangent;
in vec2 vTexCoord;

// material parameters
uniform sampler2D albedoMap0;
uniform sampler2D normalMap1;
uniform sampler2D metallicMap2;
uniform sampler2D roughnessMap3;
uniform sampler2D aoMap4;

out vec4 fragColor;    

vec3 getNormalFromMap(sampler2D normalMap,vec2 texCoord,vec3 normal,vec4 worldPos)
{
    vec3 tangentNormal = texture(normalMap, texCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(vec3(worldPos));
    vec3 Q2  = dFdy(vec3(worldPos));
    vec2 st1 = dFdx(texCoord);
    vec2 st2 = dFdy(texCoord);

    vec3 N   = normalize(normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{		
	vec3 albedo     = pow(texture(albedoMap0, vTexCoord).rgb, vec3(2.2));
    float metallic  = texture(metallicMap2, vTexCoord).r;
    float roughness = texture(roughnessMap3, vTexCoord).r;
    float ao        = texture(aoMap4, vTexCoord).r;

	vec3 worldPos = vec3(vWorldPos);
    vec3 normal = getNormalFromMap(normalMap1,vTexCoord,vNormal,vWorldPos);	
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
