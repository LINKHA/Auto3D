#version 150

#include "CommonCode.frag"
#include "BRDF.frag"


layout(std140) uniform MaterialPS3
{
    vec3 albedo;
    float metallic;
	float roughness;
	float ao;
};

in vec4 vWorldPos;
in vec3 vNormal;
in vec2 vTexCoord;

out vec4 fragColor;    


void main()
{		
	vec3 worldPos = vec3(vWorldPos);
    vec3 normal = normalize(vNormal);	
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
