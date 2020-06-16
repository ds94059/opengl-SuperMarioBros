#version 430 core

layout(location=0)in vec3 aPos;
layout(location=2)in vec2 aTexCoord;

uniform mat4 fireMatrix;
uniform float translateX,translateY;

vec3 position=aPos;
out vec2 pos;

void main()
{
	
	gl_Position=vec4(position,1.);
	pos = aTexCoord;
}