#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 viewProjection;

void main()
{
    TexCoords = aPos;
    vec4 pos =   vec4(aPos, 1.0) * viewProjection;
    gl_Position = pos.xyww;
}  