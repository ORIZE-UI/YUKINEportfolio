#include "itemUI.h"
#include "Defines.h"
#include "Sprite.h"
#include "DirectX.h"

ItemUI::ItemUI(void)
	: m_pStarTexture(nullptr)
	, m_starPos{ 130.0f, 50.0f, 10.0f }
	, m_starScale{ 0.7f, 0.7f, 1.0f }
	, m_starRotate{ 0.0f, 0.0f, 0.0f }
{
	// テクスチャ読込
	m_pStarTexture = new Texture();
	m_pStarTexture->Create("Assets/Texture/slash.png");
}

ItemUI::~ItemUI(void)
{
	if (m_pStarTexture)
	{
		delete m_pStarTexture;
		m_pStarTexture = nullptr;
	}
}

void ItemUI::Draw(void)
{
	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);

	DirectX::XMFLOAT4X4 mat[3];

	// ワールド行列はXとYのみを考慮して作成
	{
		// 拡大
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(
			m_starScale.x,
			m_starScale.y,
			m_starScale.z
		);

		// 回転
		DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(m_starRotate.x);
		DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(m_starRotate.y);
		DirectX::XMMATRIX rotZ = DirectX::XMMatrixRotationZ(m_starRotate.z);

		// 移動
		DirectX::XMMATRIX move = DirectX::XMMatrixTranslation(
			m_starPos.x,
			m_starPos.y,
			m_starPos.z
		);

		// ワールド行列
		DirectX::XMMATRIX world = scale * rotX * rotY * rotZ * move;
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

		// ビュー行列
		DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

		// プロジェクション行列
		DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 100.0f);
		DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

		// スプライトの設定
		Sprite::SetWorld(mat[0]);
		Sprite::SetView(mat[1]);
		Sprite::SetProjection(mat[2]);
		Sprite::SetSize(DirectX::XMFLOAT2(100.0f, -100.0f));
		Sprite::SetTexture(m_pStarTexture);
		Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Sprite::SetUVPos({ 0.0f, 0.0f });
		Sprite::SetUVScale({ 1.0f, 1.0f });
		Sprite::Draw();
	}

	SetRenderTargets(1, &pRTV, pDSV);
}