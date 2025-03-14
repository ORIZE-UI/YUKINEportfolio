#include "number.h"
#include "Sprite.h"
#include "Defines.h"

Number::Number(void)
	: m_nMaxDigit(2)
	, m_nNumber(0)
	, m_pos{ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }
	, m_size{ 150.0f, 150.0f }
{
	m_pNumberTexture = new Texture();
	m_pNumberTexture->Create("Assets/Texture/number.png");
}

Number::~Number(void)
{
	delete m_pNumberTexture;
	m_pNumberTexture = nullptr;
}

void Number::SetPos(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
}

void Number::SetSize(DirectX::XMFLOAT2 size)
{
	m_size = size;
}

void Number::SetNumber(int nNumber)
{
	m_nNumber = nNumber;

	int nMaxNumber = 1;
	for (int nCntDigit = 0; nCntDigit < m_nMaxDigit; nCntDigit++)
	{
		nMaxNumber *= 10;
	}
	nMaxNumber -= 1;

	if (m_nNumber > nMaxNumber)
	{
		m_nNumber = nMaxNumber;
	}
}

void Number::SetDigit(int nDigit)
{
	m_nMaxDigit = nDigit;
}

void Number::Draw(void)
{
	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);

	int nTempNum = m_nNumber;

	for (int nCntDigit = 0; nCntDigit < m_nMaxDigit; nCntDigit++)
	{
		// 表示する数字を求める
		int nDrawNum = nTempNum % 10;

		// 桁の描画
		DirectX::XMFLOAT4X4 mat[3];
		DirectX::XMFLOAT3 bgScale = { 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT3 bgRotate = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT2 bgSize = {
			m_size.x / (float)m_nMaxDigit,
			m_size.y
		};
		DirectX::XMFLOAT3 bgPos = {
			m_pos.x + ((float)(m_nMaxDigit - nCntDigit) * bgSize.x),
			m_pos.y,
			m_pos.z
		};

		DirectX::XMFLOAT2 uvScale = { 1.0f / 4.0f, 1.0f / 3.0f };
		DirectX::XMFLOAT2 uvOffset = {
			uvScale.x * (float)(nDrawNum % 4),
			uvScale.y * (float)(nDrawNum / 4)
		};

		// ワールド行列はXとYのみを考慮して作成

		// 拡大
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(
			bgScale.x,
			bgScale.y,
			bgScale.z
		);

		// 回転
		DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(bgRotate.x);
		DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(bgRotate.y);
		DirectX::XMMATRIX rotZ = DirectX::XMMatrixRotationZ(bgRotate.z);

		// 移動
		DirectX::XMMATRIX move = DirectX::XMMatrixTranslation(
			bgPos.x,
			bgPos.y,
			bgPos.z
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
		Sprite::SetSize(DirectX::XMFLOAT2(bgSize.x, -bgSize.y));
		Sprite::SetTexture(m_pNumberTexture);
		Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Sprite::SetUVPos(uvOffset);
		Sprite::SetUVScale(uvScale);

		Sprite::Draw();

		// 桁の更新
		nTempNum /= 10;

	}

	Sprite::SetUVPos({ 0.0f, 0.0f });
	Sprite::SetUVScale({ 1.0f, 1.0f });
	SetRenderTargets(1, &pRTV, pDSV);
}