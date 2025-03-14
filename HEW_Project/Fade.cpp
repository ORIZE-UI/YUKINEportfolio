#include "Fade.hpp"
#include <DirectXMath.h>
#include "Defines.h"
#include "Sprite.h"

Fade::Fade(CurtainUI* curtain)
	: m_alpha(0.0f)
	, m_time(0.0f)
	, m_totalTime(0.0f)
	, m_pCurtain(curtain)
	, m_isIn(false)
	, m_pTex(nullptr)
	, m_pPS(nullptr)
{
	m_pTex = new Texture();
	if (FAILED(m_pTex->Create("./Assets/Texture/Black.png")))
	{
		MessageBox(NULL, "[Fade.cpp] Failed to Load Texture", "Error", MB_OK);
		return;
	}
	m_pPS = new PixelShader();
	if (FAILED(m_pPS->Load("Assets/Shader/PS_Sprite.cso")))
	{
		MessageBox(nullptr, "PS_Sprite.cso", "Error", MB_OK);
		return;
	}
	
}

Fade::~Fade()
{
	//if (m_pCurtain)
	//{
	//	delete m_pCurtain;
	//	m_pCurtain = nullptr;
	//}
	if (m_pPS)
	{
		delete m_pPS;
		m_pPS = nullptr;
	}
	if (m_pTex)
	{
		delete m_pTex;
		m_pTex = nullptr;
	}
}

void Fade::Update()
{
	// カウントダウン
	m_time -= 1.0f / 30.0f;
	// フェードを実行する必要があるか
	if (!IsPlay())
	{
		return;
	}
	// フェードの経過時間の割合から透明度を計算
	float rate = m_time / m_totalTime;
	// フェードアウトの場合だけ、透明度を反転させる
	if (!m_isIn)
	{
		rate = 1.0f - rate;
	}
	// 透明度を設定
	// lerp(開始値, 終了値, 割合)
	if (m_isIn == false)
	{
		m_alpha = rate;
	}
	else
	{
		// フェードアウト
		rate =  1.0f * rate; // cosf((rate * DirectX::XM_PI) / 2.0f);
	}
	m_alpha = rate;

	//if ((GetAlpha() >= 0.5f && GetAlpha() <= 0.6f)&& m_isIn == false)
	//{
	//	m_pCurtain->Start(false, m_time + 0.5f);
	//}
	//if ((GetAlpha() >= 0.9f && GetAlpha() <= 1.0f) && m_isIn == true)
	//{
	//	m_pCurtain->Start(true, m_time - 0.4f);
	//}
}

void Fade::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	if (0.5f < m_alpha && m_alpha < 0.6f)
	{
		int a = 0 ;;

	}

	// ワールド行列は X,Y のみを考慮して作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(730.0f, 418.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	// ビュー行列は2Dだと、カメラの位置があまり関係ないので単位行列
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	// プロジェクション行列は、画面のサイズに合わせて作成
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f
	);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	float param[4] = { m_alpha, 0.0f, 0.0f, 0.0f };
	//m_pVS->Bind();
	//m_pPS->Bind();
	//m_pVS->WriteBuffer(0, &m_uv);
	//m_pVS->WriteBuffer(1, &m_transform);
	//m_pPS->WriteBuffer(0, param);
	//DrawVertexBuffer(m_pVtx, 4);

	// スプライトの設定
	Sprite::SetPixelShader(m_pPS);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetTexture(m_pTex);
	Sprite::SetOffset({ 0.0f, 0.0f });
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, m_alpha });
	Sprite::SetSize(DirectX::XMFLOAT2(SCREEN_WIDTH, -SCREEN_HEIGHT - 30.0f));

	// スプライトの描画
	Sprite::Draw();

#if 0
	FILE* fp;
	fp = fopen("check.txt", "at");
	fprintf(fp, "%.2f\n", m_alpha);
	fclose(fp);

#endif



}

/**
 * @brief フェードを開始する
 * @param[in] isIn フェードインにするか (true):フェードイン (false):フェードアウト
 * @param[in] time フェードにかける時間
*/
void Fade::Start(bool isIn, float time)
{
	// フェード再生中は設定できない
	if (IsPlay())
	{
		return;
	}
	m_isIn = isIn;
	m_time = time;
	m_totalTime = time;

}

bool Fade::IsPlay()
{
	// フェードの残り時間があるか
	return m_time > 0.0f;
}

float Fade::GetAlpha()
{
	return m_alpha;
}
