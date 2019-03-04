#version 150

layout(std140) uniform ConstantBuffer0
{
	vec4 color;
};

uniform sampler2D Texture0;
in vec2 vTexCoord;
out vec4 fragColor;

void main()
{
	fragColor = color * texture(Texture0, vTexCoord);
};