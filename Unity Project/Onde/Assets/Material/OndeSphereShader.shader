Shader "Mine/OndeSphereShader"
{

	// Link tessellation shader : Ned Makes Games https://nedmakesgames.medium.com/mastering-tessellation-shaders-and-their-many-uses-in-unity-9caeb760150e

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
		_Mode("Color : Rainbow = 0  FixedColor = 1 WireFrame = 2", int) = 0
		_Shading("Shading : None = 0  Diffuse = 1 Specular = 2 All = 3", int) = 3
		_Tessellate("Tessellate : No = 0  Yes = 1 ", int) = 1
		_FactorTessellate("Factor Tessellate ", float) = 1.0
		_FixedColor("FixedColor ", Color) = (1,0,0,1)
		_LerpSkyDiffuse("Lerp between sky and diffuse", Float) = 0.5
		_SkyTex3D("Cubemap   (HDR)", Cube) = "grey" {}
	}
	SubShader
	{
		// No culling or depth
		//Cull Back ZWrite On ZTest Always

		Pass
		{
			Tags{ "LightMode" = "ForwardBase" }
			CGPROGRAM

			
			#pragma target 5.0
			#pragma vertex vert
			#pragma hull hul
			#pragma domain dom
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
			uniform int _Shading;
			uniform int _Tessellate;
			uniform float _FactorTessellate;
			uniform float4 _Sources[20];
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

			
			struct v2h
			{
				float3 localSpaceVert : INTERNALTESSPOS;
				UNITY_VERTEX_INPUT_INSTANCE_ID
			};


			struct d2g
			{
				float4 vertex : SV_POSITION;
				float3 localSpaceVert : TEXCOORD1;
				UNITY_VERTEX_INPUT_INSTANCE_ID
				UNITY_VERTEX_OUTPUT_STEREO
			};

			struct g2f
			{
				float4 vertex : SV_POSITION;
				float3 localSpaceVert : TEXCOORD1;
				float3 barycoord : TEXCOORD2;
			};

			struct TessellationFactors 
			{
				float edge[3] : SV_TessFactor;
				float inside : SV_InsideTessFactor;
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

			float TriangleArea(float3 v1,float v2, float v3)
			{
				return 0.5f * length(cross(v2 - v1,v3 - v1));
			}

			v2h vert (appdata i)
			{
				v2h o;

				UNITY_SETUP_INSTANCE_ID(i);
				UNITY_TRANSFER_INSTANCE_ID(i, o);


				//half3 v3 = CalculatePtPos(i.vertex.xyz);
				
				//o.vertex = UnityObjectToClipPos(float4(v3,1.));
				//o.localSpaceVert = float3(i.vertex.x,i.vertex.y,i.vertex.z);

				o.localSpaceVert = i.vertex.xyz;

				return o;
			}


			// The hull function runs once per vertex. You can use it to modify vertex
			// data based on values in the entire triangle
			[domain("tri")] // Signal we're inputting triangles
			[outputcontrolpoints(3)] // Triangles have three points
			[outputtopology("triangle_cw")] // Signal we're outputting triangles
			[patchconstantfunc("PatchConstantFunction")] // Register the patch constant function
			[partitioning("integer")] // Select a partitioning mode: integer, fractional_odd, fractional_even or pow2
			v2h hul(
				InputPatch<v2h, 3> patch, // Input triangle
				uint id : SV_OutputControlPointID) 
			{ // Vertex index on the triangle

				return patch[id];
			}

			// The patch constant function runs once per triangle, or "patch"
			// It runs in parallel to the hull function
			TessellationFactors PatchConstantFunction(
				InputPatch<v2h, 3> patch) 
			{
				UNITY_SETUP_INSTANCE_ID(patch[0]); // Set up instancing
				// Calculate tessellation factors
				TessellationFactors f;

				float area_tri = TriangleArea(patch[0].localSpaceVert,
											  patch[1].localSpaceVert,
											  patch[2].localSpaceVert);
				


				float3 v0 = CalculatePtPos(patch[0].localSpaceVert);
				float3 v1 = CalculatePtPos(patch[1].localSpaceVert);
				float3 v2 = CalculatePtPos(patch[2].localSpaceVert);


				float area_tri_deform = TriangleArea(v0,v1,v2);
				if (_Tessellate == 1)
				{
					f.edge[0] = 1.0 + _FactorTessellate*abs(length(v2-v1));
					f.edge[1] = 1.0 + _FactorTessellate*abs(length(v2-v0));
					f.edge[2] = 1.0 + _FactorTessellate*abs(length(v0-v1));
					f.inside = max(1.0,_FactorTessellate*area_tri_deform/area_tri);

					/*f.edge[0] = 1.0 +abs(length(v2-v1));
					f.edge[1] = 1.0 +abs(length(v2-v0));
					f.edge[2] = 1.0 +abs(length(v0-v1));
					f.inside = area_tri_deform/area_tri;*/
				}else
				{
					f.edge[0] = 1.0;
					f.edge[1] = 1.0;
					f.edge[2] = 1.0;
					f.inside = 1.0;
				}
				return f;
			}


		





			// Call this macro to interpolate between a triangle patch, passing the field name
			#define BARYCENTRIC_INTERPOLATE(fieldName) \
				patch[0].fieldName * barycentricCoordinates.x + \
				patch[1].fieldName * barycentricCoordinates.y + \
				patch[2].fieldName * barycentricCoordinates.z
			




		// The domain function runs once per vertex in the final, tessellated mesh
			// Use it to reposition vertices and prepare for the fragment stage
			[domain("tri")] // Signal we're inputting triangles
			d2g dom(
				TessellationFactors factors, // The output of the patch constant function
				OutputPatch<v2h, 3> patch, // The Input triangle
				float3 barycentricCoordinates : SV_DomainLocation) { // The barycentric coordinates of the vertex on the triangle

				d2g o;

				// Setup instancing and stereo support (for VR)
				UNITY_SETUP_INSTANCE_ID(patch[0]);
				UNITY_TRANSFER_INSTANCE_ID(patch[0], o);
				UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(o);

				float3 positionOS = BARYCENTRIC_INTERPOLATE(localSpaceVert);


				
				half3 v3 = CalculatePtPos(positionOS.xyz);
				
				o.vertex = UnityObjectToClipPos(float4(v3,1.));
				o.localSpaceVert = positionOS;

				return o;
			}



			
			[maxvertexcount(3)]
			void geom(triangle d2g input[3], inout TriangleStream<g2f> triStream)
			{
				g2f o;
				float3 normal = normalize(cross(input[1].vertex - input[0].vertex, input[2].vertex - input[0].vertex));

				for (int i = 0; i < 3; i++)
				{
					o.vertex = input[i].vertex;
					o.localSpaceVert = input[i].localSpaceVert;
					o.barycoord = float3(0, 0, 0);
					o.barycoord[i] = 1;
					triStream.Append(o);
				}
			}


			half3 SkyReflection(half3 normal, half3 viewDir)
			{
				half3 texCoord = normalize(reflect(-viewDir, normal));
				half4 tex = texCUBE(_SkyTex3D, texCoord); 
				//DecodeHDR semble pas fonctionner, peut être pas une cubemap hdr
				//half3 c = DecodeHDR(tex, _Tex_HDR);

				return tex.xyz;

			}

			fixed4 frag(g2f f) : SV_Target{
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

				if((_Shading == 0)||(_Shading == 1))
				{
					specularReflection = 0.0;
				}

				if((_Shading == 0)||(_Shading == 2))
				{
					diffuseReflection = 1.0;
				}




				half3 sky_color = SkyReflection(normalDirection, viewDirection);
				float3 color = (diffuseReflection)* lerp(waveColor,sky_color,_LerpSkyDiffuse)  + specularReflection; //No ambient component this time
				
				

				fixed4 col = half4(color, 1.0);
				return col;
			}




			
			ENDCG
		}
	}
}
