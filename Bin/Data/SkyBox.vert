#version 330
layout (location = 0) in vec3 position;
//in vec3 position;


out vec3 TexCoords;
layout(std140) uniform PerFrameVS0
{
	mat3x4 viewMatrix;
	mat4 projectionMatrix;
	mat4 viewProjMatrix;
	vec4 depthParameters;
};

//uniform mat4 viewProjMatrix;

void main()
{
    TexCoords = position;
    vec4 pos =   vec4(position, 1.0) * viewProjMatrix;
    gl_Position = pos.xyww;
}  