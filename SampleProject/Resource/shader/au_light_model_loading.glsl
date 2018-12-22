#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

varying vec2 TexCoords;
varying vec3 Normal;
varying vec3 FragPos;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


struct Material {
	vec3 color;
    vec3 diffuse;
    vec3 specular;
    float shininess;

}; 
struct DirLight {
	vec3 color;
	vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {
	vec3 color;
	vec3 position;

    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct SpotLight {
	vec3 color;
	vec3 position;

    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform Material material;
uniform DirLight dirLight[4];
uniform PointLight pointLight[8];
uniform SpotLight spotLight[4];

uniform bool dirBool;
uniform bool pointBool;
uniform bool spotBool;

uniform int dirNum;
uniform int pointNum;
uniform int spotNum;


uniform vec3 viewPos;


vec3 CalcDirLight(DirLight light,vec3 normal ,vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * material.diffuse * material.color;
    vec3 diffuse = light.diffuse * diff * material.diffuse * material.color;
    vec3 specular = light.specular * spec * material.specular * material.color;
    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * material.diffuse* material.color;
    vec3 diffuse = light.diffuse * diff * material.diffuse* material.color;
    vec3 specular = light.specular * spec * material.specular* material.color;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * material.diffuse* material.color;
    vec3 diffuse = light.diffuse * diff * material.diffuse* material.color;
    vec3 specular = light.specular * spec * material.specular* material.color;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}


void VS()
{ 
	FragPos = vec3(model * vec4( aPos , 1.0));
	TexCoords = aTexCoords;   
	Normal = mat3(transpose(inverse(model))) * aNormal;//temp use (need use cpu get normal vex)
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

void FS()
{   
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);					
    vec3 result = vec3(0,0,0);

	if(dirBool)
	{
		for(int i=0;i<dirNum;i++)
			result += CalcDirLight(dirLight[i],norm,viewDir);
	}
	if(pointBool)
	{
		for(int i=0;i<pointNum;i++)
			result += CalcPointLight(pointLight[i],norm, FragPos, viewDir);
	}
	if(spotBool)
	{
		for(int i=0;i<spotNum;i++)
			result += CalcSpotLight(spotLight[i], norm, FragPos, viewDir);
	} 
    gl_FragColor = vec4(result, 1.0);
}