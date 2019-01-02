// Author: Nicolas Bellot

// Title: Sun moving around a single source

 

#ifdef GL_ES

precision mediump float;

#endif

 

#define PI  3.1416

#define PI2  6.2832

 

uniform vec2 u_resolution;

uniform vec2 u_mouse;

uniform float u_time;

 

void main() {

    float speed = 1.;

    float speedSun = 0.1;

    float t = mod(u_time*speedSun,PI);

    //float t =u_time;

    vec3 lightDir = vec3(cos(t),0.,sin(t));

    vec2 st = gl_FragCoord.xy/u_resolution.xy;

    st.x *= u_resolution.x/u_resolution.y;

                st -= vec2(0.5);

    st*=50.;

    vec3 color = vec3(0.);

    float r = sqrt(st.x*st.x + st.y*st.y);

    float val = (1. + cos(speed*r-u_time))/2.;

   

                float theta = atan(st.y,st.x);

   

    float display_theta = mod(theta,PI2)/PI2;

   

    vec3 r_vec = vec3(cos(theta),sin(theta),0.);

    vec3 z = vec3(0.,0.,1.);

    vec3 normal = speed*sin(speed*r-u_time)*r_vec + z;

   

    normal = normal/sqrt(normal.x*normal.x+normal.y*normal.y+normal.z*normal.z);

   

    float scalarProd = dot(normal,lightDir);

   

    

    color = vec3(0.);

    color = vec3(val,val,val);

    color = vec3(scalarProd);

    gl_FragColor = vec4(color,scalarProd);

}