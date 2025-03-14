#include "player.h"
#include "Input.h"
#include "Sprite.h"
#include <math.h>

//回転速度（ラジアン/フレーム）
const float Player::ROT_SPEED = 2.0f * (3.141592f / 180.0f);
//減速係数
const float Player::DECREASE_SPEED = 0.95f;
//スピードアップアイテムの効果時間（フレーム数）
const int	Player::SPEEDUPITEM_FRAME = 30;

// 各ギアの比率（仮設定）
const float Player::GEAR_RATIOS[6] = { 3.2f, 2.5f, 2.0f, 1.5f, 1.2f, 1.0f };
const float Player::MAX_RPM = 8000.0f;
const float Player::SHIFT_UP_RPM = 7000.0f;
const float Player::SHIFT_DOWN_RPM = 2500.0f;

// コンストラクタ: プレイヤーの初期化
Player::Player()
	: m_pos(28.0f, 1.0f, 0.0f)          // 初期位置
	, m_pCamera(nullptr)                // カメラの初期化
	, m_lookDir{ 0.0f, 0.0f, 1.0f }     // 初期の方向ベクトル
	, m_speed{ 0.0f, 0.0f, 0.0f }       // 初期の速度
	, m_nCntRap(0)                      // 周回数カウンタ
	, m_nLastHitRoadPoly(1)             // 最後に接触したポリゴン番号
	, m_bIsActiveTimer(false)           // タイマーの状態
	, m_nFrameTimer(0)                  // タイマーのフレーム数
	, m_currentGear(1)  // 初期ギア
	, m_engineRPM(1000.0f)
	, m_bClutchPressed(false)
{
	// 車両モデルのロード
	m_pModel = new Model();
	m_pModel->Load("Assets/Model/lowPolyCar2.fbx", 1.0f, Model::XFlip);

	// 頂点シェーダのロード
	m_pVS = new VertexShader();
	m_pVS->Load("Assets/Shader/VS_Model.cso");
	m_pModel->SetVertexShader(m_pVS); // モデルにシェーダを設定

	// 車両の影用テクスチャのロード
	m_pShadowTexture = new Texture();
	m_pShadowTexture->Create("Assets/Texture/Shadow.png");

	// 車両上部に表示するエフェクト用テクスチャのロード
	m_engelWing = new Texture();
	m_engelWing->Create("Assets/Texture/tenshi_ring.png");

	// エフェクトの初期化
	m_pEffect = new Effect();
	m_pMoveEmitter = new MoveEmitter();
	m_pEffect->AddEmitter(m_pMoveEmitter); // 移動時のエフェクトを設定

	// 車両の軌跡エフェクトの初期化
	m_pTrailEffect = new TrailEffect(this);
	m_pTrailEffect->AddLine(20); // 軌跡の最大ライン数を設定

	// 軌跡用テクスチャのロード
	m_pTrailTex = new Texture();
	m_pTrailTex->Create("Assets/Texture/trail.png");

	// 初期状態の加速タイプを設定
	SetAccelKind(AccelKind::Road);
}

//デストラクタ
Player::~Player()
{
	delete m_pVS;
	delete m_pModel;
	delete m_pShadowTexture;
	delete m_engelWing;
	delete m_pMoveEmitter;
	delete m_pEffect;
	delete m_pTrailEffect;
	delete m_pTrailTex;
}

