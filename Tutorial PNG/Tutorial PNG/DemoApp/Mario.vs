#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
vec3 position = aPos;

uniform float translateX,translateY;
uniform int timer;

void main()
{
    position += vec3(translateX,translateY,0);
	gl_Position = vec4(position, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}