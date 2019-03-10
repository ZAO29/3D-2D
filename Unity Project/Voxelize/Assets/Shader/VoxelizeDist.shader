Shader "Custom/VoxelizeDist"
{

	Properties
	{
		_X("X", float) = 2.
		_Y("Y", float) = 2.
		_Z("Z", float) = 2.
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
	};

	float _X;
	float _Y;
	float _Z;

	// vertex shader: takes object space normal as input too
	v2f vert(float4 vertex : POSITION, float3 normal : NORMAL)
	{
		v2f o;
		float4 worldPos = UnityObjectToClipPos(vertex);
		float3 PosCam = float3(_X, _Y, _Z);
		float PosWorld = worldPos.xyz / worldPos.w;
		float SpatialRes = length(PosCam - PosWorld);
		vertex *= SpatialRes;
		vertex = floor(vertex);
		vertex /= SpatialRes;
		o.pos = UnityObjectToClipPos(vertex);
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
	// and put into red, green, blue components
	c.rgb = i.worldNormal*0.5 + 0.5;
	float3 PosCam = float3(_X, _Y, _Z);
	float3 PosReal = i.pos.xyz / i.pos.w;
	float SpatialRes = length(PosCam - PosReal);
	float w = 0.7;
	c.rgb = c.rgb * w  + ( 1+cos(SpatialRes%100)/2) * (1 -w);
	return c;
	}
		ENDCG
	}
	}
}

