#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform int timer;

void main()
{
	float float_timer=timer;
	vec4 color = vec4(sin(float_timer),cos(float_timer),tan(float_timer),-timer*0.08);
	FragColor += color;
	FragColor += texture(texture1, TexCoord);
	if(FragColor.z>1)
		FragColor.z=1;
	else if(FragColor.z<0)
		FragColor.z=0;
}