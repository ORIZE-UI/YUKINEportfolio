#include "SceneResult.h"
#include "Input.h"
#include"Gamepad.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include "SceneManager.hpp"

#define FILENAME "Assets/Texture/backgroundandcoin.png"

InputManager imanagerSR = InputManager();


std::chrono::steady_clock::time_point lastSound;
const std::chrono::milliseconds soundInterval = std::chrono::milliseconds(4000);//再生時間三秒の時

SceneResult::SceneResult()
	: m_pBGTexture(nullptr)
	, m_pClearIcon(nullptr)
	, m_pNextIcon(nullptr)
	, m_pFade(nullptr)
	, m_pCurtainUI(nullptr)
	, m_pCoinIcon(nullptr)
	, m_pPS(nullptr)
	,m_pSoundResult(nullptr)
	,m_pSourceVoiceResult(nullptr)
	,m_pSoundResultClear(nullptr)
	,m_pSourceVoiceClear(nullptr)
{
	m_pCurtainUI = new CurtainUI();
	m_pBGTexture = new Texture();
	if (FAILED(m_pBGTexture->Create(FILENAME)))
	{
		MessageBox(NULL, "リザルト背景", "Error", MB_OK);
	}
	m_pClearIcon = new Texture();
	if (FAILED(m_pClearIcon->Create("Assets/Texture/clear.png")))
	{
		MessageBox(NULL, "リザルトclear", "Error", MB_OK);
	}
	m_pNextIcon = new Texture();
	if (FAILED(m_pNextIcon->Create("Assets/Texture/nextperformance.png")))
	{
		MessageBox(NULL, "リザルト", "Error", MB_OK);
	}
	m_pCoinIcon = new Texture();
	if (FAILED(m_pCoinIcon->Create("Assets/Texture/coin.png")))
	{
		MessageBox(NULL, "リザルトコインアイコン", "Error", MB_OK);
	}

	m_pPS = new PixelShader();
	if (FAILED(m_pPS->Load("./Assets/Shader/PS_Sprite.cso")))
	{
		MessageBox(NULL, "Result Pixel Shader", "Error", MB_OK);
	}

	m_pSoundResult = LoadSound("Assets/Sound/SE/Menu_sukoshihibiku_Oobayashi.wav");

	m_pSoundResultClear= LoadSound("Assets/Sound/BGM/Result_clear_Yamada.wav",false);

	// カーテンフェードの取得
	m_pFade = new Fade(m_pCurtainUI);
}

SceneResult::~SceneResult()
{
	if (m_pFade)
	{
		delete m_pFade;
		m_pFade = nullptr;
	}
	if (m_pPS)
	{
		delete m_pPS;
		m_pPS = nullptr;
	}
	if (m_pCoinIcon)
	{
		delete m_pCoinIcon;
		m_pCoinIcon = nullptr;
	}
	if (m_pCurtainUI)
	{
		delete m_pCurtainUI;
		m_pCurtainUI = nullptr;
	}
	if (m_pNextIcon)
	{
		delete m_pNextIcon;
		m_pNextIcon = nullptr;
	}
	if (m_pClearIcon)
	{
		delete m_pClearIcon;
		m_pClearIcon = nullptr;
	}
	if (m_pBGTexture)
	{
		delete m_pBGTexture;
		m_pBGTexture = nullptr;
	}
	m_pSourceVoiceClear->Stop();
}

void SceneResult::Update(SceneManager* pSceneManager)
{
	imanagerSR.addKeycode(0, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_A);

	imanagerSR.inspect();

	if (!m_pSourceVoiceClear)
	{
		m_pSourceVoiceClear = PlaySound(m_pSoundResultClear);
	}
	if (imanagerSR.getKey(0) & 0b011)
	{
		if (!m_pSourceVoiceResult)
		{
			m_pSourceVoiceResult = PlaySound(m_pSoundResult);
		}
		pSceneManager->SetNextScene(SCENE_SELECT);
		m_GetCoinNum1 = 0;
		m_GetCoinNum2 = 0;
		m_GetCoinNum3 = 0;
	}
}

void SceneResult::BGDraw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			635.0f, 360.0f, 0.0f
		);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//ビュー行列は2Dだとカメラの位置があまり関係ないので、単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//プロジェクション行列には2Dとして表示するための行列を設定する
	//この行列で2Dぼスクリーンの大きさが決まる
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1270.0f, 720.0f, 0.0f, 0.1f, 10.0f
	);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	//スプライトの設定
	Sprite::SetPixelShader(m_pPS);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(1270.0f, -720.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pBGTexture);
	Sprite::Draw();

	m_GetCoinNum1 = m_pSceneGame->GetCoinNum1();
	m_GetCoinNum2 = m_pSceneGame->GetCoinNum2();
	m_GetCoinNum3 = m_pSceneGame->GetCoinNum3();
}

void SceneResult::ClearDraw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			340.0f, 360.0f, 0.0f
		);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//ビュー行列は2Dだとカメラの位置があまり関係ないので、単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//プロジェクション行列には2Dとして表示するための行列を設定する
	//この行列で2Dぼスクリーンの大きさが決まる
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1270.0f, 720.0f, 0.0f, 0.1f, 10.0f
	);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	//スプライトの設定
	Sprite::SetPixelShader(m_pPS);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(1270.0f, -720.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pClearIcon);
	Sprite::Draw();
}

void SceneResult::NextDraw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			640.0f, 360.0f, 0.0f
		);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//ビュー行列は2Dだとカメラの位置があまり関係ないので、単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//プロジェクション行列には2Dとして表示するための行列を設定する
	//この行列で2Dぼスクリーンの大きさが決まる
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1270.0f, 720.0f, 0.0f, 0.1f, 10.0f
	);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	//スプライトの設定
	Sprite::SetPixelShader(m_pPS);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(1270.0f, -720.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pNextIcon);
	Sprite::Draw();

	if (m_GetCoinNum1 == 1)
	{
		ResultCoinDraw(216.0f);
	}
	if (m_GetCoinNum2 == 10)
	{
		ResultCoinDraw(613.0f);
	}
	if (m_GetCoinNum3 == 100)
	{
		ResultCoinDraw(1016.0f);
	}
}

void SceneResult::ResultCoinDraw(float x)
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(x, 482.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//ビュー行列は2Dだとカメラの位置があまり関係ないので、単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//プロジェクション行列には2Dとして表示するための行列を設定する
	//この行列で2Dぼスクリーンの大きさが決まる
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1270.0f, 720.0f, 0.0f, 0.1f, 10.0f
	);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	//スプライトの設定
	Sprite::SetPixelShader(m_pPS);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(250.0f, -250.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pCoinIcon);
	Sprite::Draw();
}
