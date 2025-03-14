//===インクルード部===
#include "CoinCntUI.h"
#include "Sprite.h"
#include "Input.h"


//===コンストラクタ===
CoinCntUI::CoinCntUI() :m_pTexture(nullptr)
{
	m_pTexture = new Texture();
	//それぞれの.pngが開けなかった時メッセージボックスを表示する
	if (FAILED(m_pTexture->Create("Assets/Texture/coin_waku.png")))
	{
		MessageBox(NULL, "コインの枠", "Error", MB_OK);
	}
}

//===デストラクタ===
CoinCntUI::~CoinCntUI()
{
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

//===更新===
void CoinCntUI::Update()
{

}

//===描画===
void CoinCntUI::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(
		180.0f, 80.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//プロジェクション行列には2Dとして表示するための行列を設定
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f, 720.0f, 0.0f, 0.5f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	//スプライトの設定
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	//Sprite::SetOffset(DirectX::XMFLOAT2(30.0f, 30.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(300.0f, -75.0f));
	Sprite::SetTexture(m_pTexture);
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 2.0f });
	Sprite::Draw();
}
