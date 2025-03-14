//===インクルード部===
#include "Curtain.h"
#include "Sprite.h"
#include "Input.h"
#include "Defines.h"


//===コンストラクタ===
CurtainUI::CurtainUI()
	: m_pTexture(nullptr)
	, m_fPosY(-330.0f)
{

	m_pTexture = new Texture();

	//それぞれの.pngが開けなかった時メッセージボックスを表示する
	if (FAILED(m_pTexture->Create("Assets/Texture/anmaku_usiro.png")))
	{
		MessageBox(NULL, "Curtain.png", "Error", MB_OK);
	}

	m_pStageCurtainTex = new Texture();

	// 画像ファイルが開けなかったら表示
	if (FAILED(m_pStageCurtainTex->Create("Assets/Texture/Stage_Curtain_V2.png")))
	{
		MessageBox(NULL, "StageCurtain.png", "Error", MB_OK);
	}
}

//===デストラクタ===
CurtainUI::~CurtainUI()
{
	if (m_pStageCurtainTex)
	{
		delete m_pStageCurtainTex;
		m_pStageCurtainTex = nullptr;
	}
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

//===更新===
void CurtainUI::Update()
{
	// カウントダウン
	m_fTime -= 1.0f / 60.0f;

	// 緞帳を動かす必要があるか
	if (!IsPlay())
	{
		return;
	}

	// 緞帳の経過時間の割合からY座標を計算
	float rate = m_fTime / m_fTotalTime;

	// 緞帳を下げる場合だけ、Y座標を反転させる
	if (!m_isUp)
	{
		rate = 1.0f - rate;
	}
	//m_fPosY = m_fMinPosY + (m_fMaxPosY - m_fMinPosY) * rate;
	m_fPosY = (1.0f - rate) * m_fMinPosY + rate * m_fMaxPosY;
}

//===描画===
void CurtainUI::LeftDraw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(
		-150.0f, 360.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//プロジェクション行列には2Dとして表示するための行列を設定
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	//スプライトの設定
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetOffset(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(640.0f, -720.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pTexture);
	Sprite::Draw();
}

void CurtainUI::RightDraw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(
		1430.0f, 360.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//プロジェクション行列には2Dとして表示するための行列を設定
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	//スプライトの設定
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetOffset(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(640.0f, -720.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pTexture);
	Sprite::Draw();
}

void CurtainUI::StageCurtainDraw()
{
	DirectX::XMFLOAT4X4 mat[3];

	SetSamplerState(SAMPLER_POINT);
	// ワールド行列はX,Yのみを考慮して作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(
		SCREEN_WIDTH / 2.0f, m_fPosY, 0.0f);// 500.0f 
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	// 単位行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	// プロジェクション行列には2Dとして表示するための行列を設定
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	// スプライトの設定
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	//Sprite::SetOffset(DirectX::XMFLOAT2(0.0f, 25.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(SCREEN_WIDTH*1.4f, -SCREEN_HEIGHT*1.4f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pStageCurtainTex);
	Sprite::Draw();

	SetSamplerState(SAMPLER_LINEAR);
}

/**
 * @brief 緞帳の移動を開始する
 * @param[in] isUp 緞帳を上げるかどうか (true):上げる (false):下げる
 * @param[in] Time 緞帳を移動する時間
*/
void CurtainUI::Start(bool isUp, float Time)
{
	// 緞帳が移動中なら何もしない
	if (IsPlay())
	{
		return;
	}

	m_isUp = isUp;
	m_fTime = Time + 1.0f;
	m_fTotalTime = Time +1.0f;
}

bool CurtainUI::IsPlay()
{
	// 緞帳が移動中かどうか
	return m_fTime > 0.0f;	// 例外スロー箇所
}
