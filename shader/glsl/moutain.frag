// Author:
// Title:

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

float rand(vec2 st)
{

    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}


float alt(vec2 st, float grid)
{
    vec2 fl = floor(st)/grid;
    
    return rand(fl);
     //return (floor(st.x)+floor(st.y)*grid)/(grid*grid);
    //return (floor(st.x)+floor(st.y))/(2.*grid);
}

float mean(float w, float A, float B)
{
    return A*(1.-w)+B*w;
}


float mean2D(vec2 w, float UV, float UV1,float U1V, float U1V1)
{
    float A = mean(w.y,UV,UV1);
    float B = mean(w.y,U1V,U1V1);
    
    return mean(w.x,A,B);
    
}

float level(float grid,vec2 st)
{
    
    
    st *= grid;
    
    float altUV = alt(st+vec2(0.),grid);
    float altU1V = alt(st+vec2(1.,0.),grid);
    float altUV1 = alt(st+vec2(0.,1.),grid);
    float altU1V1 = alt(st+vec2(1.,1.),grid);
    
     vec2 w = fract(st);
    
    return mean2D(w,altUV,altUV1,altU1V,altU1V1);
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    //st.x *= u_resolution.x/u_resolution.y;

    float grid = 4.;
    
   float altInterp = level(4.,st)/2.;
         altInterp += level(8., st)/4.;
    	 altInterp += level(16., st)/8.;
    	 altInterp += level(32., st)/16.;
    
    vec3 color = vec3(0.);
    //color = vec3(altU1V,altU1V,altU1V);
    
     color = vec3(altInterp,altInterp,altInterp);

    gl_FragColor = vec4(color,1.0);
}