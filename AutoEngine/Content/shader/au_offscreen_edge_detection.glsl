#version 430 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec4 FragColor;

varying vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;  

void VS()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}

void FS()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), 
        vec2( 0.0f,    offset), 
        vec2( offset,  offset), 
        vec2(-offset,  0.0f),   
        vec2( 0.0f,    0.0f),   
        vec2( offset,  0.0f),  
        vec2(-offset, -offset), 
        vec2( 0.0f,   -offset), 
        vec2( offset, -offset) 
    );
	float kernel[9] = float[](
		1, 1, 1,
        1,  -8, 1,
        1, 1, 1
	);
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
		
    FragColor = vec4(col, 1.0);
}