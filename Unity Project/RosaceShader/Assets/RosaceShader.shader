Shader "Mine/RosaceShader"
{
	Properties
	{
		// to redefine under
		_MainTex ("Texture", 2D) = "white" {} 
		_ScriptTime("Time", Float) = 0
		_NbPoint("NbPoint", int) = 128
	}
	SubShader
	{
		// No culling or depth
		Cull Off ZWrite Off ZTest Always

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"


			#define PI 3.14159265359


			sampler2D _MainTex;
			float _ScriptTime;
			int _NbPoint;


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
			
			


			//Frag stage
			half2 ratioCorrectedST(half2 st)
			{
				half2 ratio = half2(_ScreenParams.x / _ScreenParams.y, 1.);
				return st.xy*ratio;
			}

			float droite(half3 d, half2 pt, float threshold)

			{

				float a = d.x * pt.x + d.y * pt.y + d.z;

				//return step(abs(a),threshold);
				return max((threshold - abs(a)) / threshold, 0.);
				// return smoothstep(0.,1.,(threshold-abs(a))/threshold);	


			}

			half3 Pt2droite(half2 p1, half2 p2)
			{
				half2 dir = p2 - p1;

				half2 ortho = normalize(half2(dir.y, -dir.x));


				return half3(ortho, -dot(ortho, p1));

			}

			float droitePt(half2 st, float threshold, half2 p1, half2 p2)
			{
				half3 d = Pt2droite(p1, p2);

				return droite(d, st, threshold);
			}

			float degrade(half3 d, half2 pt, float threshold)
			{
				float a = droite(d, pt, threshold);

				for (int i = 2; i< 5; i++)
				{
					float ifl = float(i);
					float b = droite(d, pt, ifl*threshold);
					a = lerp(a, b, ifl / (ifl + 1.));

				}
				return a;
			}


			float insideCircle(half2 c, half2 pt, float r)

			{

				half2 n_pt = pt - c;

				//return step(dot(n_pt,n_pt), r*r);
				return smoothstep(1., 0., 2.*dot(n_pt, n_pt) / r * r);

			}









			fixed4 frag(v2f f) : SV_Target{
				fixed4 col = tex2D(_MainTex, f.uv);
				//_NbPoint = 5;
				


				half2 center;
				half ratio;

					ratio = _ScreenParams.y / _ScreenParams.x / 2.0;
					center = half2(0.5,ratio);
				}

				half2 st = ratioCorrectedST(f.uv) - center;

				half3 color = half3(0.,0.,0.);

				half2 pt = half2(0, 0);
				float radBigCircle = ratio;
				float radLittleCircle = 0.05;
				//color = half3(insideCircle(center,st,rad));
				float inside = 0.;
				float theta = 0.;
				float slow = 0.001;
				float mult = _ScriptTime;
				half2 multPt;
				float threshold = 0.001;
				for (int i = 0;i<_NbPoint;i++)
				{
					float angle = float(i) / float(_NbPoint) * 2.*PI;
					float multAngle = angle * mult;
					pt = radBigCircle * half2(cos(angle), sin(angle));
					multPt = radBigCircle * half2(cos(multAngle), sin(multAngle));

					color.x = 1.005*lerp(droitePt(st, 0.001, pt, multPt), color.x, float(i) / float(i + 1));
					color.y = 1.0015* lerp(droitePt(st, 0.01, pt, multPt), color.y, float(i) / float(i + 1));
					color.z = color.y;
					//color.z = 1.0015*lerp(droitePt(st, 0.02, pt, multPt), color.z, float(i) / float(i + 1));
					//color = max(val,color);
				}

				color *= 15.;




				return half4(color.x, color.y, color.z, 1.);/**insideCircle(half2(0.,0.), st, 0.1);*/

			}
			ENDCG
		}
	}
}
