#version 330
#include "CommonCode.vert"

in vec3 position;
in vec3 normal;
in vec4 tangent;
in vec2 texCoord;


out vec3 TexCoords;

void main()
{
    TexCoords = position;
	vec4 pos = vec4(position, 1.0) * mat4(mat3(viewMatrix)) * projectionMatrix;
    gl_Position = pos.xyww;
}