// プレイヤーの更新処理
void Player::Update(void)
{
	// 速度や方向がゼロベクトルの場合、デフォルト値を設定
	if (DirectX::XMVector3Equal(XMLoadFloat3(&m_lookDir), DirectX::XMVectorZero()))
	{
		m_lookDir = { 0.0f, 0.0f, 1.0f };
	}
	if (DirectX::XMVector3Equal(XMLoadFloat3(&m_speed), DirectX::XMVectorZero()))
	{
		m_speed = { 0.0f, 0.0f, 0.0f }; // 動いていない場合は速度ゼロ
	}

	// カメラ情報を取得
	DirectX::XMFLOAT3 camPos = m_pCamera->GetPos();
	DirectX::XMFLOAT3 camLook = m_pCamera->GetLook();
	DirectX::XMVECTOR vCamPos = DirectX::XMVectorSet(camPos.x, camPos.y, camPos.z, 0.0f);
	DirectX::XMVECTOR vCamLook = DirectX::XMVectorSet(camLook.x, camLook.y, camLook.z, 0.0f);

	// 左右旋回
	if (IsKeyPress(VK_RIGHT))
	{
		DirectX::XMVECTOR vLookDir = DirectX::XMLoadFloat3(&m_lookDir);
		DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationY(ROT_SPEED);
		vLookDir = DirectX::XMVector3TransformCoord(vLookDir, rotMat);
		DirectX::XMStoreFloat3(&m_lookDir, vLookDir);
	}
	if (IsKeyPress(VK_LEFT))
	{
		DirectX::XMVECTOR vLookDir = DirectX::XMLoadFloat3(&m_lookDir);
		DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationY(-ROT_SPEED);
		vLookDir = DirectX::XMVector3TransformCoord(vLookDir, rotMat);
		DirectX::XMStoreFloat3(&m_lookDir, vLookDir);
	}

	// クラッチ処理
	if (IsKeyRelease('Q')) { m_bClutchPressed = true; }
	if (IsKeyPress('Q')) { m_bClutchPressed = false; }

	// ギアチェンジ (クラッチを踏んでいる時のみ変更可能)
	if (m_bClutchPressed)
	{
		if (IsKeyPress('E') && m_currentGear < 6)
		{
			m_currentGear++;
			m_engineRPM *= 0.75f; // ギアアップ時に回転数を下げる
		}
		if (IsKeyPress('W') && m_currentGear > 1)
		{
			m_currentGear--;
			m_engineRPM *= 1.2f; // ギアダウン時に回転数を上げる
		}
	}

	// エンジン回転数の計算
	float speedFactor = DirectX::XMVectorGetX(DirectX::XMVector3Length(XMLoadFloat3(&m_speed))) * 300.0f;
	if (!m_bClutchPressed) // クラッチがつながっている時のみ回転数を速度と同期
	{
		m_engineRPM = speedFactor * GEAR_RATIOS[m_currentGear - 1];
	}
	else // クラッチを切っている間は回転数が極端に下がらないよう制限
	{
		m_engineRPM = (m_engineRPM * 0.98f > 1000.0f) ? (m_engineRPM * 0.98f) : 1000.0f;
			//std::max(m_engineRPM * 0.98f, 1000.0f);
	}

	// エンジン回転数の上限・下限を設定
	m_engineRPM = (m_engineRPM < 1000.0f) ? 1000.0f : ((m_engineRPM > MAX_RPM) ? MAX_RPM : m_engineRPM);
		//std::max(1000.0f, std::min(m_engineRPM, MAX_RPM));

	// 加速処理 (クラッチが繋がっている時のみ加速可能)
	if (!m_bClutchPressed && m_bIsActiveTimer)
	{
		if (IsKeyPress(VK_UP))
		{
			float accelFactor = 0.005f * GEAR_RATIOS[m_currentGear - 1];
			m_speed.x += m_lookDir.x * accelFactor;
			m_speed.z += m_lookDir.z * accelFactor;
		}
	}

	// ギアごとの最高速度制限
	float maxSpeed = m_fMaxSpeed * GEAR_RATIOS[m_currentGear - 1];
	if (DirectX::XMVectorGetX(DirectX::XMVector3Length(XMLoadFloat3(&m_speed))) > maxSpeed)
	{
		DirectX::XMVECTOR vSpeed = DirectX::XMVector3Normalize(XMLoadFloat3(&m_speed));
		vSpeed = DirectX::XMVectorScale(vSpeed, maxSpeed);
		DirectX::XMStoreFloat3(&m_speed, vSpeed);
	}

	// 減速処理
	DirectX::XMVECTOR vSpeed = XMLoadFloat3(&m_speed);
	vSpeed = DirectX::XMVectorScale(vSpeed, DECREASE_SPEED);
	DirectX::XMStoreFloat3(&m_speed, vSpeed);

	// 座標更新
	m_pos.x += m_speed.x;
	m_pos.y += m_speed.y;
	m_pos.z += m_speed.z;

	// エフェクト更新
	m_pEffect->Update();
	m_pTrailEffect->Update();

	// タイマーの更新
	if (m_bIsActiveTimer)
	{
		m_nFrameTimer++;
	}

	// スピードアップのフレームをカウント
	if (m_accelKind == AccelKind::SpeedUpItem)
	{
		m_nCntSpeedUpFrame++;
		if (m_nCntSpeedUpFrame > SPEEDUPITEM_FRAME)
		{
			// スピードアップ終了
			SetAccelKind(AccelKind::Road);
		}
	}
}

