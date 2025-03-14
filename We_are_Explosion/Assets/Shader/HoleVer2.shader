Shader "Custom/MaskedHoleShader"
{
	Properties
	{
		_MainTex("Background Texture", 2D) = "white" {}
		_MaskTex("Mask Texture", 2D) = "white" {}
		_HoleCenter("Hole Center", Vector) = (0.5, 0.5, 0, 0)
		_HoleSize("Hole Size", Float) = 0.1
	}
		SubShader
		{
			Tags { "Queue" = "Transparent" }
			Pass
			{
				CGPROGRAM
				#pragma vertex vert
				#pragma fragment frag
				#include "UnityCG.cginc"

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

				sampler2D _MainTex;
				sampler2D _MaskTex;
				float4 _HoleCenter;
				float _HoleSize;

				v2f vert(appdata v)
				{
					v2f o;
					o.vertex = UnityObjectToClipPos(v.vertex);
					o.uv = v.uv;
					return o;
				}

				fixed4 frag(v2f i) : SV_Target
				{
					float2 uv = i.uv;
					fixed4 col = tex2D(_MainTex, uv);

					// マスクテクスチャのUV座標を計算
					float2 maskUV = (uv - _HoleCenter.xy) / _HoleSize + 0.5;

					// マスクテクスチャから値を取得
					fixed4 mask = tex2D(_MaskTex, maskUV);

					// マスクが白い部分を透明にする
					if (mask.r > 0.5)
					{
						discard;
					}

					return col;
				}
				ENDCG
			}
		}
			FallBack "Unlit/Transparent"
	
}
