//Author : Nicolas Bellot
// 2 January 2019
// Title : 4 wave source moving around

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

float ondula(vec2 st,vec2 center,float shift, float period)
{
    float r = distance(st,center);    
    float ondula = (1.+cos(r*period-5.*u_time+shift))/2./(1.+5.*r);
    return ondula;
}

vec3 RGBondula(vec2 st, vec2 center, vec3 RGBshift)
{	
    float val = 50.0;
    float period = val + (1.+cos(u_time))/2.*val;
    float ondeR = ondula(st,center,RGBshift.x,period);
    float ondeG = ondula(st,center,RGBshift.y,2.*period);
    float ondeB = ondula(st,center,RGBshift.z,3.*period);
    
    return vec3(ondeR,ondeG,ondeB);
    
}


void main(){
    float amplitude = 0.5;
    float oscillation = pow(1.-abs(sin(u_time/15.0)),1.5);
    //float oscillation = pow(mod(u_time,1.),2.0);
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    float r = distance(st,vec2(0.5));
    vec2 center = vec2(0.5);
    vec2 center1 = center+vec2(amplitude,amplitude)*oscillation;
    vec2 center2 = center+vec2(-amplitude,amplitude)*oscillation;
     vec2 center3 = center+vec2(amplitude,-amplitude)*oscillation;
    vec2 center4 = center+vec2(-amplitude,-amplitude)*oscillation;
   
    vec3 RGBshift = vec3(3.14/2.*cos(u_time),
                        3.14/4.*cos(u_time*2.0),
                        -3.14/4.*cos(u_time*4.0));
    
	vec3 color1 = RGBondula(st,center1,RGBshift);
    vec3 color2 = RGBondula(st,center2,RGBshift);
    vec3 color3 = RGBondula(st,center3,RGBshift);
    vec3 color4 = RGBondula(st,center4,RGBshift);
    
    
    
    vec3 color = (color1+color2+color3+color4);

   

    gl_FragColor = vec4(color,1.0);
}
