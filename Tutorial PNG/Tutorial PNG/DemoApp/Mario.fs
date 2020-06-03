#version 430
out vec4 FragColor;


void main(void)
{ 
    float theta = timer*20.0;
      
    vec3 dir1 = vec3(cos(theta),0,sin(theta));
    vec3 dir2 = vec3(sin(theta),0,cos(theta));
    
    float diffuse1 = pow(dot(vVaryingNormal,dir1),2.0);
    float diffuse2 = pow(dot(vVaryingNormal,dir2),2.0);
    
    vec3 col1 = diffuse1 * vec3(1,0,0);
    vec3 col2 = diffuse2 * vec3(0,0,1);
    
    FragColor += vec4(col1 + col2, 1.0);
}