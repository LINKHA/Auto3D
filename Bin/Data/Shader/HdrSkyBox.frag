#version 330

uniform samplerCube skybox;

in vec3 TexCoords;
out vec4 FragColor;

void main()
{    
	vec3 envColor = texture(skybox, TexCoords).rgb;

	// HDR tonemap and gamma correct
	// envColor = envColor / (envColor + vec3(1.0));
	// envColor = pow(envColor, vec3(1.0/2.2)); 

	FragColor = vec4(envColor, 1.0);
}