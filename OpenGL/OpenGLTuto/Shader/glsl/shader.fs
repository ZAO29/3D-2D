#version 460

layout (location = 0) out vec3 FragColor;
layout (location = 1) out vec3 BrightColor;

in vec4 Color;
in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;
in vec3 Tangent0;


struct Light
{
    vec3 Color;
    float AmbientIntensity;
    vec3 DirOrPos;
    float DiffuseIntensity;
    bool isDir;
};

struct Specular
{
   float intensity;
   float power;
};



uniform sampler2D gSampler;
uniform sampler2D gSamplerNormalMap;
uniform Light gLight;
uniform vec3 gWorldEyePos;
uniform Specular gSpecular;
uniform bool gDebug;
uniform float gThresholdBrightness;

mat3 cotangent_frame( vec3 N, vec3 p, vec2 uv ) { 
    // get edge vectors of the pixel triangle 
    vec3 dp1 = dFdx( p ); 
    vec3 dp2 = dFdy( p ); 
    vec2 duv1 = dFdx( uv ); 
    vec2 duv2 = dFdy( uv );   
    // solve the linear system 
    vec3 dp2perp = cross( dp2, N ); 
    vec3 dp1perp = cross( N, dp1 ); 
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x; 
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;   
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) ); 
    return mat3( T * invmax, B * invmax, N ); 
}




vec3 CalcBumpedNormal(vec3 pos2Eye)                                                                     
{   

    ivec2 texSize = textureSize(gSamplerNormalMap,0);
    vec3 Normal = normalize(Normal0);
    if(texSize.x * texSize.y <= 1)
    {
       return Normal;      
    }
        
                                          
    vec3 BumpMapNormal = texture(gSamplerNormalMap, TexCoord0).xyz;                                
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);                              
                                                                                                                       
    mat3 TBN = cotangent_frame(Normal, -pos2Eye, TexCoord0); 
    return normalize(TBN * BumpMapNormal);                                                                                                                               
}

vec4 CalcDiffuseColor(vec3 normal,vec3 ldir)
{
    float DiffuseFactor = dot(normal, -ldir);
    vec4 DiffuseColor = vec4(gLight.Color,1.0) * DiffuseFactor * gLight.DiffuseIntensity;

    return DiffuseColor;
}


vec4 CalcSpecularColor(vec3 normal,vec3 ldir, vec3 Pos2Eye)
{
    float SpecularFactor = 0.f;

    vec3 reflectLdir = reflect(ldir,normal);
    SpecularFactor = max(dot(Pos2Eye,reflectLdir),0.0);
    SpecularFactor = pow(SpecularFactor,gSpecular.power); 
    SpecularFactor *= gSpecular.intensity;
    
    if(SpecularFactor < 0)
    {
        SpecularFactor = 0.;
    }
    
    return vec4(vec3(SpecularFactor),1.0);
}


void main()
{
    if(gDebug)
    {
        FragColor = vec3(1.0,0.0,0.0);
        return;
    }

    vec3 Pos2Eye = normalize(gWorldEyePos -   WorldPos0);

    vec3 Normal = CalcBumpedNormal(Pos2Eye);
    

    /// TEST to distinguish directionnal light from point light
    vec3 ldir;

    if(gLight.isDir)
    {
        ldir = normalize( gLight.DirOrPos);
    }
    else
    {   
        ldir = normalize(WorldPos0 - gLight.DirOrPos);
    }
     

    vec4 AmbientColor = gLight.AmbientIntensity * vec4(gLight.Color,1.0);
    
    vec4 DiffuseColor = CalcDiffuseColor(Normal, ldir);

    vec4 SpecularColor = CalcSpecularColor(Normal, ldir, Pos2Eye);


    
    

   FragColor =  (DiffuseColor.xyz + AmbientColor.xyz + SpecularColor.xyz) * texture2D(gSampler, TexCoord0.xy).xyz;

   

   BrightColor = step(gThresholdBrightness,dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722)))*FragColor; 

   
   //BrightColor = texture(gSamplerNormalMap, TexCoord0).xyz;
   //FragColor = texture(gSamplerNormalMap, TexCoord0).xyz; 

    
	
}
