Shader "Custom/RotateTriangle"
{

	// Link tessellation shader : Ned Makes Games https://nedmakesgames.medium.com/mastering-tessellation-shaders-and-their-many-uses-in-unity-9caeb760150e

	Properties
	{
		_MainTex("Texture", 2D) = "white" {}
		_ScriptTime("Time", Float) = 0
		_Shininess("Shininess", Float) = 10 //Shininess
		_SpecColor("Specular Color", Color) = (1, 1, 1, 1) //Specular highlights color
		_Mode("Color : Rainbow = 0  FixedColor = 1 WireFrame = 2", int) = 0
		_Shading("Shading : None = 0  Diffuse = 1 Specular = 2 All = 3", int) = 3
		_FixedColor("FixedColor ", Color) = (1,0,0,1)
		_LerpSkyDiffuse("Lerp between sky and diffuse", Float) = 0.5
		_SkyTex3D("Cubemap   (HDR)", Cube) = "grey" {}
	}
		SubShader
	{
		// No culling or depth
		Cull Back ZWrite On ZTest Always

		Pass
		{
			Tags{ "LightMode" = "ForwardBase" }
			CGPROGRAM


			#pragma target 5.0
			#pragma vertex vert
			#pragma geometry geom
			#pragma fragment frag

			#include "UnityCG.cginc"

			uniform float4 _LightColor0; //From UnityCG

			#define PI 3.14159265359

			uniform sampler2D _MainTex;
			uniform float _ScriptTime;
			uniform float4 _SpecColor;
			uniform float _Shininess;
			uniform int _Mode;
			uniform int _Shading;
			uniform float4 _FixedColor;
			uniform float _LerpSkyDiffuse;
			uniform samplerCUBE _SkyTex3D;


			half4 _Tex_HDR;


			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
				UNITY_VERTEX_INPUT_INSTANCE_ID
			};


			struct v2g
			{
				float3 localSpaceVert : INTERNALTESSPOS;
				UNITY_VERTEX_INPUT_INSTANCE_ID
				UNITY_VERTEX_OUTPUT_STEREO
			};

			struct g2f
			{
				float4 vertex : SV_POSITION;
				float3 barycoord : TEXCOORD2;
			};



			float TriangleArea(float3 v1,float v2, float v3)
			{
				return 0.5f * length(cross(v2 - v1,v3 - v1));
			}

			v2g vert(appdata i)
			{
				v2g o;

				UNITY_SETUP_INSTANCE_ID(i);
				UNITY_TRANSFER_INSTANCE_ID(i, o);


				//half3 v3 = CalculatePtPos(i.vertex.xyz);

				//o.vertex = UnityObjectToClipPos(float4(v3,1.));
				//o.localSpaceVert = float3(i.vertex.x,i.vertex.y,i.vertex.z);

				o.localSpaceVert = i.vertex.xyz;

				return o;
			}


			[maxvertexcount(3)]
			void geom(triangle v2g input[3], inout TriangleStream<g2f> triStream)
			{
				g2f o;

				for (int i = 0; i < 3; i++)
				{
					o.vertex = UnityObjectToClipPos(input[i].localSpaceVert);
					o.barycoord = float3(0, 0, 0);
					o.barycoord[i] = 1;
					triStream.Append(o);
				}
			}


			fixed4 frag(g2f f) : SV_Target{
			
				half minVal = min(f.barycoord.x, min(f.barycoord.y, f.barycoord.z));
				half3 color = lerp(half3(1., 1., 1), half3(0., 0., 0.), minVal / 0.1);
				fixed4 col = half4(color, 1.0);
				return col;
			}





			ENDCG
		}
	}
}
