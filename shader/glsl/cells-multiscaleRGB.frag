// Author: Nicolas Bellot
// Title: Cellular Noise

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

float grid(vec2 st,float nbCell)
{
    // Scale
    st *= nbCell;
	
    // Tile the space
    vec2 i_st = floor(st);
    vec2 f_st = fract(st);

    float m_dist = 1.;  // minimun distance

    for (int y= -1; y <= 1; y++) {
        for (int x= -1; x <= 1; x++) {
            // Neighbor place in the grid
            vec2 neighbor = vec2(float(x),float(y));

            // Random position from current + neighbor place in the grid
            vec2 point = random2(i_st + neighbor);

			// Animate the point
            point = 0.5 + 0.5*sin(u_time + 6.2831*point);

			// Vector between the pixel and the point
            vec2 diff = neighbor + point - f_st;

            // Distance to the point
            //diff.x*=2.+cos(u_time);
            float dist = length(diff);
			//float dist = abs(diff.x)+abs(diff.y);	
            // Keep the closer distance
            m_dist = min(m_dist, dist);
        }
    }
	float value = 0.;
    // Draw the min distance (distance field)
    value += m_dist;

    // Draw cell center
    //value += 1.-step(.02, value);

    // Draw grid
    //color.r += step(.98, f_st.x) + step(.98, f_st.y);

    // Show isolines
    // color -= step(.7,abs(sin(27.0*m_dist)))*.5;
    
    return value;

}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;
	float nbcell = 2.0;
    
    float R = grid(st,nbcell);
    float G = grid(st,pow(nbcell,2.));
    float B = grid(st,pow(nbcell,3.)) ;
    float alpha = grid(st,pow(nbcell,4.)) ; 
    gl_FragColor = vec4(R,G,B,1.);
}
