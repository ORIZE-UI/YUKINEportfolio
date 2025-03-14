#include "meter.h"
#include "Sprite.h"
#include "Defines.h"

Meter::Meter(Player* pPlayer)
	: m_fRotZ(0.0f)
{
	m_pPlayer = pPlayer;

	m_pBackImg = new Texture();
	m_pBackImg->Create("Assets/Texture/meterBack.png");

	m_pFrontImg = new Texture();
	m_pFrontImg->Create("Assets/Texture/meterFront.png");
}

Meter::~Meter(void)
{
	delete m_pBackImg;
	delete m_pFrontImg;
}

void Meter::Update(void)
{
	DirectX::XMFLOAT3 playerSpeed = m_pPlayer->GetSpeed();
	float fSpeedLen = sqrtf(
		playerSpeed.x * playerSpeed.x +
		playerSpeed.y * playerSpeed.y +
		playerSpeed.z * playerSpeed.z
	);

	float fTargetRotZ = (240.0f * (3.141592f / 180.0f)) * (fSpeedLen / 0.5f);

	if (fTargetRotZ > m_fRotZ)
	{
		m_fRotZ += 0.05f;
	}
	else if (fTargetRotZ < m_fRotZ)
	{
		m_fRotZ -= 0.05f;
	}
}

void Meter::Draw(void)
{
	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);

	DirectX::XMFLOAT4X4 mat[3];

	// 背景
	{
		// 拡大
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(
			1.0f,
			1.0f,
			1.0f
		);

		// 回転
		DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(0.0f);
		DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(0.0f);
		DirectX::XMMATRIX rotZ = DirectX::XMMatrixRotationZ(0.0f);

		// 移動
		DirectX::XMMATRIX move = DirectX::XMMatrixTranslation(
			150.0f,
			570.0f,
			0.0f
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
		Sprite::SetSize(DirectX::XMFLOAT2(360.0f, -300.0f));
		Sprite::SetTexture(m_pBackImg);
		Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Sprite::SetUVPos({ 100.0f, 0.0f });
		Sprite::SetUVScale({ 1.0f, 1.0f });
		Sprite::Draw();
	}

	// 表
	{
		// 拡大
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(
			1.0f,
			1.0f,
			1.0f
		);

		// 回転
		DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(0.0f);
		DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(0.0f);
		DirectX::XMMATRIX rotZ = DirectX::XMMatrixRotationZ(m_fRotZ);

		// 移動
		DirectX::XMMATRIX move = DirectX::XMMatrixTranslation(
			150.0f,
			570.0f,
			0.0f
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
		Sprite::SetSize(DirectX::XMFLOAT2(360.0f, -300.0f));
		Sprite::SetTexture(m_pFrontImg);
		Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Sprite::SetUVPos({ 100.0f, 0.0f });
		Sprite::SetUVScale({ 1.0f, 1.0f });
		Sprite::Draw();
	}

	SetRenderTargets(1, &pRTV, pDSV);
}