#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

#ifdef INSTANCED
in vec4 texCoord4;
in vec4 texCoord5;
in vec4 texCoord6;
#endif

layout(std140) uniform PerFrameVS0
{
	mat3x4 viewMatrix;
	mat4 projectionMatrix;
	mat4 viewProjMatrix;
	vec4 depthParameters;
};

layout(std140) uniform PerObjectVS1
{
	mat3x4 worldMatrix;
};

out vec4 vWorldPos;

out vec2 vTexCoord;

void main()
{
    #ifdef INSTANCED
		mat3x4 instanceWorldMatrix = mat3x4(texCoord4, texCoord5, texCoord6);
		vWorldPos.xyz = vec4(position, 1.0) * instanceWorldMatrix;
	#else
		vWorldPos.xyz = vec4(position, 1.0) * worldMatrix;
	#endif
	
	gl_Position = vec4(vWorldPos.xyz, 1.0) * viewProjMatrix;
    vTexCoord = texCoord;
};