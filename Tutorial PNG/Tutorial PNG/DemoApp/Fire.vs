#version 430 core

layout(location=0)in vec3 aPos;
layout(location=2)in vec2 aTexCoord;

uniform mat4 fireMatrix;
uniform float translateX,translateY;

vec3 position=aPos;
out vec2 pos;

void main()
{
	position=(fireMatrix*vec4(aPos.x,aPos.y,0.,1.)).xyz;
	position+=vec3(-.265,-.7,0);
	position+=vec3(translateX,translateY,0);

	gl_Position=vec4(position,1.);
	//gl_Position=fireMatrix*vec4(aPos.x,aPos.y,0.,1.);
	
	pos = aTexCoord;
}