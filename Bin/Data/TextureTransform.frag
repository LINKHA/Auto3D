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
	vec4 texColor = texture(Texture0, vTexCoord);
    if(texColor.a < 0.1)
        discard;
	fragColor = texture(Texture0, vTexCoord) * color;
};