Shader "Custom/StarShapeHoleShader"
{
	Properties
	{
		_MainTex("Background Texture", 2D) = "white" {}
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

					float2 diff = uv - _HoleCenter.xy;
					float angle45 = radians(45.0);
					float2x2 rotationMatrix = float2x2(cos(angle45), -sin(angle45), sin(angle45), cos(angle45));
					float2 rotatedDiff = mul(rotationMatrix, diff);

					bool isInSquare = abs(diff.x) < _HoleSize && abs(diff.y) < _HoleSize;
					bool isInRotatedSquare = abs(rotatedDiff.x) < _HoleSize && abs(rotatedDiff.y) < _HoleSize;

					if (isInSquare || isInRotatedSquare)
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
