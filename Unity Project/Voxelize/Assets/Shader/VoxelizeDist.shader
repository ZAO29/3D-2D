Shader "Custom/VoxelizeDist"
{

	Properties
	{
		_Position("Position", Vector) = (.0, .0, .0)
		_W("weight", Float) = 0.5
		_Freq("Freq", Float) = 1.
	}
		// no Properties block this time!
		SubShader
	{
		Pass
	{
		CGPROGRAM
#pragma vertex vert
#pragma fragment frag
		// include file that contains UnityObjectToWorldNormal helper function
#include "UnityCG.cginc"

		struct v2f {
		// we'll output world space normal as one of regular ("texcoord") interpolators
		half3 worldNormal : TEXCOORD0;
		float4 pos : SV_POSITION;
		float dist : DIST;
	};

	float3 _Position;
	float _W;
	float _Freq;


	// vertex shader: takes object space normal as input too
	v2f vert(float4 vertex : POSITION, float3 normal : NORMAL)
	{
		v2f o;
		o.dist = distance(mul(unity_ObjectToWorld, vertex), _Position);;
		float SpatialRes = o.dist;
		vertex.z *= SpatialRes;
		vertex.z = floor(vertex.z);
		vertex.z /= SpatialRes;
		o.pos = UnityObjectToClipPos(vertex);
		//o.pos.y += cos(5*SpatialRes)*1.1;
		// UnityCG.cginc file contains function to transform
		// normal from object to world space, use that
		o.worldNormal = UnityObjectToWorldNormal(normal);
		return o;
	}

	fixed4 frag(v2f i) : SV_Target
	{
		fixed4 c = 0;
	// normal is a 3D vector with xyz components; in -1..1
	// range. To display it as color, bring the range into 0..1
	// and put into red, green, blue components = length(PosCam - PosReal);
	float3 vec = (1 + cos(i.dist *_Freq+_Time) / 2) * float3(1., 1., 1.);
	c.rgb = (i.worldNormal*0.5 + 0.5) * _W  +  vec * (1 -_W);
	return c;
	}
		ENDCG
	}
	}
}

