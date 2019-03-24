#version 150 core
in vec3 position;

out vec3 TexCoords;
layout(std140) uniform PerFrameVS0
{
	mat3x4 viewMatrix;
	mat4 projectionMatrix;
	mat4 viewProjMatrix;
};

void main()
{
    TexCoords = position;
    vec4 pos = viewProjMatrix * vec4(position, 1.0);
    gl_Position = pos.xyww;
}  