
#ifdef GL_ES
precision lowp float;
#endif


#ifdef GL_ES
precision lowp float;
#endif
in vec2 pos;
uniform int timer;

#define iTime timer
#define PI 3.14159265358979323844
void main(void)
{
    vec2 uv = pos;
    float floattimer = timer;
    floattimer -=120;
    if(sqrt(pow(uv.x-0.5,2)+pow(uv.y-0.5,2))>(1/floattimer))
    {
        gl_FragColor = vec4(0,0,0,1);
    }
    else if(floattimer>100)
    {
        gl_FragColor = vec4(0,0,0,1);
    }

}