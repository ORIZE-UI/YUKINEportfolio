#include "signal.h"
#include "Sprite.h"
#include "Defines.h"

const int	Signal::MAX_SIGNAL_FRAME = 180;

Signal::Signal()
	: m_nCntFrame(0)
	, m_pos{ 640.0f, 100.0f, 0.0f }
	, m_bIsActive(false)
{
	m_pSignalTexture = new Texture();
	m_pSignalTexture->Create("Assets/Texture/signalChange.png");
}

Signal::~Signal()
{
	delete m_pSignalTexture;
	m_pSignalTexture = nullptr;
}

void Signal::Update(void)
{
	if (m_nCntFrame > MAX_SIGNAL_FRAME + 30)
	{
		m_pos.y -= 6.0f;
	}

	if (m_bIsActive)
	{
		m_nCntFrame++;
	}
}

void Signal::Draw(void)
{
	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);

	DirectX::XMFLOAT4X4 mat[3];

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
			m_pos.x,
			m_pos.y,
			m_pos.z
		);

		// ワールド行列
		DirectX::XMMATRIX world = scale * rotX * rotY * rotZ * move;
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

		// ビュー行列
		DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

		// プロジェクション行列
		DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 100.0f);
		DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

		// UV座標を計算
		DirectX::XMFLOAT2 uvOffset;
		uvOffset.x = 0.0f;

		if (m_nCntFrame < MAX_SIGNAL_FRAME / 3)
		{
			uvOffset.y = 0.0f;
		}
		else if (m_nCntFrame < MAX_SIGNAL_FRAME / 3 * 2)
		{
			uvOffset.y = 0.25f;
		}
		else if (m_nCntFrame < MAX_SIGNAL_FRAME)
		{
			uvOffset.y = 0.5f;
		}
		else
		{
			uvOffset.y = 0.75f;
		}

		// スプライトの設定
		Sprite::SetWorld(mat[0]);
		Sprite::SetView(mat[1]);
		Sprite::SetProjection(mat[2]);
		Sprite::SetSize(DirectX::XMFLOAT2(200.0f, -200.0f));
		Sprite::SetTexture(m_pSignalTexture);
		Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Sprite::SetUVPos(uvOffset);
		Sprite::SetUVScale({ 1.0f, 0.25f });
		Sprite::Draw();
	}

	SetRenderTargets(1, &pRTV, pDSV);

	Sprite::SetUVPos({ 0.0f, 0.0f });
	Sprite::SetUVScale({ 1.0f, 1.0f });

}

bool Signal::IsSignalEnd(void)
{
	return (m_nCntFrame > MAX_SIGNAL_FRAME - 1);
}

void Signal::CountDownStart(void)
{
	m_bIsActive = true;
}