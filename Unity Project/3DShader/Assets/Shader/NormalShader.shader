Shader "Custom/NormalShader" {
	Properties{
		_MainTex("Texture", 2D) = "white" {}
	_Amount("Extrusion Amount", Range(-1,1)) = 0.5
	}
		SubShader{
		Tags{ "RenderType" = "Opaque" }
		CGPROGRAM
#pragma surface surf Lambert vertex:vert
		struct Input {
		float2 uv_MainTex;
		float3 worldPos;
	};
	float _Amount;
	void vert(inout appdata_full v) {
		v.vertex.xyz += v.normal * max(cos(_Time+2.*v.vertex.y),0.);
	}
	sampler2D _MainTex;
	void surf(Input IN, inout SurfaceOutput o) {
		//o.Albedo = tex2D(_MainTex, IN.uv_MainTex).rgb;
		float a  = max(cos(3.*_Time + 2.*IN.worldPos.y), 0.);
		//a = 1.;
		o.Albedo = float3(a, a, a);
	}
	ENDCG
	}
		Fallback "Diffuse"
}