#version 430 core
layout (location = 0) in vec3 aPos;

varying vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform samplerCube skybox;	

out vec4 FragColor;

void VS()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}


void FS()
{    
    FragColor = texture(skybox, TexCoords);
}