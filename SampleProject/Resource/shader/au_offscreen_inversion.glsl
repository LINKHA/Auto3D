#version 430 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec4 FragColor;

varying vec2 TexCoords;

uniform sampler2D screenTexture;


void VS()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}

void FS()
{
	FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);    
} 


