#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

varying vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void VS()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0f); 
}

void FS()
{
    FragColor = texture(texture_diffuse1, TexCoords);
}