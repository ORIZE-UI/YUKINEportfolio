#include "SceneTitle.h"
#include "Sprite.h"
#include "Defines.h"
#include "Main.h"
#include "Input.h"

SceneTitle::SceneTitle(void)
{
	m_pTitleTexture = new Texture();
	m_pTitleTexture->Create("Assets/Texture/titleBG.png");

	m_pFade = new Fade();
	m_pFade->StartFade(Fade::FadeKind::FadeIn);

}

SceneTitle::~SceneTitle(void)
{
	if (m_pFade)
	{
		delete m_pFade;
		m_pFade = nullptr;
	}

	if (m_pTitleTexture)
	{
		delete m_pTitleTexture;
		m_pTitleTexture = nullptr;
	}
}

void SceneTitle::Update(float tick)
{
	if (IsKeyTrigger(VK_RETURN))
	{
		// フェード開始
		m_pFade->StartFade(Fade::FadeKind::FadeOut);
	}

	m_pFade->Update();

	if (m_pFade->GetKind() == Fade::FadeKind::FadeOut && m_pFade->IsFinish())
	{
		// ゲームシーン開始
		GetSceneMng()->SetScene(SceneManager::CSceneKind::Game);
	}
}

void SceneTitle::Draw(void)
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
	Sprite::SetTexture(m_pTitleTexture);
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	Sprite::Draw();

	SetRenderTargets(1, &pRTV, pDSV);

	m_pFade->Draw();
}