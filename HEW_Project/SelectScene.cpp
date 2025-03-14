#include "SelectScene.h"
#include "Input.h"
#include"Gamepad.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include "SceneManager.hpp"

#define FILENAME "Assets/Texture/selectbackground.png"

InputManager imanagerSS = InputManager();

SelectScene::SelectScene()
	: m_pTexture(nullptr)
	, m_pBackTexture(nullptr)
	, m_pMenuTexture(nullptr)
	, m_pStartTexture(nullptr)
	, m_pCursorTexture(nullptr)
	, m_pStage1Texture(nullptr)
	, m_pStage2Texture(nullptr)
	, m_pStage3Texture(nullptr)
	, m_pStage4Texture(nullptr)
	, m_pStage5Texture(nullptr)
	, m_pFade(nullptr)
	, m_pCurtainUI(nullptr)
	, m_pPS(nullptr)
	, m_CursorPos(236.0f, 226.5f, 0.0f)
	,m_pSoundSelect(nullptr)
	,m_pSourceVoiceSelect(nullptr)
{
	m_pCurtainUI = new CurtainUI();
	m_pTexture = new Texture();
	if (FAILED(m_pTexture->Create(FILENAME)))
	{
		MessageBox(NULL, "ステージ選択", "Error", MB_OK);
	}

	m_pBackTexture = new Texture();
	if (FAILED(m_pBackTexture->Create("Assets/Texture/select_Back.png")))
	{
		MessageBox(NULL, "SelectUI系", "Error", MB_OK);
	}
	m_pMenuTexture = new Texture();
	if (FAILED(m_pMenuTexture->Create("Assets/Texture/select_Menu.png")))
	{
		MessageBox(NULL, "SelectUI系", "Error", MB_OK);
	}
	m_pStartTexture = new Texture();
	if (FAILED(m_pStartTexture->Create("Assets/Texture/select_start.png")))
	{
		MessageBox(NULL, "SelectUI系", "Error", MB_OK);
	}
	m_pCursorTexture = new Texture();
	if (FAILED(m_pCursorTexture->Create("Assets/Texture/select_select.png")))
	{
		MessageBox(NULL, "SelectUI系", "Error", MB_OK);
	}
	m_pStage1Texture = new Texture();
	if (FAILED(m_pStage1Texture->Create("Assets/Texture/Stage1.png")))
	{
		MessageBox(NULL, "SelectUI系", "Error", MB_OK);
	}
	m_pStage2Texture = new Texture();
	if (FAILED(m_pStage2Texture->Create("Assets/Texture/Stage2.png")))
	{
		MessageBox(NULL, "SelectUI系", "Error", MB_OK);
	}
	m_pStage3Texture = new Texture();
	if (FAILED(m_pStage3Texture->Create("Assets/Texture/Stage3.png")))
	{
		MessageBox(NULL, "SelectUI系", "Error", MB_OK);
	}
	m_pStage4Texture = new Texture();
	if (FAILED(m_pStage4Texture->Create("Assets/Texture/Stage4.png")))
	{
		MessageBox(NULL, "SelectUI系", "Error", MB_OK);
	}
	m_pStage5Texture = new Texture();
	if (FAILED(m_pStage5Texture->Create("Assets/Texture/Stage5.png")))
	{
		MessageBox(NULL, "SelectUI系", "Error", MB_OK);
	}

	m_pPS = new PixelShader();
	if (FAILED(m_pPS->Load("./Assets/Shader/PS_Sprite.cso")))
	{
		MessageBox(NULL, "Select Pixel Shader", "Error", MB_OK);
	}

	m_pSoundSelect = LoadSound("Assets/Sound/SE/Menu_sukoshihibiku_Oobayashi.wav");

	// カーテンフェードの取得
	m_pFade = new Fade(m_pCurtainUI);
}

