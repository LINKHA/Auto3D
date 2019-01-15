#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void VS()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}

void FS()
{             
    // gl_FragDepth = gl_FragCoord.z;
}