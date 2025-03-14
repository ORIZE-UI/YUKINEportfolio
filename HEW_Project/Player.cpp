#include "Player.h"
#include "Input.h"
#include"Gamepad.h"
#include "Model.h"
#include <chrono>

InputManager imanagerP = InputManager();

DirectX::XMFLOAT3 MinBound = DirectX::XMFLOAT3(-0.1f, -0.1f, -0.1f);  //境界の最小値
DirectX::XMFLOAT3 MaxBound = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);     //最大値

DirectX::XMFLOAT3 HMinBound = DirectX::XMFLOAT3(-0.1f, -0.1f, -0.1f);  //境界の最小値
DirectX::XMFLOAT3 HMaxBound = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);     //最大値

std::chrono::steady_clock::time_point lastSoundPlayTimePly;
const std::chrono::milliseconds soundInterval = std::chrono::milliseconds(1000);//再生時間三秒の時


#define Max_X (2.23f)
#define Min_X (-2.23f)

#define Max_Z (3.36f)
#define Min_Z (1.6f)

#define POS_GET 1

Player::Player()
	: m_pos(0.0f, 0.0f, 3.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	, m_pSVSEPly(nullptr)//スピーカ
	, m_pSDSEPly(nullptr)//サウンドデータ
	, ok (false)
	, m_pVS(nullptr)
	, m_anime_Levitation(NULL)
	, m_anime_possession(NULL)
{
	m_pModel = new Model;
	 //モデルの読み込み処理
	if (!m_pModel->Load("Assets/Model/Player/kuroko.fbx", 0.1f, Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
	}

	// モデルにShaderListからVS,PSを読み込む
	m_pPS= new PixelShader();
	m_pPS = ShaderList::GetPS(ShaderList::PS_TOON);
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	m_pModel->SetPixelShader(m_pPS);

	m_pVS = new VertexShader();

	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}

//	m_pModel->SetVertexShader(m_pVS);

	m_anime_Levitation = m_pModel->AddAnimation("Assets/Animation/kuroko_huyu.fbx");	//	ファイルパスを入れる
	if (FAILED(m_pModel->AddAnimation("Assets/Animation/kuroko_huyu.fbx")))
	{
		MessageBox(nullptr, "anime", "Error", MB_OK);
	}
	m_anime_possession = m_pModel->AddAnimation("Assets/Animation/kuroko_hyoui.fbx");


	minBound = DirectX::XMFLOAT3(-0.1f, -0.1f, -0.1f);
	maxBound = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);

	hminBound = DirectX::XMFLOAT3(-0.1f, -0.1f, -0.1f);
	hmaxBound = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);

	m_pSDSEPly= LoadSound("Assets/Sound/SE/yuurei idouonn_Arai_1.wav");
}

Player::~Player()
{
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
	if (m_pVS)
	{
		delete m_pVS;
		m_pVS = nullptr;
	}
}

