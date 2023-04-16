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
		_Coeff("Coeff",Float) = 0
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
			uniform float _Coeff;


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
				float3 color : TEXCOORD3;
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


			[maxvertexcount(7)]
			void geom(triangle v2g input[3], inout TriangleStream<g2f> triStream)
			{
				g2f o;


				float3 v1 = input[0].localSpaceVert;
				float3 v2 = input[1].localSpaceVert;
				float3 v3 = input[2].localSpaceVert;

				float coeff = _Coeff;
				//float coeff = 0.5;

				float3 v12 = lerp(v1, v2, coeff);
				float3 v23 = lerp(v2, v3, coeff);
				float3 v31 = lerp(v3, v1, coeff);


				float3 color1 = float3(1, 1, 1);
				float3 color2 = float3(0, 1, 0);
				float3 color3 = float3(0, 0, 1);


				//triangle central
				o.vertex = UnityObjectToClipPos(v12);
				o.barycoord = float3(0, 0, 0);
				o.barycoord[0] = 1;
				o.color = color1;
				triStream.Append(o);

				o.vertex = UnityObjectToClipPos(v23);
				o.barycoord = float3(0, 0, 0);
				o.barycoord[1] = 1;
				o.color = color1;
				triStream.Append(o);

				o.vertex = UnityObjectToClipPos(v31);
				o.barycoord = float3(0, 0, 0);
				o.barycoord[2] = 1;
				o.color = color1;
				triStream.Append(o);


				//triangle bas gauche
				/*o.vertex = UnityObjectToClipPos(v1);
				o.barycoord = float3(0, 0, 0);
				o.barycoord[0] = 1;
				o.color = color2;
				triStream.Append(o);

				o.vertex = UnityObjectToClipPos(v12);
				o.barycoord = float3(0, 0, 0);
				o.barycoord[1] = 1;
				o.color = color2;
				triStream.Append(o);*/

				/*o.vertex = UnityObjectToClipPos(v23);
				o.barycoord = float3(0, 0, 0);
				o.barycoord[2] = 1;
				o.color = color3;
				triStream.Append(o);*/


				//triangle bas droit
				/*o.vertex = UnityObjectToClipPos(v31);
				o.barycoord = float3(0, 0, 0);
				o.barycoord[0] = 1;
				o.color = color2;
				triStream.Append(o);

				o.vertex = UnityObjectToClipPos(v23);
				o.barycoord = float3(0, 0, 0);
				o.barycoord[1] = 1;
				o.color = color2;
				triStream.Append(o);

				o.vertex = UnityObjectToClipPos(v3);
				o.barycoord = float3(0, 0, 0);
				o.barycoord[2] = 1;
				o.color = color2;
				triStream.Append(o);*/


				//triangle haut
				/*o.vertex = UnityObjectToClipPos(v12);
				o.barycoord = float3(0, 0, 0);
				o.barycoord[0] = 1;
				o.color = color2;
				triStream.Append(o);

				o.vertex = UnityObjectToClipPos(v2);
				o.barycoord = float3(0, 0, 0);
				o.barycoord[1] = 1;
				o.color = color2;
				triStream.Append(o);

				o.vertex = UnityObjectToClipPos(v23);
				o.barycoord = float3(0, 0, 0);
				o.barycoord[2] = 1;
				o.color = color2;
				triStream.Append(o);*/




			}


			fixed4 frag(g2f f) : SV_Target{
			
				half minVal = min(f.barycoord.x, min(f.barycoord.y, f.barycoord.z));
				//half3 color = lerp(half3(0., 0., 0.),f.color, minVal / 0.1);
				
			
				fixed4 col = half4(f.color, 1.0);
				return col;
			}





			ENDCG
		}
	}
}
