#include "CPUCar.h"
#include <math.h>
#include <algorithm>

// 静的定数の定義
const float CPUCar::DECREASE_SPEED = 0.95f;

// コンストラクタ
CPUCar::CPUCar()
	: m_pCamera(nullptr)
	, m_pos(23.0f, 0.0f, 0.0f)
	, m_lookDir(0.0f, 0.0f, 1.0f)
	, m_speed(0.0f, 0.0f, 0.0f)
	, m_currentControlPoint(0)
	, m_fAccelLen(0.05f)
	, m_fMaxSpeed(0.2f)
	, m_turnSpeed(0.03f)
	, m_bIsActiveTimer(false)
{
	// モデルのロード
	m_pModel = new Model();
	if (!m_pModel->Load("Assets/Model/lowPolyCar2.fbx", 1.0f, Model::XFlip))
	{
		MessageBox(NULL, "ModelRead", "Error", MB_OK);
	}

	// 頂点シェーダのロード
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBoxA(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pModel->SetVertexShader(m_pVS);

	// 影のテクスチャ
	m_pShadowTexture = new Texture();
	m_pShadowTexture->Create("Assets/Texture/Shadow.png");

	// 軌跡エフェクト
	m_pTrailEffect = new TrailEffect(this);
	m_pTrailEffect->AddLine(20);
}

// デストラクタ
CPUCar::~CPUCar()
{
	delete m_pModel;
	delete m_pVS;
	delete m_pShadowTexture;
	delete m_pTrailEffect;
}

// 更新処理
void CPUCar::Update()
{
	// タイマーが動作中の場合
	if (!m_bIsActiveTimer)
	{
		m_speed = { 0.0f, 0.0f, 0.0f }; // 速度をゼロにする
		return;
	}

	// 速度や方向がゼロベクトルの場合、デフォルト値を設定
	//if (DirectX::XMVector3Equal(XMLoadFloat3(&m_lookDir), DirectX::XMVectorZero()))
	//{
	//	m_lookDir = { 0.0f, 0.0f, 1.0f };
	//}
	//
	//if (DirectX::XMVector3Equal(XMLoadFloat3(&m_speed), DirectX::XMVectorZero()))
	//{
	//	m_speed = { 0.0f, 0.0f, 0.0f }; // 動いていない場合は速度ゼロ
	//}

	if (m_controlPoints.empty())
		return;

	// 現在の制御ポイントを取得
	DirectX::XMFLOAT3 target = m_controlPoints[m_currentControlPoint];
	DirectX::XMVECTOR vTarget = DirectX::XMLoadFloat3(&target);
	DirectX::XMVECTOR vPos = DirectX::XMLoadFloat3(&m_pos);

	// 方向ベクトルを計算
	DirectX::XMVECTOR vDir = DirectX::XMVectorSubtract(vTarget, vPos);
	vDir = DirectX::XMVector3Normalize(vDir);

	// 回転処理
	DirectX::XMVECTOR vLookDir = DirectX::XMLoadFloat3(&m_lookDir);
	DirectX::XMVECTOR axis = DirectX::XMVector3Cross(vLookDir, vDir);
	axis = DirectX::XMVector3Normalize(axis);

	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationAxis(axis, m_turnSpeed);
	vLookDir = DirectX::XMVector3TransformNormal(vLookDir, rotMat);
	DirectX::XMStoreFloat3(&m_lookDir, vLookDir);

	// 加速処理
	DirectX::XMVECTOR vMove = DirectX::XMVectorScale(vLookDir, m_fAccelLen);
	DirectX::XMFLOAT3 accel;
	DirectX::XMStoreFloat3(&accel, vMove);

	m_speed.x += accel.x;
	m_speed.y += accel.y;
	m_speed.z += accel.z;

	// 速度制限
	DirectX::XMVECTOR vSpeed = DirectX::XMLoadFloat3(&m_speed);
	float fSpeedLen = DirectX::XMVectorGetX(DirectX::XMVector3Length(vSpeed));
	if (fSpeedLen > m_fMaxSpeed)
	{
		vSpeed = DirectX::XMVector3Normalize(vSpeed);
		vSpeed = DirectX::XMVectorScale(vSpeed, m_fMaxSpeed);
	}

	// 減速処理
	vSpeed = DirectX::XMVectorScale(vSpeed, DECREASE_SPEED);
	DirectX::XMStoreFloat3(&m_speed, vSpeed);

	// 座標更新
	m_pos.x += m_speed.x;
	m_pos.y += m_speed.y;
	m_pos.z += m_speed.z;

	// 現在の制御点との距離を計算し、十分近ければ次の制御点に切り替える
	float distanceToTarget = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(vTarget, vPos)));
	if (distanceToTarget < 1.0f)
	{
		m_currentControlPoint = (m_currentControlPoint + 1) % m_controlPoints.size();
	}

	// 軌跡エフェクト更新
	m_pTrailEffect->Update();
}

// 描画処理
void CPUCar::Draw()
{
	if (!m_pCamera)
		return;

	DirectX::XMMATRIX move = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(move));
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();

	m_pVS->WriteBuffer(0, mat);
	m_pModel->Draw();
}

// カメラの設定
void CPUCar::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

// 現在位置の取得
DirectX::XMFLOAT3 CPUCar::GetPos() const
{
	return m_pos;
}

// 制御ポイントを追加
void CPUCar::AddControlPoint(const DirectX::XMFLOAT3& point)
{
	m_controlPoints.push_back(point);
}

void CPUCar::SetControlPoints(const std::vector<DirectX::XMFLOAT3>& controlPoints)
{
	m_controlPoints = controlPoints; // 道の制御点を設定
	m_currentControlPoint = 0; // 最初の制御点からスタート
}

//タイマーの状態を設定
void CPUCar::SetActiveTimer(bool isActive)
{
	m_bIsActiveTimer = isActive;
}