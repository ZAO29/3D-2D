#version 330 core
out vec3 FragColor;
  
in vec2 UV;

uniform sampler2D gScene;
uniform sampler2D gBloomBlur;
uniform float exposure = 1.0;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(gScene, UV).rgb;      
    vec3 bloomColor = texture(gBloomBlur, UV).rgb;
    hdrColor += bloomColor; // additive blending
    FragColor = hdrColor + bloomColor;
    // tone mapping
    //vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    //result = pow(result, vec3(1.0 / gamma));
    //FragColor = vec4(result, 1.0);
    
}