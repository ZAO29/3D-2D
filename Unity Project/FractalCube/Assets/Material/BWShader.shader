Shader "Custom/BWShader" {
    Properties{
        _MainTex("Texture", 2D) = "white" {}
        _Color("Color", Color) = (1,1,1,1)
        _EdgeWidth("Edge Width", Range(0.0, 1.0)) = 0.1
    }

        SubShader{
            Tags {"Queue" = "Transparent" "RenderType" = "Opaque"}
            LOD 100

            Pass {
                CGPROGRAM
                #pragma vertex vert
                #pragma fragment frag
                #include "UnityCG.cginc"

                struct appdata {
                    float4 vertex : POSITION;
                    float2 uv : TEXCOORD0;
                };

                struct v2f {
                    float2 uv : TEXCOORD0;
                    float4 vertex : SV_POSITION;
                };

                sampler2D _MainTex;
                float4 _Color;
                float _EdgeWidth;

                v2f vert(appdata v) {
                    v2f o;
                    o.vertex = UnityObjectToClipPos(v.vertex);
                    o.uv = v.uv;
                    return o;
                }

                fixed4 frag(v2f i) : SV_Target {
                    float4 color = tex2D(_MainTex, i.uv);
                    float edge = step(i.uv.x,_EdgeWidth) + 
                                 step(1-i.uv.x, _EdgeWidth) +
                                 step(i.uv.y, _EdgeWidth) + 
                                 step(1 - i.uv.y, _EdgeWidth);
                                
                    return color * _Color * (step(edge,0.5)) + (1 - step(edge,0.5)) * float4(1,0,0,1);
                }
                ENDCG
            }
        }

            FallBack "Diffuse"
}
