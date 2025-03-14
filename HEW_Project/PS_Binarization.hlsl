#pragma warning(disable:3570)

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);

#define FILTER_SIZE 5
#define SIGMA_D 3.0f
#define SIGMA_R 0.1f

float4 main(PS_IN pin) : SV_TARGET
{
	//float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//// グレースケール変換
	////float3 gray = color.r * 0.299f + color.g * 0.587f + color.b * 0.114f;
	//float3 gray = dot(color.rgb, float3(0.299f, 0.587f, 0.114f));
	//color = float4(gray.r, gray.g, gray.b, 1.0f);
	
	// テクスチャから色を取得
	float4 color = tex.Sample(samp, pin.uv);
	
	if (all(color.rgba == float4(1.0f, 1.0f, 1.0f, 1.0f)))
	{
		return float4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	if (color.a == 0.0f)
	{
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	if (color.g == 1.0f)
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (0.05f <= color.g && color.g <= 0.1f || 0.0f <= color.b && color.b <= 0.05f)
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	} // 自動ブロック(赤)

	if (0.5f <= color.r && color.r <= 0.55f && 0.5f <= color.g && color.g <= 0.55f || 0.5f <= color.b && color.b <= 0.55f)
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}// 床
	if (0.26f <= color.r && color.r <= 0.42f || 0.26f <= color.g && color.g <= 0.42f || 0.26 <= color.b && color.b <= 0.42f)
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}// リフト




	//if (color.r == 1.0f && color.g == 1.0f && color.b == 1.0f)
	//{
	//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
	//}
	
	//if (color.r == 0.0f && color.g == 0.0f && color.b == 0.0f)
	//{
	//	return float4(0.0f, 0.0f, 0.0f, 1.0f);
	//}
	
	if (0.95f <= color.r && color.r <= 1.0f || 0.22f <= color.g && color.g <= 0.25f || 0.95f <= color.b && color.b <= 1.0f)
	{
		return color;
	}//コインのテカリ
	
	if (0.50f <= color.r && color.r <= 0.55f || 0.75f <= color.g && color.g <= 0.80f || 0.75f <= color.b && color.b <= 0.80f)
	{
		return color;
	}//ゴール
	
	if (0.31f <= color.r && color.r <= 0.35f || 0.0f <= color.g && color.g <= 0.0f || 0.25 <= color.b && color.b <= 0.30f)
	{
		return color;
	}//コイン
	

	

	//// RGB値を平均化
	float gray = (color.r + color.g + color.b) / 3.0f;
	////color = float4(gray, gray, gray, 1.0f);
	
	// しきい値を設定
	float threshold = 0.0f;
	
	//// 2値化
	float4 binaryColor = (gray >= threshold) ? float4(0.0f, 0.0f, 0.0f, 1.0f) : float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	//return gray;
	
	//return binaryColor;
	
	// 1/テクスチャのサイズ
	uint width, height;
	tex.GetDimensions(width, height);
	float2 offset = 1.0f / float2(width, height);
	
	// 入力画像の中心ピクセルの色
	float4 center = tex.Sample(samp, pin.uv);
	
	// フィルターの重みの合計
	float weightSum = 0.0f;
	
	// 出力色
	float4 output = 0.0f;
	

	
	// フィルター処理
	float4 neighbor = 0.0f;
	for (int i = -FILTER_SIZE; i <= FILTER_SIZE; i++)
	{
		for (int j = -FILTER_SIZE; j <= FILTER_SIZE; j++)
		{
			// フィルタ内のピクセルの座標
			float2 offset = float2(i, j) / float2(width, height);
			
			// フィルタ内のピクセルの色
			// 前回のサンプリングと比較して、色が変わっていなければ、再度サンプリングを行わない
			if (all(neighbor == center))
			{
				neighbor = tex.Sample(samp, pin.uv + offset);
			}
			
			// 距離に基づく重み(空間的な距離)
			float weightD = exp(-0.5f * (i * i + j * j) / (SIGMA_D * SIGMA_D));
			
			// 色差に基づく重み(輝度値の差)
			float weightR = exp(-0.5f * dot(neighbor - center, neighbor - center) / (SIGMA_R * SIGMA_R));
			
			// 重みの積
			float weight = weightD * weightR;
			
			// 重みの合計に加算
			weightSum += weight;
			
			// 出力色に加算
			output += weight * neighbor;

		}

	}
	
	// 重みの合計で割って正規化
	output /= weightSum;
	
	// グレースケール
	float4 grayScale = dot(output.rgb, float3(0.299f, 0.587f, 0.114f));
	
	
	// 2値化
	binaryColor = (grayScale >= threshold) ? float4(0.0f, 0.0f, 0.0f, 1.0f) : float4(0.0f, 0.0f, 0.0f, 1.0f);


	
	return binaryColor;
	//return grayScale;
}