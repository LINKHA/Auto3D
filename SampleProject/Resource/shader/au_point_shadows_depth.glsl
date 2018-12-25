#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;

uniform mat4 shadowMatrices[6];
uniform vec3 lightPos;
uniform float far_plane;

varying vec4 FragPos; // FragPos from GS (output per emitvertex)

void VS()
{
    gl_Position = model * vec4(aPos, 1.0);
}

void FS()
{
    float lightDistance = length(FragPos.xyz - lightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}
