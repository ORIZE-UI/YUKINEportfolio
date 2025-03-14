#include "TitleScene.h"
#include "Input.h"
#include"Gamepad.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include "SceneManager.hpp"

#define FILENAME "Assets/Texture/Title.png"

InputManager imanagerST = InputManager();

SceneTitle::SceneTitle()
	: m_pTexture(nullptr)
	, m_pFade(nullptr)
	, m_pCurtainUI(nullptr)
	, m_pPS(nullptr)
	,m_pSoundTitle(nullptr)
	, m_pSourceTitle(nullptr)
{
	m_pCurtainUI = new CurtainUI();

	m_pTexture = new Texture();
	if (FAILED(m_pTexture->Create(FILENAME)))
	{
		MessageBox(NULL, "Title", "Error", MB_OK);
	}

	m_pPS = new PixelShader();
	if (FAILED(m_pPS->Load("./Assets/Shader/PS_Sprite.cso")))
	{
		MessageBox(NULL, "Title Pixel Shader", "Error", MB_OK);
	}

	m_pSoundTitle = LoadSound("Assets/Sound/SE/Menu_sukoshihibiku_Oobayashi.wav");

	// カーテンフェードの取得
	m_pFade = new Fade(m_pCurtainUI);
}

SceneTitle::~SceneTitle()
{
	if (m_pFade)
	{
		delete m_pFade;
		m_pFade = nullptr;
	}

	if (m_pCurtainUI)
	{
		delete m_pCurtainUI;
		m_pCurtainUI = nullptr;
	}

	if (m_pPS)
	{
		delete m_pPS;
		m_pPS = nullptr;
	}
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void SceneTitle::Update(SceneManager* pSceneManager)
{
	imanagerST.addKeycode(0, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_A);
	imanagerST.addKeycode(1, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);
	imanagerST.inspect();
	if (imanagerST.getKey(0) & 0b011)
	{
		if(!m_pSourceTitle)
		{
			m_pSourceTitle = PlaySound(m_pSoundTitle);
		}

//		m_pFade->Start(false, 1.0f);
		pSceneManager->SetNextScene(SCENE_SELECT);

//		m_pFade->Start(false, 0.5f);	// フェードアウト
//		m_pSceneManager->ChangeScene(SceneManager::SCENE_TUTORIAL);	// 操作説明シーンに移る
	}
}

void SceneTitle::Draw()
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
		0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f
	);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));
	
	//スプライトの設定
	Sprite::SetPixelShader(m_pPS);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -720.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pTexture);
	Sprite::Draw();

	//m_pFade->Draw();
	//m_pCurtainUI->StageCurtainDraw();
}
