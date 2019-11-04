#version 150

#include "CommonCode.frag"
#include "BRDF.frag"

in vec4 vWorldPos;
in vec3 vNormal;
in vec4 vTangent;
in vec2 vTexCoord;
#ifdef NUMSHADOWCOORDS
in vec4 vShadowPos[NUMSHADOWCOORDS];
#endif

// material parameters
uniform sampler2D albedoMap0;
uniform sampler2D normalMap1;
uniform sampler2D metallicMap2;
uniform sampler2D roughnessMap3;
uniform sampler2D aoMap4;

// IBL
uniform samplerCube irradianceMap5;
uniform samplerCube prefilterMap6;
uniform sampler2D brdfLUT7;

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

// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
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
	
	
	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, albedo, metallic);
		
	int lightNum = GetLightNum();
	

		
	for(int i = 0; i < lightNum; ++i)
	{
		vec3 lightPosition = vec3(lightPositions[i]);
		vec3 lightColor = vec3(lightColors[i]);

		vec3 lightVec = normalize(lightPosition - worldPos);	 
		vec3 halfVec = normalize(viewVec + lightVec);		
		
		float distance = length(lightPosition - worldPos);	
		
		
		
		//

		//vec3 halfVec = normalize(viewVec + lightVec);
		float attenuation = 1.0 / (distance * distance);
	
		float VdotH = clamp(dot(halfVec, viewVec), 0.0, 1.0);
		float LdotH = clamp(dot(halfVec, lightVec), 0.0, 1.0);
		float NdotL = max(dot(normal, lightVec), 0.0);
		float NdotV = max(dot(viewVec, normal), 0.0);
		float NdotH = max(dot(normal, halfVec), 0.0);	
	
	
		vec3 radiance = lightColor * attenuation;
	
		float distributionValue = Distribution(NdotH,roughness);
		float geometryValue = Geometry(NdotL,NdotV,roughness);
		vec3 fresnelValue = Fresnel(F0,VdotH,LdotH);
	   
		vec3 nominator    = distributionValue * geometryValue * fresnelValue;
		float denominator = 4 * max(dot(normal, viewVec), 0.0) * max(dot(normal, lightVec), 0.0);
		vec3 specular = nominator / max(denominator, 0.001);

		vec3 kS = fresnelValue;
	
		vec3 kD = vec3(1.0) - kS;

		kD *= 1.0 - metallic;	  
		vec3 BRDFValue = 	(kD * albedo / M_PI + specular) * radiance * NdotL;  
		//
		Lo += BRDFValue;
	}
	
	  // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(normal, viewVec), 0.0), F0, roughness);
	    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
	
	//IBL
	vec3 irradiance = texture(irradianceMap5, normal).rgb;
	vec3 diffuse      = irradiance * albedo;
	
	// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap6, reflect(-viewVec, normal),  roughness * MAX_REFLECTION_LOD).rgb;    
	vec2 brdf  = texture(brdfLUT7, vec2(max(dot(normal, viewVec), 0.0), roughness)).rg;
	vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
	 
	 
	 
    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = ambient + Lo;

	//HDR to LDR
    color = color / (color + vec3(1.0));
	//Gamma Correction
    color = pow(color, vec3(1.0/2.2)); 

	#ifdef NUMSHADOWCOORDS
    //vec4 totalLight = CalculateLighting(vWorldPos, normal, vShadowPos);
    #else
    //vec4 totalLight = CalculateLighting(vWorldPos, normal);
    #endif

    fragColor = /*totalLight **/ vec4(color, 1.0);
}
