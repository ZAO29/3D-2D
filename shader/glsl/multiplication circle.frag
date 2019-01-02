// Author: Nicolas Bellot

// Title: multiplication circle

 

#ifdef GL_ES

precision mediump float;

#endif

 
#define NB_PT 500
#define NB_PTF 500.
#define PI 3.14159265359

uniform vec2 u_resolution;

uniform vec2 u_mouse;

uniform float u_time;

 

float droite(vec3 d, vec2 pt, float threshold)

{

    float a = d.x * pt.x + d.y * pt.y + d.z;

    //return step(abs(a),threshold);
    return max((threshold-abs(a))/threshold,0.);  
	 // return smoothstep(0.,1.,(threshold-abs(a))/threshold);	
       

}


float degrade(vec3 d, vec2 pt, float threshold)
{
    float a = droite(d,pt,threshold);
    
    for (int i=2; i< 5; i++)
    {
    	float ifl = float(i);
        float b = droite(d,pt,ifl*threshold);
        a = mix(a,b,ifl/(ifl+1.));
        
    }
    return a;
}

 

/* FAIL */

float insideCircle(vec2 c , vec2 pt, float r)

{

    vec2 n_pt = pt - c;

    //return step(dot(n_pt,n_pt), r*r);
    return smoothstep(1.,0.,2.*dot(n_pt,n_pt)/r*r);

}

 



 

 vec3 Pt2droite(vec2 p1,vec2 p2)
 {
     vec2 dir = p2 - p1;
     
     vec2 ortho = normalize(vec2(dir.y,-dir.x));
     	  
     
     return vec3(ortho,-dot(ortho,p1));
     
 }

 
float droitePt(vec2 st,float threshold,vec2 p1, vec2 p2)
{
    vec3 d = Pt2droite(p1,p2);
    
    return droite(d,st,threshold);
}




void main() {

    vec2 st = gl_FragCoord.xy/u_resolution.xy - vec2(0.5);
    vec3 color = vec3(0.);
	
    vec2 pt = vec2(0,0);  
    float radBigCircle = 0.5;
    float radLittleCircle = 0.05;
    //color = vec3(insideCircle(center,st,rad));
	float inside = 0.;
    float theta = 0.;
    float slow = 0.001;
    float mult = u_time/5.;
    vec2 multPt;
    float threshold = 0.001;
    for (int i=0;i<NB_PT;i++)
    {	
        float angle = float(i)/NB_PTF*2.*PI;
        float multAngle = angle * mult;
        pt = radBigCircle*vec2(cos(angle),sin(angle));
        multPt = radBigCircle*vec2(cos(multAngle),sin(multAngle));
        
        color.x = 1.005*mix(droitePt(st,0.001,pt,multPt),color.x,float(i)/float(i+1));
        color.y =1.0015* mix(droitePt(st,0.01,pt,multPt),color.y,float(i)/float(i+1));
        color.z = 1.0015*mix(droitePt(st,0.05,pt,multPt),color.z,float(i)/float(i+1));
        //color = max(val,color);
    }
    
    color *= 15.;
    

    //color += degrade(d,st,0.01);

    /// droite ///

    /*vec3 d1 = vec3(1.,1.,-1.0);
    vec3 d2 = vec3(-1.,1.,.0);

    float threshold = 0.05;

   	float a;
    

    threshold = (cos(u_time)+1.)/5.+0.01;
    vec3 color1,color2;
    color1.x = degrade(d1,st,threshold);
    color1.y = degrade(d1,st,threshold/2.);
	color1.z = degrade(d1,st,threshold/4.);
    
    
    color2.x = degrade(d2,st,threshold);
    color2.y = degrade(d2,st,threshold/2.);
	color2.z = degrade(d2,st,threshold/4.);
    
    color = mix(color1,color2,0.5);*/

    gl_FragColor = vec4(color.x,color.y,color.z,1.)*insideCircle(vec2(0.),st,0.5);;

}