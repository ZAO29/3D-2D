#version 460
out vec4 FragColor;
  
in vec2 UV;

uniform sampler2D gRenderedTex;
  
uniform bool gHorizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(gRenderedTex, 0); // gets size of single texel
    vec3 result = texture(gRenderedTex, UV).rgb * weight[0]; // current fragment's contribution
    if(gHorizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(gRenderedTex, UV + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(gRenderedTex, UV - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(gRenderedTex, UV + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(gRenderedTex, UV - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}

