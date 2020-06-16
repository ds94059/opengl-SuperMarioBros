// Lightning
// By: Brandon Fogerty
// bfogerty at gmail dot com
// xdpixel.com

// By: Brandon Fogerty
// bfogerty at gmail dot com
// xdpixel.com

// EVEN MORE MODS BY 27

#ifdef GL_ES
precision lowp float;
#endif
// EVEN MORE MODS BY 27

#ifdef GL_ES
precision lowp float;
#endif
in vec2 pos;
uniform int timer,right;
uniform sampler2D texture2;

#define iTime timer
#define PI 3.14159265358979323844

float GetNoise(vec2 uv)// -> (0.25, 0.75)
{
	float n=(texture(texture2,uv).r-.5)*.5;
	n+=(texture(texture2,uv*2.).r-.5)*.5*.5;
	//n += (texture(iChannel0, uv * 3.0).r - 0.5) * 0.5 * 0.5;
	
	return n+.5;
}

void mainImage(vec4 fragColor,vec2 fragCoord)
{
	
	vec2 uv=fragCoord.xy;
	
	if(right == 1)
	{
		if(uv.x<.5)
		{
			gl_FragColor=vec4(0,0,0,0);
			return;
		}
	}
	else
	{
		if(uv.x>.5)
		{
			gl_FragColor=vec4(0,0,0,0);
			return;
		}
	}
	
	uv-=vec2(.5);// (x=[-0.5, 0.5], y=[-0.5, 0.5])
	//uv.y/=(iResolution.x/iResolution.y);// obey aspect ratio
	
	float paramTime=iTime;
	
	////////////////////////////////////////
	
	//
	// Transform to polar coordinates
	//
	
	// (r, a) (r=[0.0, 1.0], a=[0.0, 1.0 CCW from W])
	uv=vec2(
		length(uv)/sqrt(.5),
		(atan(uv.y,uv.x)/(2.*PI)+.5));
		
		// Scale radius to better fit in quad
		uv.x*=1.2;
		
		//
		// Flame time
		//
		
		#define FlameSpeed.2
		float flameTime=paramTime*FlameSpeed;
		
		//
		// Get noise for this fragment (in polar coordinates) and time
		//
		
		#define NoiseResolution.5
		float fragmentNoise=GetNoise(uv*vec2(NoiseResolution/2.,NoiseResolution)+vec2(-flameTime,0.));
		
		//
		// Randomize radius based on noise and radius
		//
		
		float variationR=(fragmentNoise-.5);
		
		// Straighten the flame at the center and make full turbulence outside
		variationR*=smoothstep(.05,.4,uv.x);
		
		// Scale variation
		variationR*=.65;
		
		// Randomize!
		uv.x+=variationR;
		
		//
		// Calculate brightness
		//
		
		float radius1=uv.x;
		
		// Focus (compress dynamics)
		float radius2=smoothstep(.2,.35,radius1);
		
		float brightness=1.-radius2;
		
		//
		// Emit
		//
		vec3 temp=vec3(0.,0.,0.);
		
		vec3 col1=mix(vec3(1.,1.,1.),vec3(1.,1.,.6),smoothstep(.1,.2,radius1));
		col1=mix(col1,vec3(1.,.4,.1),smoothstep(.18,.25,radius1));
		
		// Blend with background
		float alpha=brightness;
		fragColor=mix(vec4(0.,0.,0.,1.),vec4(col1,1.),alpha);
		
		float grayx=fragColor.x*.299;
		float grayy=fragColor.y*.587;
		float grayz=fragColor.z*.114;
		
		float gray=grayx+grayy+grayz;
		
		if(gray==0)
		gl_FragColor=vec4(0,0,0,0);
		else
		gl_FragColor=fragColor;
	}
	
	void main(void)
	{
		vec2 uv=pos;
		// mainImage(gl_FragColor,uv.xy);
		
		// float v2=abs(pow((uv.x-0.03)*.2,.5));
		
		// float v1=abs(pow((uv.x)*.2,.5));
		
		// if(abs(uv.y-0.5)<=v1&&abs(uv.y-.5)>=v2)
		// 	gl_FragColor=vec4(.9451,.4588,.1373,1.);
		// else
		// 	gl_FragColor=vec4(1.,1.,1.,0.);
		vec4 temp=gl_FragColor;
		mainImage(temp,uv);
		
		//gl_FragColor=vec4(0.7529, 0.251, 0.251, 0.0);
		
		// float temp = pow((uv.y-0.5),2)-0.2*(uv.x-0.3);
		
		// if(abs(temp)<=0.005)
		// 	gl_FragColor=vec4(0.9451, 0.4588, 0.1373, 1.0);
		// else
		// 	gl_FragColor=vec4(1.0, 1.0, 1.0, 0.0);
		
		//gl_FragColor=vec4(uv.y,uv.y,uv.y,1);
		
		// gl_FragColor=vec4(1-uv.y,1-uv.y,1-uv.y,1);
		
		// // if(gl_FragColor.x>0.4 && gl_FragColor.x<0.6)
		// // 	gl_FragColor=vec4(0.8667, 0.2314, 0.2314, 1);
		// // else
		// // 	gl_FragColor=vec4(1-uv.y,1-uv.y,1-uv.y,0);
	}