SelectScene::~SelectScene()
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
	if (m_pCurtainUI)
	{
		delete m_pCurtainUI;
		m_pCurtainUI = nullptr;
	}
	if (m_pStage5Texture)
	{
		delete m_pStage5Texture;
		m_pStage5Texture = nullptr;
	}
	if (m_pStage4Texture)
	{
		delete m_pStage4Texture;
		m_pStage4Texture = nullptr;
	}
	if (m_pStage3Texture)
	{
		delete m_pStage3Texture;
		m_pStage3Texture = nullptr;
	}
	if (m_pStage2Texture)
	{
		delete m_pStage2Texture;
		m_pStage2Texture = nullptr;
	}
	if (m_pStage1Texture)
	{
		delete m_pStage1Texture;
		m_pStage1Texture = nullptr;
	}
	if (m_pCursorTexture)
	{
		delete m_pCursorTexture;
		m_pCursorTexture = nullptr;
	}
	if (m_pStartTexture)
	{
		delete m_pStartTexture;
		m_pStartTexture = nullptr;
	}
	if (m_pBackTexture)
	{
		delete m_pBackTexture;
		m_pBackTexture = nullptr;
	}
	if (m_pMenuTexture)
	{
		delete m_pMenuTexture;
		m_pMenuTexture = nullptr;
	}
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void SelectScene::Update(SceneManager* pSceneManager)
{
	imanagerSS.addKeycode(0, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_A);
	imanagerSS.addKeycode(1, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);
	imanagerSS.addKeycode(2, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_Y);
	imanagerSS.addKeycode(3, 0, GAMEPAD_KEYTYPE::LTrigger, XINPUT_GAMEPAD_LEFT_SHOULDER);
	imanagerSS.addKeycode(4, 0, GAMEPAD_KEYTYPE::RTrigger, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	imanagerSS.inspect();


	if (imanagerSS.getKey(2) & 0b001)
	{
		m_pSourceVoiceSelect = PlaySound(m_pSoundSelect);
		pSceneManager->SetNextScene(SCENE_TUTORIAL);
	}
	if (imanagerSS.getKey(1) & 0b011)
	{
		if (!m_pSourceVoiceSelect)
		{
			m_pSourceVoiceSelect = PlaySound(m_pSoundSelect);
		}
		pSceneManager->SetNextScene(SCENE_TITLE);
	}
	
	//カーソル移動
	if (imanagerSS.getKey(4) & 0b100)//カーソル右移動
	{
		if (m_CursorPos.x == 236.0f || m_CursorPos.x == 636.0f || m_CursorPos.x == 438.0f)
		{
			m_CursorPos.x += 400.0f;
		}
		else if (m_CursorPos.x == 1036.0f)
		{
			m_CursorPos.x = 438.0f;
			m_CursorPos.y = 456.1f;
		}
		else if (m_CursorPos.x == 838.0f)
		{
			m_CursorPos.x = 236.0f;
			m_CursorPos.y = 226.1f;
		}
	}
	if (imanagerSS.getKey(3) & 0b100)	//カーソル左移動
	{
		if (m_CursorPos.x == 636.0f || m_CursorPos.x == 1036.0f || m_CursorPos.x == 838.0f)
		{
			m_CursorPos.x -= 400.0f;
		}
		else if (m_CursorPos.x == 438.0f)
		{
			m_CursorPos.x = 1036.0f;
			m_CursorPos.y = 226.5f;
		}
		else if (m_CursorPos.x == 236.0f)
		{
			m_CursorPos.x = 838.0f;
			m_CursorPos.y = 456.1f;
		}
	}

	

	//各ゲームシーンに移動
	if (m_CursorPos.x == 236.0f && (imanagerSS.getKey(0) & 0b011))
	{
		if (!m_pSourceVoiceSelect)
		{
			m_pSourceVoiceSelect = PlaySound(m_pSoundSelect);
		}
		m_SelectStage = 1;
		pSceneManager->SetNextScene(SCENE_GAME);
	}
	else if (m_CursorPos.x == 636.0f && (imanagerSS.getKey(0) & 0b011))
	{
		if (!m_pSourceVoiceSelect)
		{
			m_pSourceVoiceSelect = PlaySound(m_pSoundSelect);
		}
		m_SelectStage = 2;
		pSceneManager->SetNextScene(SCENE_GAME);
	}
	else if (m_CursorPos.x == 1036.0f && (imanagerSS.getKey(0) & 0b011))
	{
		if (!m_pSourceVoiceSelect)
		{
			m_pSourceVoiceSelect = PlaySound(m_pSoundSelect);
		}
		m_SelectStage = 3;
		pSceneManager->SetNextScene(SCENE_GAME);
	}
	else if (m_CursorPos.x == 438.0f && (imanagerSS.getKey(0) & 0b011))
	{
		if (!m_pSourceVoiceSelect)
		{
			m_pSourceVoiceSelect = PlaySound(m_pSoundSelect);
		}
		m_SelectStage = 4;
		pSceneManager->SetNextScene(SCENE_GAME);
	}
	else if (m_CursorPos.x == 838.0f && (imanagerSS.getKey(0) & 0b011))
	{
		if (!m_pSourceVoiceSelect)
		{
			m_pSourceVoiceSelect = PlaySound(m_pSoundSelect);
		}
		m_SelectStage = 5;
		pSceneManager->SetNextScene(SCENE_GAME);
	}
	/*else if (m_CursorPos.x == 236.0f)
	{
		pSceneManager->SetNextScene(SCENE_GAME);
	}*/

	std::string test = "X:" + std::to_string(m_CursorPos.x) + " Y:" + std::to_string(m_CursorPos.y) + "\n";
	OutputDebugString(test.c_str());
}

void SelectScene::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			640.0f, 375.0f, 0.0f
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
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -680.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pTexture);
	Sprite::Draw();
}