// 描画処理
void Player::Draw(void)
{
	if (!m_pCamera)
	{
		// カメラがない
		return;
	}

	DirectX::XMFLOAT4X4 mat[3];

	// 行列の計算
	{

		DirectX::XMMATRIX move = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
		DirectX::XMMATRIX world = move;

		// 角度を求める

		DirectX::XMVECTOR vFromDir = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		DirectX::XMVECTOR vToDir = DirectX::XMVectorSet(m_lookDir.x, m_lookDir.y, m_lookDir.z, 0.0f);

		DirectX::XMVECTOR vAngle = DirectX::XMVector3Dot(vFromDir, vToDir);
		float fAngle;
		DirectX::XMStoreFloat(&fAngle, vAngle);
		fAngle = acosf(fAngle);

		// 回転軸を求める
		DirectX::XMVECTOR axis;
		axis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFromDir, vToDir));

		// 回転行列を求める
		DirectX::XMMATRIX rot;

		if (fabsf(m_lookDir.z) > 1.0f)
		{
			if (m_lookDir.z > 0.0f)
			{
				rot = DirectX::XMMatrixRotationY(3.141592f);
			}
			else
			{
				rot = DirectX::XMMatrixRotationY(0.0f);
			}
		}
		else if (!DirectX::XMVector3Equal(axis, DirectX::XMVectorZero()))
		{
			rot = DirectX::XMMatrixRotationAxis(axis, fAngle);
		}
		else
		{
			if (m_lookDir.z > 0.0f)
			{
				rot = DirectX::XMMatrixRotationY(3.141592f);
			}
			else
			{
				rot = DirectX::XMMatrixRotationY(0.0f);
			}
		}
		world = rot * move;

		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

		mat[1] = m_pCamera->GetViewMatrix();
		mat[2] = m_pCamera->GetProjectionMatrix();

		// モデル表示
		m_pVS->WriteBuffer(0, mat);
		m_pModel->Draw();
	}

	// 影の表示
	{
		// ワールド行列
		DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(90.0f * (3.141592f / 180.0f));
		DirectX::XMMATRIX move = DirectX::XMMatrixTranslation(m_pos.x, 0.001f, m_pos.z);
		DirectX::XMMATRIX mat = rotX * move;
		DirectX::XMFLOAT4X4 world;
		DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(mat));
		Sprite::SetWorld(world);
		Sprite::SetView(m_pCamera->GetViewMatrix());
		Sprite::SetProjection(m_pCamera->GetProjectionMatrix());
		Sprite::SetSize(DirectX::XMFLOAT2(0.5f, 0.5f));
		Sprite::SetTexture(m_pShadowTexture);
		Sprite::Draw();
	}

	// ビルボードの処理
	{
		RenderTarget* pRTV = GetDefaultRTV();
		DepthStencil* pDSV = GetDefaultDSV();
		SetRenderTargets(1, &pRTV, nullptr);

		DirectX::XMFLOAT4X4 inv;	//逆行列の格納崎
		inv = m_pCamera->GetViewMatrix();

		// カメラの行列を転置前に直す
		DirectX::XMMATRIX matInv = DirectX::XMLoadFloat4x4(&inv);
		matInv = DirectX::XMMatrixTranspose(matInv);

		// 移動成分を打ち消す
		DirectX::XMStoreFloat4x4(&inv, matInv);
		inv._41 = inv._42 = inv._43 = 0.0f;

		// 逆行列を計算
		matInv = DirectX::XMLoadFloat4x4(&inv);
		matInv = DirectX::XMMatrixInverse(nullptr, matInv);

		DirectX::XMMATRIX move = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y + 1.7f, m_pos.z);
		DirectX::XMMATRIX world = matInv * move;

		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

		Sprite::SetWorld(mat[0]);
		Sprite::SetTexture(m_engelWing);
		Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		//Sprite::Draw();

		SetRenderTargets(1, &pRTV, pDSV);

		// エフェクト描画
		{
			DirectX::XMFLOAT4X4   billBoardMat;

			DirectX::XMStoreFloat4x4(&billBoardMat, matInv);
			m_pEffect->SetBillboardMatrix(billBoardMat);
			m_pEffect->Draw(m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
		}
	}

	{
		//軌跡描画
		m_pTrailEffect->SetView(m_pCamera->GetViewMatrix());
		m_pTrailEffect->SetProjection(m_pCamera->GetProjectionMatrix());
		m_pTrailEffect->SetTexture(m_pTrailTex);
		m_pTrailEffect->Draw();
	}
}

