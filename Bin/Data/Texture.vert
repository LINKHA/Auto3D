#version 150

in vec3 position;
in vec2 texCoord;
in vec3 texCoord1; // objectPosition

out vec2 vTexCoord;

void main()
{
	gl_Position = vec4(position + texCoord1, 1.0);
	vTexCoord = texCoord;
}