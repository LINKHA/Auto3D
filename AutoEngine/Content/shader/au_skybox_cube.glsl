#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

varying vec3 Normal;
varying vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 FragColor;


uniform vec3 cameraPos;
uniform samplerCube skybox;

void VS()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

void FS()
{    
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}