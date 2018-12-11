#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}



out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec4 ourColor;

void main()
{
	vec4 texColor = texture(texture1, TexCoord);
    if(texColor.a < 0.1)
        discard;
	FragColor = texture(texture1, TexCoord) * vec4(ourColor);
}