void Player::Update(float tick)
{
	//憑依解除時に憑依した時の位置にプレイヤーを戻すため
	if (ok == false)
	{
		m_oldPos = m_pos;
	}
	float moveSpeed = 0.015f; // 移動速度の調整

	float rotationSpeed = 1.0f;

	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSoundPlayTimePly);

	m_pModel->Step(tick);

	if (ok == false)
	{
		m_pModel->Play(m_anime_Levitation, true);	// 浮遊アニメーション(常時)
	}
	else if ( ok == true && !m_pModel->IsPlay(m_anime_possession) )
	{
		m_pModel->Play(m_anime_possession, false);
	}

	//else
	//{
	//	m_pModel->Play(m_anime_possession, false);	// ループ無しアニメーション
	//}

	//m_pCamera->Update();

	//ゲームパッドの対応
	imanagerP.addKeycode(0, 0, GAMEPAD_KEYTYPE::ThumbLL, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(1, 0, GAMEPAD_KEYTYPE::ThumbLR, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(2, 0, GAMEPAD_KEYTYPE::ThumbLU, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(3, 0, GAMEPAD_KEYTYPE::ThumbLD, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(4, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);
	imanagerP.addKeycode(5, 0, GAMEPAD_KEYTYPE::LTrigger, XINPUT_GAMEPAD_LEFT_SHOULDER);
	imanagerP.addKeycode(6, 0, GAMEPAD_KEYTYPE::RTrigger, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	imanagerP.inspect();

	//// 左スティックのX軸とY軸方向の入力を取得
	float leftStickX1 = static_cast<float>(imanagerP.getKey(0));
	float leftStickX2 = static_cast<float>(imanagerP.getKey(1));
	float leftStickZ1 = static_cast<float>(imanagerP.getKey(2));
	float leftStickZ2 = static_cast<float>(imanagerP.getKey(3));



	//// 移動方向ベクトルを計算
	DirectX::XMFLOAT3 moveDirection = DirectX::XMFLOAT3(leftStickX1 - leftStickX2, 0.0f, leftStickZ1 - leftStickZ2);

	 //移動方向ベクトルを正規化
	DirectX::XMVECTOR directionVector = DirectX::XMVectorSet(moveDirection.x, 0.0f, moveDirection.z, 0.0f);
	directionVector = DirectX::XMVector3Normalize(directionVector);
	DirectX::XMFLOAT3 normalizedDirection;
	DirectX::XMStoreFloat3(&normalizedDirection, directionVector);

	// 移動方向ベクトルから回転角度を計算
	float rotationAngle = atan2(normalizedDirection.x, normalizedDirection.z);
	m_rotationY = rotationAngle;

	// 位置を更新

	if (ok == false)
	{
		m_pos.x += moveSpeed * moveDirection.x;
		m_pos.z += moveSpeed * moveDirection.z;


		if ((imanagerP.getKey(0) & 0b011) || (imanagerP.getKey(1) & 0b011) ||
			(imanagerP.getKey(2) & 0b011) || (imanagerP.getKey(3) & 0b011))
		{
			if (elapsedTime >= soundInterval)
			{
				m_pSVSEPly = PlaySound(m_pSDSEPly);

				// 最後のサウンド再生時間を更新
				lastSoundPlayTimePly = currentTime;
			}
		}
		else
		{
			if (m_pSVSEPly)
			{
				m_pSVSEPly->Stop();
			}
		}

		// 回転行列を更新
		m_rotationMatrix = DirectX::XMMatrixRotationY(m_rotationY);




		if (moveDirection.x == 0.0f && moveDirection.z == 0.0f)
		{
			// 最後に向いていた方向を使用
			m_rotationY = m_lastFacingDirection;
		}
		else
		{
			// 移動方向ベクトルから回転角度を計算
			float rotationAngle = atan2(normalizedDirection.x, normalizedDirection.z);
			m_rotationY = rotationAngle;
			// 最後に向いた方向を更新
			m_lastFacingDirection = m_rotationY;
		}

		if (imanagerP.getKey(5) & 0b011)
		{
			m_pos.y -= moveSpeed * 1.0f;
		}
		if (imanagerP.getKey(6) & 0b011)
		{
			m_pos.y += moveSpeed * 1.0f;
		}

	}

	//非憑依時プレイヤー移動
	//if (ok == false)
	//{

	//	if (IsKeyPress('W'))
	//	{
	//		m_pos.z -= moveSpeed;
	//		if (elapsedTime >= soundInterval)
	//		{
	//			m_pSVSEPly = PlaySound(m_pSDSEPly);

	//			// 最後のサウンド再生時間を更新
	//			lastSoundPlayTimePly = currentTime;
	//		}
	//	}
	//	if ( IsKeyPress('S'))
	//	{
	//		m_pos.z += moveSpeed;
	//		if (elapsedTime >= soundInterval)
	//		{
	//			m_pSVSEPly = PlaySound(m_pSDSEPly);

	//			// 最後のサウンド再生時間を更新
	//			lastSoundPlayTimePly = currentTime;
	//		}
	//	}
	//	if ( IsKeyPress('D'))
	//	{
	//		m_pos.x -= moveSpeed;
	//		if (elapsedTime >= soundInterval)
	//		{
	//			m_pSVSEPly = PlaySound(m_pSDSEPly);

	//			// 最後のサウンド再生時間を更新
	//			lastSoundPlayTimePly = currentTime;
	//		}
	//	}
	//	if ( IsKeyPress('A'))
	//	{
	//		m_pos.x += moveSpeed;
	//		if (elapsedTime >= soundInterval)
	//		{
	//			m_pSVSEPly = PlaySound(m_pSDSEPly);

	//			// 最後のサウンド再生時間を更新
	//			lastSoundPlayTimePly = currentTime;
	//		}
	//	}
	//	if (IsKeyPress(VK_SHIFT))
	//	{
	//		m_pos.y -= moveSpeed;
	//	}
	//	if (IsKeyPress(VK_SPACE))
	//	{
	//		m_pos.y += moveSpeed;
	//	}
	//}

		SetBounds(MinBound, MaxBound);  //最小値と最大値をセット
		HSetBounds(HMinBound, HMaxBound);

	
		if (m_pos.x >= Max_X || m_pos.x <= Min_X
			|| m_pos.z >= Max_Z || m_pos.z <= Min_Z
			|| m_pos.y >= 4.0f)
		{
			PlayerPos();
		}
}

void Player::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::XMFLOAT4X4 mat[3];

	// ワールド行列の計算に回転を追加
	DirectX::XMMATRIX MT = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	DirectX::XMMATRIX MR = DirectX::XMMatrixRotationY(m_rotationY); // Y軸回転
	DirectX::XMMATRIX MS = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX world = MS  * MR * MT;//MR*  // 回転を適用//


	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);
	mat[1] = viewMatrix;
	mat[2] = projectionMatrix;

	 //行列をシェーダーへ設定
	m_pVS->WriteBuffer(0, mat);
//	m_pModel->Draw();

	ShaderList::SetWVP(mat);	// 転置済みの変換行列

	m_pModel->Draw(nullptr, [this](int index)	// ラムダ式
	{
		const Model::Mesh* pMesh = m_pModel->GetMesh(index);
		const Model::Material* pMaterial = m_pModel->GetMaterial(pMesh->materialID);
		ShaderList::SetMaterial(*pMaterial);

		DirectX::XMFLOAT4X4 bones[200];
		for (int i = 0; i < pMesh->bones.size() && i < 200; ++i)
		{
			// この計算はゲームつくろー「スキンメッシュの仕組み」が参考になる
			DirectX::XMStoreFloat4x4(&bones[i], DirectX::XMMatrixTranspose(
				pMesh->bones[i].invOffset *
				m_pModel->GetBone(pMesh->bones[i].index)
			));
		}
		ShaderList::SetBones(bones);
	});
#ifdef _DEBUG
	//m_pModel->DrawBone();

#endif // DEBUG

}

void Player::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 Player::GetminBox()
{
	return minBound;
}

DirectX::XMFLOAT3 Player::GetmaxBox()
{
	return maxBound;
}


DirectX::XMFLOAT3 Player::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	//posに最小値、最大値を足して当たり判定をずらす
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}



//憑依当たり判定
void Player::HSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	hminBound = HAdd(m_pos, min);
	hmaxBound = HAdd(m_pos, max);
}

