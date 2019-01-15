#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (location = 0) out vec4 FragColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;



uniform vec3 lightColor;


void VS()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

void FS()
{           
    FragColor = vec4(lightColor, 1.0);
}

