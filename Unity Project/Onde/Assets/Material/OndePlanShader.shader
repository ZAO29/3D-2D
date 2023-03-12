Shader "Mine/OndePlanShader"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
		_ScriptTime("Time", Float) = 0
		_nbSource("nbSource", int) = 10
	}
	SubShader
	{


		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"


			
			#define PI 3.14159265359

			float _ScriptTime;
			int _nbSource;
			


			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
			};

			struct v2f
			{
				float2 uv : TEXCOORD0;
				float4 vertex : SV_POSITION;
			};

			v2f vert (appdata v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.uv = v.uv;
				return o;
			}
			
			

			half2 ratioCorrectedST(half2 st)
			{
				half2 ratio = half2(_ScreenParams.x / _ScreenParams.y, 1.);
				return st.xy*ratio;
			}

			half ondula(half2 st, half2 center, half shift, half period)
			{
				half r = distance(st, center);
				half ondula = (1. + cos(r*period - 5.*_ScriptTime + shift)) / 2. / (1. + 5.*r);
				return ondula;
			}

			half3 RGBondula(half2 st, half2 center, half3 RGBshift)
			{
				float val = 50.0;
				float period = val + (1. + cos(_ScriptTime)) / 2.*val;
				float ondeR = ondula(st, center, RGBshift.x, period);
				float ondeG = ondula(st, center, RGBshift.y, 2.*period);
				float ondeB = ondula(st, center, RGBshift.z, 3.*period);

				return half3(ondeR, ondeG, ondeB);

			}

			







			fixed4 frag(v2f f) : SV_Target{
				fixed4 col;
				
				//half2 stTex = ratioCorrectedST(_MainTex.xy);
				half2 st = ratioCorrectedST(f.uv);
				

				//float amplitude = min(_ScreenParams.x, _ScreenParams.y)/2.;

				half oscillation = pow(1. - abs(sin(_ScriptTime / 15.0)), 1.5);
				//half oscillation = pow(mod(_ScriptTime,1.),2.0);

				half ratio = _ScreenParams.x / _ScreenParams.y / 2.0;
				half2 center = half2(ratio, 0.5);
				

				half amplitude = ratio;

				half3 color = half3(0., 0., 0.);


				half theta = 2. / half(_nbSource) *PI;
				/*half theta = 2. / NB_SRC_MAXF * PI;*/
				half3 RGBshift = half3(3.14 / 2.*cos(_ScriptTime),
					3.14 / 4.*cos(_ScriptTime*2.0),
					-3.14 / 4.*cos(_ScriptTime*4.0));


				for (int i = 0;i < _nbSource;i++)
				{
					if (i < _nbSource)
					{
						half2 center1 = center + half2(amplitude*cos(half(i)*theta), amplitude*sin(half(i)*theta))*oscillation;
						//half2 center1 = center + half2(amplitude*cos(theta), amplitude*sin(theta))*oscillation;
						half3 color1 = RGBondula(st, center1, RGBshift);
						color += color1;
					}

					
				}

				color /= (half(_nbSource)/2.);
				
				col = half4(color, 1.0);
				return col;
			}




			
			ENDCG
		}
	}
}