DirectX::XMFLOAT3 Player::HGetminBox()
{
	return hminBound;
}

DirectX::XMFLOAT3 Player::HGetmaxBox()
{
	return hmaxBound;
}


DirectX::XMFLOAT3 Player::HAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	//posに最小値、最大値を足して当たり判定をずらす
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//__


//ブロックとプレイヤー衝突時、プレイヤーの位置を返す
void Player::PlayerPos()
{
	m_pos = m_oldPos;
}

//憑依時に飛ばす
void Player::HPlayerPos()
{
	// 12/29 pos.y = -100.0f → -0.3f
	m_pos.y = -3.0f;
}

//リセット用（未実装）
void Player::RPlayerPos()
{
	m_pos.z = 0.0f;
	m_pos.x = 0.0f;

}

//
bool Player::Set()
{
	return true;
}
void Player::SetOk()
{
	ok = true;
}
void Player::SetNOk()
{
	ok = false;
}

float Player::GetPosX()
{
	return m_oldPos.x;
}

float Player::GetPosY()
{
	return m_oldPos.y+1.2f;
}

float Player::GetPosZ()
{
	return m_oldPos.z;
}

/**
 * @brief アニメーションの設定
 * @param[in] なし
 * @return なし
 */
void Player::SetAnime()
{
	// 入れるアニメーションのセット

}

/**
 * @brief アニメーションの設定
 * @param[in] なし
 * @return なし
 */
void Player::SetAnime2()
{
	m_pModel->Play(m_anime_possession, false);	// 憑依の描画
}
