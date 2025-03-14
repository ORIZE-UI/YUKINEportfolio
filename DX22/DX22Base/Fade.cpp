#include "Fade.h"
#include "Sprite.h"
#include "Main.h"
#include "Defines.h"

const float	Fade::FADE_SPEED = 0.01f;

Fade::Fade(void)
	: m_fadeState(FadeKind::None)
	, m_fAlpha(0.0f)
{
	m_pFadeTexture = new Texture();
	m_pFadeTexture->Create("Assets/Texture/black.png");
}

Fade::~Fade(void)
{
	delete m_pFadeTexture;
	m_pFadeTexture = nullptr;
}

void Fade::Update(void)
{
	switch (m_fadeState)
	{
	case Fade::FadeKind::None:
		break;
	case Fade::FadeKind::FadeIn:
		m_fAlpha -= FADE_SPEED;

		if (m_fAlpha <= 0.0f)
		{
			m_fAlpha = 0.0f;
		}

		break;
	case Fade::FadeKind::FadeOut:
		m_fAlpha += FADE_SPEED;

		if (m_fAlpha >= 1.0f)
		{
			m_fAlpha = 1.0f;
		}

		break;
	default:
		break;
	}

}

void Fade::Draw(void)
{
	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);

	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMFLOAT3 bgScale = { 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 bgRotate = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 bgPos = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f };
	DirectX::XMFLOAT2 bgSize = { SCREEN_WIDTH, SCREEN_HEIGHT };

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
	Sprite::SetTexture(m_pFadeTexture);
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, m_fAlpha });
	Sprite::Draw();

	SetRenderTargets(1, &pRTV, pDSV);
}

void Fade::StartFade(FadeKind kind)
{
	m_fadeState = kind;

	switch (m_fadeState)
	{
	case Fade::FadeKind::None:
		break;
	case Fade::FadeKind::FadeIn:
		m_fAlpha = 1.0f;
		break;
	case Fade::FadeKind::FadeOut:
		m_fAlpha = 0.0f;
		break;
	default:
		break;
	}
}

Fade::FadeKind Fade::GetKind(void)
{
	return m_fadeState;
}

bool Fade::IsFinish(void)
{
	switch (m_fadeState)
	{
	case Fade::FadeKind::None:
		break;
	case Fade::FadeKind::FadeIn:
		if (m_fAlpha <= 0.0f)
		{
			return true;
		}
		break;
	case Fade::FadeKind::FadeOut:
		if (m_fAlpha >= 1.0f)
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}