Shader "Mine/OndeSphereShader"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
		_ScriptTime("Time", Float) = 0
		_AmplitudeDeformation("AmplitudeDeformation", Float) = 1
		_AmplitudeColor("AmplitudeColor", Float) = 1
		_FreqTemporal("FreqTemporal", Float) = 10
		_FreqSpatial("FreqSpatial", Float) = 10
		_nbSource("nbSource", int) = 10
		_Center("Center", Vector) = (1,1,1,1)
		_Shift("Shift", Vector) = (1,2,4,1)
		_Shininess("Shininess", Float) = 10 //Shininess
		_SpecColor("Specular Color", Color) = (1, 1, 1, 1) //Specular highlights color
		_Mode("Rainbow = 0  FixedColor = 1 WireFrame = 2", int) = 0
		_FixedColor("FixedColor ", Color) = (1,0,0,1)
	}
	SubShader
	{
		// No culling or depth
		//Cull Back ZWrite On ZTest Always

		Pass
		{
			Tags{ "LightMode" = "ForwardBase" }
			CGPROGRAM


			#pragma vertex vert
			#pragma geometry geom
			#pragma fragment frag
			
			#include "UnityCG.cginc"

			uniform float4 _LightColor0; //From UnityCG
			
			#define PI 3.14159265359

			uniform sampler2D _MainTex;
			uniform float _ScriptTime;
			uniform float _AmplitudeDeformation;
			uniform float _AmplitudeColor;
			uniform float _FreqTemporal;
			uniform float _FreqSpatial;
			uniform int _nbSource;
			uniform float4 _Center;
			uniform float4 _Shift;
			uniform float4 _SpecColor;
			uniform float _Shininess;
			uniform int _Mode;
			uniform float4 _Sources[20];
			uniform float4 _FixedColor;


			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
			};

			struct v2g
			{
				float2 uv : TEXCOORD0;
				float4 vertex : SV_POSITION;
				float3 localSpaceVert : TEXCOORD1;
			};

			struct g2f
			{
				float2 uv : TEXCOORD0;
				float4 vertex : SV_POSITION;
				float3 localSpaceVert : TEXCOORD1;
				float3 barycoord : TEXCOORD2;
			};


			half ondula(half3 st, half3 center, half shift, half period)
			{
				half r = dot(normalize(st), normalize(center));
				half ondula = (1. + cos(period*r + _FreqTemporal * _ScriptTime)) / 2.;
				return ondula;
			}

			half3 RGBondula(half3 st, half3 center, half3 RGBshift)
			{
				float period = _FreqSpatial;
				float ondeR = ondula(st, center, RGBshift.x, period);
				float ondeG = ondula(st, center, RGBshift.y, 2.*period);
				float ondeB = ondula(st, center, RGBshift.z, 3.*period);

				return half3(ondeR, ondeG, ondeB);

			}


			half3 RGBondula(half3 pos)
			{
				half3 color = half3(0., 0., 0.);
				for (int i = 0; i < _nbSource; i++)
				{
					color += RGBondula(pos, _Sources[i].xyz, _Shift.xyz);
				}
				color /= half(_Sources.Length);
				return color;
			}


			half3 CalculatePtPos(half3 pos)
			{
				half3 p = normalize(pos);
				half3 color = RGBondula(p);
				//color = RGBondula(pos, _Center.xyz, _Shift.xyz);
				float offset = _AmplitudeDeformation*(color.x + 0.3*color.y + 0.1*color.z)*5. / 6.*0.1;
				float3 v3 = (1. + offset) * pos;
				return v3;
			}

			half3 ComputeNormal(half3 p)
			{
				half eps = 0.001f;
				half3 v1 = half3(-p.y, p.x, 0.);
				//if (abs(p.x) < eps) 
				//	v1 = half3(0., -p.z, p.y);

				float4 right_cam = mul(UNITY_MATRIX_T_MV,float4(1, 0, 0, 0));
				v1 = normalize(cross(right_cam.xyz, p));

				half3 v2 = normalize(cross(v1, p));
				
			
				half3 p0 = CalculatePtPos(p);
				half3 posv1 = CalculatePtPos(p + eps * v1);
				half3 posv2 = CalculatePtPos(p + eps * v2);
				half3 n = cross(posv1 - p0, posv2 - p0);
				if (dot(n, p) < 0)
					n = -n;
				return n;
				
			}

			v2g vert (appdata v)
			{
				v2g o;

				half3 v3 = CalculatePtPos(v.vertex.xyz);
				
				o.vertex = UnityObjectToClipPos(float4(v3,1.));
				o.localSpaceVert = float3(v.vertex.x,v.vertex.y,v.vertex.z);
				return o;
			}
			
			[maxvertexcount(3)]
			void geom(triangle v2g input[3], inout TriangleStream<g2f> triStream)
			{
				g2f o;
				float3 normal = normalize(cross(input[1].vertex - input[0].vertex, input[2].vertex - input[0].vertex));

				for (int i = 0; i < 3; i++)
				{
					o.uv = input[i].uv;
					o.vertex = input[i].vertex;
					o.localSpaceVert = input[i].localSpaceVert;
					o.barycoord = float3(0, 0, 0);
					o.barycoord[i] = 1;
					triStream.Append(o);
				}
			}


			fixed4 frag(g2f f) : SV_Target{
				fixed4 col = tex2D(_MainTex, f.uv);
				half3 v = half3(f.localSpaceVert.x, f.localSpaceVert.y, f.localSpaceVert.z);
				half3 waveColor = _AmplitudeColor*RGBondula(v);
				half minVal = min(f.barycoord.x, min(f.barycoord.y, f.barycoord.z));
				half3 otherColor = fmod(waveColor + half3(0.5, 0.5, 0.5), half3(1., 1., 1.));
				if (_Mode == 1)
				{
					waveColor = _FixedColor.xyz;
				}
				else if(_Mode == 2)
				{
					waveColor = lerp(half3(1., 1., 1), half3(0., 0., 0.), minVal / 0.1);
				}
				
			
				half3 posWorld =normalize(mul(f.localSpaceVert, unity_WorldToObject).xyz);
				half3 viewDirection = normalize(_WorldSpaceCameraPos - posWorld.xyz);

				half3 n = ComputeNormal(v);
				half3 normalDirection = normalize(mul(float4(n, 0.0), unity_WorldToObject).xyz);

				half val = (1.+dot(normalize(n),half3(1.,0.,0.)))/2.0;
				
				
				
				float3 vert2LightSource = _WorldSpaceLightPos0.xyz - posWorld.xyz;
				float oneOverDistance = 1.0 / length(vert2LightSource);
				//float attenuation = lerp(1.0, oneOverDistance, _WorldSpaceLightPos0.w); //Optimization for spot lights. This isn't needed if you're just getting started.
				float attenuation = 1.0;
				float3 lightDirection = _WorldSpaceLightPos0.xyz - posWorld.xyz * _WorldSpaceLightPos0.w;

				float3 diffuseReflection = attenuation * _LightColor0.rgb * max(0.0, dot(normalDirection, lightDirection)); //Diffuse component
				float3 specularReflection;
				if (dot(normalDirection, lightDirection) < 0.0) //Light on the wrong side - no specular
				{
					specularReflection = float3(0.0, 0.0, 0.0);
				}
				else
				{
					//Specular component
					specularReflection = attenuation * _LightColor0.rgb * _SpecColor.rgb * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), _Shininess);
				}

				float3 color = (diffuseReflection)* waveColor  + specularReflection; //No ambient component this time
				
				col = half4(color, 1.0);
				return col;
			}




			
			ENDCG
		}
	}
}
