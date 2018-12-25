#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


uniform sampler2D texture_diffuse1;
out vec4 FragColor;

varying vec2 TexCoords;

varying VS_OUT {
    vec2 texCoords;
} vs_out;



void main()
{
    vs_out.texCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
}


void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
}

