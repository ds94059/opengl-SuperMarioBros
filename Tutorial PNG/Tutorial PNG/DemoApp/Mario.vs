#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
vec3 position = aPos;

uniform float translateX,translateY;
uniform int timer;
uniform mat4 coinMatrix;

void main()
{
	gl_Position = coinMatrix * vec4(aPos.x,aPos.y,0.0,1.0);

	position += vec3(-0.305, -0.7, 0);
    position += vec3(translateX, translateY + timer*0.02, 0);
	gl_Position = vec4(position, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}