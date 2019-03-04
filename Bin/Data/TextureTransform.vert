#version 150
in vec3 position;
in vec2 texCoord;
in vec3 texCoord1; // objectPosition

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
	vWorldPos.xyz = vec4(position, 1.0) * worldMatrix;
	gl_Position = vec4(vWorldPos.xyz, 1.0) * viewProjMatrix;
   vTexCoord = texCoord;
};