void SelectScene::BackUIDraw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			140.0f, 680.0f, 0.0f
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
	Sprite::SetSize(DirectX::XMFLOAT2(265.0f, -52.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pBackTexture);
	Sprite::Draw();
}
void SelectScene::MenuUIDraw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			500.0f, 680.0f, 0.0f
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
	Sprite::SetSize(DirectX::XMFLOAT2(265.0f, -52.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pMenuTexture);
	Sprite::Draw();
}
void SelectScene::StartUIDraw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			1140.0f, 680.0f, 0.0f
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
	Sprite::SetSize(DirectX::XMFLOAT2(265.0f, -52.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pStartTexture);
	Sprite::Draw();
}
void SelectScene::Stage1Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			235.0f, 225.8f, 0.0f
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
	Sprite::SetSize(DirectX::XMFLOAT2(360.5f, -190.0f));				
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pStage1Texture);
	Sprite::Draw();
}
void SelectScene::Stage2Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			635.0f, 225.8f, 0.0f
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
	Sprite::SetSize(DirectX::XMFLOAT2(360.5f, -190.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pStage2Texture);
	Sprite::Draw();
}
void SelectScene::Stage3Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			1035.0f, 225.8f, 0.0f
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
	Sprite::SetSize(DirectX::XMFLOAT2(360.5f, -190.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pStage3Texture);
	Sprite::Draw();
}
void SelectScene::Stage4Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			437.0f, 455.0f, 0.0f
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
	Sprite::SetSize(DirectX::XMFLOAT2(360.5f, -190.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pStage4Texture);
	Sprite::Draw();
}
void SelectScene::Stage5Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			837.0f, 455.0f, 0.0f
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
	Sprite::SetSize(DirectX::XMFLOAT2(360.5f, -190.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pStage5Texture);
	Sprite::Draw();
}


void SelectScene::CursorUIDraw(float x, float y, float z)
{
	SetPosition(x, y, z);

	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(x, y, z);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//ビュー行列は2Dだとカメラの位置があまり関係ないので、単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//プロジェクション行列には2Dとして表示するための行列を設定する
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f
	);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	//スプライトの設定
	Sprite::SetPixelShader(m_pPS);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(380.0f, -220.0f));		//602×340
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pCursorTexture);
	Sprite::Draw();
}
void SelectScene::SetPosition(float x, float y, float z)
{
	m_CursorPos.x = x;
	m_CursorPos.y = y;
	m_CursorPos.z = z;
}
DirectX::XMFLOAT3 SelectScene::GetPosition() const
{
	return m_CursorPos;
}
