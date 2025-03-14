//Goal.cpp
//2Dのゴールについて

#include "Goal.h"
#include "Sprite.h"

#include "SceneManager.hpp"

//コンストラクタ
Goal::Goal()
	: m_pGoalTexture(nullptr)
	, m_GoalPos(0.0f, 0.0f, 0.0f)
	, m_pPS(nullptr)

{
	//モデルの読み込み処理
	m_pGoalTexture = new Texture();
	if (m_pGoalTexture->Create("Assets/Texture/Goal.png"))
	{
		MessageBox(NULL, "ゴールの読み込みエラー", "Error", MB_OK);
	}

	SetGoal(false);

	m_pPS = new PixelShader();
	if (FAILED(m_pPS->Load("Assets/Shader/PS_Sprite.cso")))
	{
		MessageBox(NULL, "ゴールの読み込みエラー", "Error", MB_OK);
	}

	m_EffectGoal = LibEffekseer::Create("Assets/effect/Goal.efkefc");

}

//デストラクタ
Goal::~Goal()
{
	if (m_pPS)
	{
		delete m_pPS;
		m_pPS = nullptr;
	}
	if (m_pGoalTexture)
	{
		delete m_pGoalTexture;
		m_pGoalTexture = nullptr;
	}
}

//更新
void Goal::Update()
{
	if (IsGoal == true)
	{
		m_EffectHandle = LibEffekseer::GetManager()->Play(m_EffectGoal, m_GoalPos.x, m_GoalPos.y, m_GoalPos.z);
	}
}

//描画
void Goal::Draw(float x, float y, float z, float sizeX, float sizeY)
{

	SetPosition(x, y, z);

	GetSizeX(sizeX);
	GetSizeY(sizeY);


	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(x, y, z);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//プロジェクション行列には2Dとして表示するための行列を設定
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	// エフェクト
	DirectX::XMFLOAT4X4 effectMat[2];
	//effectMat[0] = m_pCamera->GetViewMatrix();
	//effectMat[1] = m_pCamera->GetProjectionMatrix();

	// Effekseerの仕様上行列を転置前の状態で渡す
	DirectX::XMMATRIX effectView = DirectX::XMLoadFloat4x4(&mat[1]);
	effectView = DirectX::XMMatrixTranspose(effectView);
	DirectX::XMMATRIX effectProj = DirectX::XMLoadFloat4x4(&mat[2]);
	effectProj = DirectX::XMMatrixTranspose(effectProj);
	DirectX::XMStoreFloat4x4(&effectMat[0], effectView);
	DirectX::XMStoreFloat4x4(&effectMat[1], effectProj);


//	LibEffekseer::SetViewPosition(DirectX::XMFLOAT3(0.0f, 1.5f, 6.0f));
//	LibEffekseer::SetCameraMatrix(effectMat[0], effectMat[1]);

	//スプライトの設定
	Sprite::SetPixelShader(m_pPS);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetOffset(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(sizeX, -sizeY));
	Sprite::SetTexture(m_pGoalTexture);
	Sprite::Draw();
}

float Goal::SetPosition(float x, float y, float z)
{
	m_GoalPos.x = x;
	m_GoalPos.y = y;
	m_GoalPos.z = z;

	return m_GoalPos.x;
	return m_GoalPos.y;
	return m_GoalPos.z;
}

DirectX::XMFLOAT3 Goal::GetPosition() const
{
	return m_GoalPos;
}

float Goal::GetSizeX(float x)
{
	m_GoalSize.x = x;
	return m_GoalSize.x;
}

float Goal::GetSizeY(float y)
{
	m_GoalSize.y = y;
	return m_GoalSize.y;
}

DirectX::XMFLOAT3 Goal::GetSize() const
{
	return m_GoalSize;
}

void Goal::SetGoal(bool Goal)
{
	IsGoal = Goal;
}

bool Goal::GetGoal()
{
	return IsGoal;
}
