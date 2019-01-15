#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform samplerCube environmentMap;

varying vec3 WorldPos;



void VS()
{
    WorldPos = aPos;

	mat4 rotView = mat4(mat3(view));
	vec4 clipPos = projection * rotView * vec4(WorldPos, 1.0);

	gl_Position = clipPos.xyww;
}


void FS()
{		
    vec3 envColor = textureLod(environmentMap, WorldPos, 0.0).rgb;
    
    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
    
    gl_FragColor = vec4(envColor, 1.0);
}