// 各種セッター・ゲッター関数
void Player::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

DirectX::XMFLOAT3 Player::GetPos() const
{
	return m_pos;
}

void Player::SetPos(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
}

DirectX::XMFLOAT3 Player::GetLookDir(void)
{
	return m_lookDir;
}

void Player::AddRap(void)
{
	m_nCntRap++;

	OutputDebugString(("周回" + std::to_string(m_nCntRap) + "\n").c_str());
}

int Player::GetRap(void)
{
	return m_nCntRap;
}

int Player::GetLastHitRoadPoly(void)
{
	return m_nLastHitRoadPoly;
}

void Player::SetLastHitRoadPoly(int nCntPoly)
{
	m_nLastHitRoadPoly = nCntPoly;
}

void Player::TimerStart(void)
{
	m_bIsActiveTimer = true;
	m_nFrameTimer = 0;
}

void Player::TimerStop(void)
{
	m_bIsActiveTimer = false;
}

int Player::GetTimerFrame(void)
{
	return m_nFrameTimer;
}

bool Player::IsTimerActive(void)
{
	return m_bIsActiveTimer;
}

void Player::SetAccelKind(AccelKind kind)
{
	m_accelKind = kind;

	switch (kind)
	{
	case AccelKind::Road:
		m_fAccelLen = 0.5f;
		m_fMaxSpeed = 0.3f;
		break;
	case AccelKind::OutSide:
		m_fAccelLen = 0.01f;
		m_fMaxSpeed = 0.03f;
		break;
	case AccelKind::SpeedUpItem:
		m_fAccelLen = 0.75f;
		m_fMaxSpeed = 0.45f;
		m_nCntSpeedUpFrame = 0;
		break;
	default:
		m_fAccelLen = 0.0f;
		m_fMaxSpeed = 0.0f;
		break;
	}
}

Player::AccelKind Player::GetAccelKind(void)
{
	return m_accelKind;
}

DirectX::XMFLOAT3 Player::GetSpeed(void)
{
	return m_speed;
}

// ギアシステムの更新
void Player::UpdateGearSystem()
{
	// エンジン回転数を速度とギア比から計算
	float speedFactor = DirectX::XMVectorGetX(DirectX::XMVector3Length(XMLoadFloat3(&m_speed))) * 300.0f;
	m_engineRPM = speedFactor * GEAR_RATIOS[m_currentGear - 1];

	// ギアアップ
	if (m_engineRPM > SHIFT_UP_RPM && m_currentGear < 6 && !m_bClutchPressed)
	{
		m_bClutchPressed = true;
		m_currentGear++;
		m_engineRPM *= 0.8f; // ギアチェンジ時に少しRPMを下げる
	}

	// ギアダウン
	if (m_engineRPM < SHIFT_DOWN_RPM && m_currentGear > 1 && !m_bClutchPressed)
	{
		m_bClutchPressed = true;
		m_currentGear--;
		m_engineRPM *= 1.2f; // ギアダウン時に少しRPMを上げる
	}

	// クラッチ解除（適当なフレーム後に解除）
	if (m_bClutchPressed)
	{
		static int clutchFrame = 0;
		clutchFrame++;
		if (clutchFrame > 30) // 15フレーム後に解除
		{
			m_bClutchPressed = false;
			clutchFrame = 0;
		}
	}
}