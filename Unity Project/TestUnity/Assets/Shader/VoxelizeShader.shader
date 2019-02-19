Shader "Custom/VoxelizeShader"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
		_SpatialRes("SpatialRes", float) = 2. 
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            // make fog work
            #pragma multi_compile_fog

            #include "UnityCG.cginc"
			
			#define PI 3.1415926535

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
				float3 color : COLOR;
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex;
            float4 _MainTex_ST;
			float _SpatialRes;

            v2f vert (appdata v)
            {
                v2f o;
				/*v.vertex.xyz *= 5.;*/
				v.vertex.xyz *= _SpatialRes;
				v.vertex.xyz = floor(v.vertex.xyz);
				v.vertex.xyz /= _SpatialRes;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);
				//o.color.xyz = 1 + cos(10* v.vertex.xyz/(2*PI))/2.;
				o.color.xyz = float3(0., 0., 0.);
				o.color.xyz += 1./3.*(1 + cos(10.1*_SpatialRes * v.vertex.xyz+_Time*_SpatialRes));
				o.color.xyz += 2./3.*(1 + cos(5.1*_SpatialRes * v.vertex.xyz + _Time*_SpatialRes));
				
				o.color.xyz *= _SpatialRes;
				o.color.xyz = floor(o.color.xyz);
				o.color.xyz /= _SpatialRes;

				UNITY_TRANSFER_FOG(o,o.vertex);
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // sample the texture
				fixed4 col = fixed4(0.,0.,0.,1.);
				col.xyz = i.color.x*i.color.y*i.color.z;
				//col.xyz = i.color.xyz;
                // apply fog
                
                return col;
            }
            ENDCG
        }
    }
}
