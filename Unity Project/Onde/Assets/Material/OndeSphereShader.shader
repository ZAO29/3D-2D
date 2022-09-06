Shader "Mine/OndeSphereShader"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
		_ScriptTime("Time", Float) = 0
		_nbSource("nbSource", int) = 10
		_Center("Center", Vector) = (1,1,1,1)
		_Shift("Shift", Vector) = (1,2,4,1)
	}
	SubShader
	{
		// No culling or depth
		Cull Front ZWrite On ZTest Always

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"


			
			#define PI 3.14159265359

			sampler2D _MainTex;
			float _ScriptTime;
			int _nbSource;
			float4 _Center;
			float4 _Shift;


			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
			};

			struct v2f
			{
				float2 uv : TEXCOORD0;
				float4 vertex : SV_POSITION;
				float3 localSpaceVert : TEXCOORD1;
			};


			half ondula(half3 st, half3 center, half shift, half period)
			{
				half r = dot(normalize(st), normalize(center));
				half ondula = (1. + cos(period*r + 10 * _ScriptTime)) / 2.;
				return ondula;
			}

			half3 RGBondula(half3 st, half3 center, half3 RGBshift)
			{
				float val = 50.0;
				float period = 10;
				float ondeR = ondula(st, center, RGBshift.x, period);
				float ondeG = ondula(st, center, RGBshift.y, 2.*period);
				float ondeB = ondula(st, center, RGBshift.z, 3.*period);

				return half3(ondeR, ondeG, ondeB);

			}


			v2f vert (appdata v)
			{
				v2f o;

				half3 color = RGBondula(v.vertex.xyz, _Center.xyz, _Shift.xyz);
				float offset = (color.x + color.y / 2. + color.z / 3.)*5./6.*0.1;
				float3 v3 = (1. + offset) * v.vertex.xyz;
				o.vertex = UnityObjectToClipPos(float4(v3,1.));
				o.uv = v.uv;
				o.localSpaceVert = float3(v.vertex.x,v.vertex.y,v.vertex.z);
				return o;
			}
			
			fixed4 frag(v2f f) : SV_Target{
				fixed4 col = tex2D(_MainTex, f.uv);
				
				half3 center = half3(1., 1., 1.);
				half3 v = half3(f.localSpaceVert.x, f.localSpaceVert.y, f.localSpaceVert.z);
				half3 RGBshift = half3(1., 2., 4.);
				half3 color = RGBondula(v, _Center.xyz, _Shift.xyz);

				col = half4(color, 1.0);
				//col = half4(v, 1.0);
				return col;
			}




			
			ENDCG
		}
	}
}
