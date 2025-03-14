#include "Stair.h"
#include "Geometory.h"
#include "Input.h"
#include"Gamepad.h"
#include <chrono>

//minbound maxboundをメンバ変数に
//create部分にスケールと当たり判定をかけ合わせる処理を追加　yは追加で計算必要


InputManager imanagerSt = InputManager();

//DirectX::XMFLOAT3 StairMinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);//プレイヤーとの当たり判定用
//DirectX::XMFLOAT3 StairMaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
//
//DirectX::XMFLOAT3 hStairMinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);//憑依用
//DirectX::XMFLOAT3 hStairMaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
//
//DirectX::XMFLOAT3 cStairMinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);//ブロック同士用
//DirectX::XMFLOAT3 cStairMaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);

#define Max_X (2.23f)
#define Min_X (-2.23f)

#define Max_Z (3.36f)
#define Min_Z (1.8f)



std::chrono::steady_clock::time_point lastSoundPlayTimeStair;
const std::chrono::milliseconds soundIntervalStair = std::chrono::milliseconds(1000);//再生時間三秒の時
Stair::Stair()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_mmovespeed(0.0f, 0.0f, 0.0f)
	, m_scale(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_jmp(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	, moveok(false)
	, StairMinBound(-0.5f, -0.5f, -0.5f)//当たり判定用
	, StairMaxBound(0.5f, 0.5f, 0.5f)
	, hStairMinBound(-0.5f, -0.5f, -0.5f)
	, hStairMaxBound(0.5f, 0.5f, 0.5f)
	, cStairMinBound(-0.5f, -0.5f, -0.5f)
	, cStairMaxBound(0.5f, 0.5f, 0.5f)
	, ok(true)
	, gravity(false)
	, colgravity(true)
	, xz(false)
	, StairTop(false)
	, m_reverse(false)
	, m_pSVSEBlk(nullptr)
	, m_pSDSEBlk(nullptr)
{
	m_pStairModelL = new Model;
	m_pStairModelR = new Model;

	m_pStairModelHL = new Model;
	m_pStairModelHR = new Model;


	if (!m_pStairModelL->Load("Assets/Model/Block/SlopeL.fbx", Model::Flip::XFlip))/*BoxS.fbx*/ {
		MessageBox(NULL, "モデルの読み込みエラー,スロープL", "Error", MB_OK);
	}
	if (!m_pStairModelR->Load("Assets/Model/Block/SlopeL.fbx", Model::Flip::XFlip))/*BoxS.fbx*/ {
		MessageBox(NULL, "モデルの読み込みエラー,スロープR", "Error", MB_OK);
	}
	if (!m_pStairModelHL->Load("Assets/Model/Block/HyouiSlopeL.fbx", Model::Flip::XFlip))/*BoxS.fbx*/ {
		MessageBox(NULL, "モデルの読み込みエラー,スロープHL", "Error", MB_OK);
	}
	if (!m_pStairModelHR->Load("Assets/Model/Block/HyouiSlopeL.fbx", Model::Flip::XFlip))/*BoxS.fbx*/ {
		MessageBox(NULL, "モデルの読み込みエラー,スロープHR", "Error", MB_OK);
	}


	ExtractSlopeVertexCoordinates(*m_pStairModelL);
	ExtractSlopeVertexCoordinates(*m_pStairModelR);
	ExtractSlopeVertexCoordinates(*m_pStairModelHL);
	ExtractSlopeVertexCoordinates(*m_pStairModelHR);


	m_pStairVS = new VertexShader();
	if (FAILED(m_pStairVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pStairModelL->SetVertexShader(m_pStairVS);
	m_pStairModelR->SetVertexShader(m_pStairVS);
	m_pStairModelHL->SetVertexShader(m_pStairVS);
	m_pStairModelHR->SetVertexShader(m_pStairVS);

	SetBounds(minBound, maxBound);
	HSetBounds(hminBound, hmaxBound);
	CSetBounds(cminBound, cmaxBound);

	m_pSDSEBlk = LoadSound("Assets/Sound/SE/Blockgaugokuoto_Oobayashi.wav");

	if (m_reverse == false)
	{
		points = {
			{minBound.x, maxBound.y, minBound.z},  //右上
			{minBound.x, maxBound.y, maxBound.z},
			{maxBound.x, minBound.y, minBound.z},  //左下
			{maxBound.x, minBound.y, maxBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.5f), minBound.y, minBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.5f), minBound.y, maxBound.z},
			{minBound.x, minBound.y, minBound.z},  //右下
			{minBound.x, minBound.y, maxBound.z},
			{minBound.x, minBound.y + (1.0f * m_scale.x * 0.5f), minBound.z},
			{minBound.x, minBound.y + (1.0f * m_scale.x * 0.5f), maxBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.75f), minBound.y + (1.0f * m_scale.x * 0.75f), minBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.75f), minBound.y + (1.0f * m_scale.x * 0.75f), maxBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.5f), minBound.y + (1.0f * m_scale.x * 0.5f), minBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.5f), minBound.y + (1.0f * m_scale.x * 0.5f), maxBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.25f), minBound.y + (1.0f * m_scale.x * 0.25f), minBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.25f), minBound.y + (1.0f * m_scale.x * 0.25f), maxBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.15f), minBound.y + (1.0f * m_scale.x * 0.15f), minBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.15f), minBound.y + (1.0f * m_scale.x * 0.75f), maxBound.z},
		};
	}
	else if (m_reverse == true)
	{
		points = {
			{maxBound.x, maxBound.y, minBound.z},  //左上
			{maxBound.x, maxBound.y, maxBound.z},
			{maxBound.x, minBound.y, minBound.z},  //左下
			{maxBound.x, minBound.y, maxBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.5f), minBound.y, minBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.5f), minBound.y, maxBound.z},
			{minBound.x, minBound.y, minBound.z},  //右下
			{minBound.x, minBound.y, maxBound.z},
			{maxBound.x, minBound.y + (1.0f * m_scale.x * 0.5f), minBound.z},
			{maxBound.x, minBound.y + (1.0f * m_scale.x * 0.5f), maxBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.75f), minBound.y + (1.0f * m_scale.x * 0.75f), minBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.75f), minBound.y + (1.0f * m_scale.x * 0.75f), maxBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.5f), minBound.y + (1.0f * m_scale.x * 0.5f), minBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.5f), minBound.y + (1.0f * m_scale.x * 0.5f), maxBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.25f), minBound.y + (1.0f * m_scale.x * 0.25f), minBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.25f), minBound.y + (1.0f * m_scale.x * 0.25f), maxBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.15f), minBound.y + (1.0f * m_scale.x * 0.15f), minBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.15f), minBound.y + (1.0f * m_scale.x * 0.75f), maxBound.z},
		};
	}
}

Stair::~Stair()
{
	if (m_pStairModelR)
	{
		delete m_pStairModelR;
		m_pStairModelR = nullptr;
	}
	if (m_pStairModelHR)
	{
		delete m_pStairModelHR;
		m_pStairModelHR = nullptr;
	}
	if (m_pStairModelHL)
	{
		delete m_pStairModelHL;
		m_pStairModelHL = nullptr;
	}
	if (m_pStairModelL)
	{
		delete m_pStairModelL;
		m_pStairModelL = nullptr;
	}
	if (m_pStairVS)
	{
		delete m_pStairVS;
		m_pStairVS = nullptr;
	}
	//m_pSVSEBlk->Stop();
}

void Stair::Update()
{

	m_oldPos = m_pos;
	if (colgravity == true)
	{
		m_pos.y -= 0.05f;
	}
	float moveSpeed = 0.003f; // 移動速度の調整
	float rotationSpeed = 10.0f;

	imanagerSt.addKeycode(0, 0, GAMEPAD_KEYTYPE::ThumbLL, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerSt.addKeycode(1, 0, GAMEPAD_KEYTYPE::ThumbLR, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerSt.addKeycode(2, 0, GAMEPAD_KEYTYPE::ThumbLU, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerSt.addKeycode(3, 0, GAMEPAD_KEYTYPE::ThumbLD, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerSt.addKeycode(4, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_A);

	imanagerSt.inspect();



	//// 左スティックのX軸とY軸方向の入力を取得
	float leftStickX1 = static_cast<float>(imanagerSt.getKey(0));
	float leftStickX2 = static_cast<float>(imanagerSt.getKey(1));
	float leftStickZ1 = static_cast<float>(imanagerSt.getKey(2));
	float leftStickZ2 = static_cast<float>(imanagerSt.getKey(3));



	// 移動方向ベクトルを計算
	DirectX::XMFLOAT3 moveDirection = DirectX::XMFLOAT3(leftStickX1 - leftStickX2, 0.0f, leftStickZ1 - leftStickZ2);

	// 移動方向ベクトルを正規化（長さが1になるように）
	DirectX::XMVECTOR directionVector = DirectX::XMVectorSet(moveDirection.x, 0.0f, moveDirection.z, 0.0f);
	directionVector = DirectX::XMVector3Normalize(directionVector);
	DirectX::XMFLOAT3 normalizedDirection;
	DirectX::XMStoreFloat3(&normalizedDirection, directionVector);

	// 移動方向ベクトルから回転角度を計算
	float rotationAngle = atan2(normalizedDirection.x, normalizedDirection.z);
	m_rotationY = rotationAngle;

	if (moveok == true)//憑依時
	{
		m_pos.x += moveSpeed * moveDirection.x;
		m_pos.z += moveSpeed * moveDirection.z;
	}

	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSoundPlayTimeStair);

	//m_jmp = m_pos;

	if (m_pos.y <= 0.0f)
	{
		//ok = false;
		gravity = false;
	}
	//三角形憑依時の移動
	if (moveok == true)
	{
		if (moveok == true)//憑依時
		{
			if ((imanagerSt.getKey(0) & 0b011) || (imanagerSt.getKey(1) & 0b011) ||
				(imanagerSt.getKey(2) & 0b011) || (imanagerSt.getKey(3) & 0b011))
			{
				if (m_pos.y <= 0.1f)
				{
					if (elapsedTime >= soundIntervalStair)
					{
						m_pSVSEBlk = PlaySound(m_pSDSEBlk);

						// 最後のサウンド再生時間を更新
						lastSoundPlayTimeStair = currentTime;
					}
				}
				xz = true;
			}
			else
			{
				xz = false;
			}


			if (ok == false)
			{
				if (imanagerSt.getKey(4) & 0b011)
				{
					frame -= moveSpeed * 0.01;
					// スペースキーが押されたら上昇を実行.ゲージを減少
				   //m_pos.y += 0.07f;
					m_pos.y += frame * 0.001f;

					if (m_pos.y > 2.5f)
					{
						m_pos.y = m_oldPos.y;
					}
				}
				if (frame <= 0.0f || !(imanagerSt.getKey(4) & 0b011))
				{
					m_pos.y -= 0.05f;
					gravity = true;
				}
				if (m_pos.y <= 0.0f&&frame <= 0)
				{
					frame = 25;
				}



			}
			else if (ok == true)
			{
				m_pos.y -= 0.05f;
				gravity = true;
				//m_jmp = m_pos;
			}

			
		}




		










		//if (IsKeyPress('W'))
		//{
		//	m_pos.z -= moveSpeed;

		//	if (m_pos.y <= 0.1f)
		//	{
		//		if (elapsedTime >= soundIntervalStair)
		//		{
		//			m_pSVSEBlk = PlaySound(m_pSDSEBlk);

		//			// 最後のサウンド再生時間を更新
		//			lastSoundPlayTimeStair = currentTime;
		//		}
		//	}
		//	xz = true;
		//}
		//else if ( IsKeyPress('S'))
		//{
		//	m_pos.z += moveSpeed;
		//	if (m_pos.y <= 0.1f)
		//	{
		//		if (elapsedTime >= soundIntervalStair)
		//		{
		//			m_pSVSEBlk = PlaySound(m_pSDSEBlk);

		//			// 最後のサウンド再生時間を更新
		//			lastSoundPlayTimeStair = currentTime;
		//		}
		//	}
		//	xz = true;
		//}
		//else if ( IsKeyPress('D'))
		//{
		//	m_pos.x -= moveSpeed;
		//	if (m_pos.y <= 0.1f)
		//	{
		//		if (elapsedTime >= soundIntervalStair)
		//		{
		//			m_pSVSEBlk = PlaySound(m_pSDSEBlk);

		//			// 最後のサウンド再生時間を更新
		//			lastSoundPlayTimeStair = currentTime;
		//		}
		//	}
		//	xz = true;
		//}
		//else if ( IsKeyPress('A'))
		//{
		//	m_pos.x += moveSpeed;
		//	if (m_pos.y <= 0.1f)
		//	{
		//		if (elapsedTime >= soundIntervalStair)
		//		{
		//			m_pSVSEBlk = PlaySound(m_pSDSEBlk);

		//			// 最後のサウンド再生時間を更新
		//			lastSoundPlayTimeStair = currentTime;
		//		}
		//	}
		//	xz = true;
		//}
		//else
		//{
		//	xz = false;
		//}

	



		//if (ok == false)
		//{

		//	if (IsKeyPress(VK_SPACE))
		//	{
		//		frame -= moveSpeed * 0.01;
		//		// スペースキーが押されたら上昇を実行.ゲージを減少
		//	   //m_pos.y += 0.07f;
		//		m_pos.y += frame * 0.001f;

		//		if (m_pos.y > 2.5f)
		//		{
		//			m_pos.y = m_oldPos.y;
		//		}
		//	}
		//	if (frame <= 0.0f || !(IsKeyPress(VK_SPACE)))
		//	{
		//		m_pos.y -= 0.05f;
		//		gravity = true;
		//	}
		//	if (m_pos.y <= 0.0f&&frame <= 0)
		//	{
		//		frame = 30;
		//	}

		//}
		//else if (ok == true)
		//{
		//	m_pos.y -= 0.05f;
		//	gravity = true;
		//	//m_jmp = m_pos;
		//}
	}



	if (m_pos.x >= Max_X || m_pos.x <= Min_X
		|| m_pos.z >= Max_Z || m_pos.z <= Min_Z
		|| m_pos.y >= 4.0f)
	{

		OBJPos();
	}

	/*if (m_pos.y <= 0.0f)
	{
		ok = false;
		gravity = false;
	}*/

	if (m_pos.y <= 0.0f)
	{
		SetF1();
		OBJPosy();
		//m_pos.y = 0.001f;
		//m_pos.y = 0.0f;
		gravity = false;
	}

	//points = {
	//{-0.5f, 0.0f, 3.0f},
	//{ 0.0f, 0.0f, 3.0f},
	//{ 0.5f, 0.0f, 3.0f},
	//{ 0.5f, 0.5f, 3.0f},
	//{ 0.5f, 0.8f, 3.0f},
	//{ 0.0f, 0.5f, 3.0f},
	//// Add more points as needed
	//};
	if (m_reverse == false)
	{
		points = {
			{minBound.x, maxBound.y, minBound.z},  //右上
			{minBound.x, maxBound.y, maxBound.z},
			{maxBound.x, minBound.y, minBound.z},  //左下
			{maxBound.x, minBound.y, maxBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.5f), minBound.y, minBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.5f), minBound.y, maxBound.z},
			{minBound.x, minBound.y, minBound.z},  //右下
			{minBound.x, minBound.y, maxBound.z},
			{minBound.x, minBound.y + (1.0f * m_scale.x * 0.5f), minBound.z},
			{minBound.x, minBound.y + (1.0f * m_scale.x * 0.5f), maxBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.75f), minBound.y + (1.0f * m_scale.x * 0.75f), minBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.75f), minBound.y + (1.0f * m_scale.x * 0.75f), maxBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.5f), minBound.y + (1.0f * m_scale.x * 0.5f), minBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.5f), minBound.y + (1.0f * m_scale.x * 0.5f), maxBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.25f), minBound.y + (1.0f * m_scale.x * 0.25f), minBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.25f), minBound.y + (1.0f * m_scale.x * 0.25f), maxBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.15f), minBound.y + (1.0f * m_scale.x * 0.15f), minBound.z},
			{maxBound.x - (1.0f * m_scale.x * 0.15f), minBound.y + (1.0f * m_scale.x * 0.75f), maxBound.z},
		};
	}
	else if (m_reverse == true)
	{
		points = {
			{maxBound.x, maxBound.y, minBound.z},  //左上
			{maxBound.x, maxBound.y, maxBound.z},
			{maxBound.x, minBound.y, minBound.z},  //左下
			{maxBound.x, minBound.y, maxBound.z},
			{maxBound.x + (1.0f * m_scale.x * 0.5f), minBound.y, minBound.z},
			{maxBound.x + (1.0f * m_scale.x * 0.5f), minBound.y, maxBound.z},
			{minBound.x, minBound.y, minBound.z},  //右下
			{minBound.x, minBound.y, maxBound.z},
			{maxBound.x, minBound.y - (1.0f * m_scale.x * 0.5f), minBound.z},
			{maxBound.x, minBound.y - (1.0f * m_scale.x * 0.5f), maxBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.75f), minBound.y - (1.0f * m_scale.x * 0.75f), minBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.75f), minBound.y - (1.0f * m_scale.x * 0.75f), maxBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.5f), minBound.y - (1.0f * m_scale.x * 0.5f), minBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.5f), minBound.y - (1.0f * m_scale.x * 0.5f), maxBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.25f), minBound.y - (1.0f * m_scale.x * 0.25f), minBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.25f), minBound.y - (1.0f * m_scale.x * 0.25f), maxBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.15f), minBound.y - (1.0f * m_scale.x * 0.15f), minBound.z},
			{minBound.x - (1.0f * m_scale.x * 0.15f), minBound.y - (1.0f * m_scale.x * 0.75f), maxBound.z},
		};
	
	}
	SetBounds(StairMinBound, StairMaxBound);  //最小値と最大値をセット
	HSetBounds(hStairMinBound, hStairMaxBound);//憑依用の当たり判定
	CSetBounds(cStairMinBound, cStairMaxBound);//ブロック同士の当たり判定


}
void Stair::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::XMFLOAT4X4 mat[3];

	DirectX::XMMATRIX MoT = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	DirectX::XMMATRIX MoS = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	DirectX::XMMATRIX world = MoS * MoT;
	//world = [ワールド行列の設定];
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	mat[1] = viewMatrix; // 与えられた viewMatrix を使う
	mat[2] = projectionMatrix; // 与えられた projectionMatrix を使う

	m_pStairVS->WriteBuffer(0, mat);    //配列の先頭アドレスを指定して、まとめて変換行列を渡す
	if (moveok==true)
	{
		if (m_reverse == false)
		{
			m_pStairModelHL->Draw();
		}
		else
		{
			m_pStairModelHR->Draw();
		}
	}
	if (moveok==false)
	{
		if (m_reverse == false)
		{
			m_pStairModelL->Draw();

		}
		else
		{
			m_pStairModelR->Draw();
		}

	}
	
	

}


void Stair::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 Stair::GetminBounds()
{
	return minBound;
}

DirectX::XMFLOAT3 Stair::GetmaxBounds()
{
	return maxBound;
}

DirectX::XMFLOAT3 Stair::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_


//憑依当たり判定
void Stair::HSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	hminBound = HAdd(m_pos, min);
	hmaxBound = HAdd(m_pos, max);
}

DirectX::XMFLOAT3 Stair::HGetminBounds()
{
	return hminBound;
}

DirectX::XMFLOAT3 Stair::HGetmaxBounds()
{
	return hmaxBound;
}

DirectX::XMFLOAT3 Stair::HAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_


//ブロック同士
void Stair::CSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	cminBound = CAdd(m_pos, min);
	cmaxBound = CAdd(m_pos, max);
}

DirectX::XMFLOAT3 Stair::CGetminBounds()
{
	return cminBound;
}

DirectX::XMFLOAT3 Stair::CGetmaxBounds()
{
	return cmaxBound;
}

DirectX::XMFLOAT3 Stair::CAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

DirectX::XMFLOAT3 Stair::GetStairMaxBounds()
{
	return cStairMaxBound;
}

DirectX::XMFLOAT3 Stair::GetPos()
{
	return m_pos;
}

void Stair::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, bool reverse)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;
	m_reverse = reverse;

	//
	if (reverse == false)
	{
		StairMinBound.x *= m_scale.x;
		StairMinBound.y *= m_scale.y;
		StairMinBound.z *= m_scale.z;
		StairMaxBound.x *= m_scale.x;
		StairMaxBound.y *= m_scale.y;
		StairMaxBound.z *= m_scale.z;
	}
	else if (reverse == true)
	{
		StairMinBound.x *= m_scale.x * -1;
		StairMinBound.y *= m_scale.y;
		StairMinBound.z *= m_scale.z * -1;
		StairMaxBound.x *= m_scale.x * -1;
		StairMaxBound.y *= m_scale.y;
		StairMaxBound.z *= m_scale.z * -1;
	}

	if (StairMinBound.y < 0)
	{
		a = StairMinBound.y *= -1;
		StairMaxBound.y += a;

		StairMinBound.y = 0;
	}
	SetBounds(StairMinBound, StairMaxBound);

	if (reverse == false)
	{
		cStairMinBound.x *= m_scale.x;
		cStairMinBound.y *= m_scale.y;
		cStairMinBound.z *= m_scale.z;
		cStairMaxBound.x *= m_scale.x;
		cStairMaxBound.y *= m_scale.y;
		cStairMaxBound.z *= m_scale.z;

		//これがないとy軸の当たり判定おかしくなる

		if (cStairMinBound.y < 0)
		{
			a = cStairMinBound.y *= -1;
			cStairMaxBound.y += a;

			cStairMinBound.y = 0;
		}

		CSetBounds(cStairMinBound, cStairMaxBound);
	}
	else
	{
		cStairMinBound.x *= m_scale.x * -1;
		cStairMinBound.y *= m_scale.y;
		cStairMinBound.z *= m_scale.z * -1;
		cStairMaxBound.x *= m_scale.x * -1;
		cStairMaxBound.y *= m_scale.y;
		cStairMaxBound.z *= m_scale.z * -1;

		//これがないとy軸の当たり判定おかしくなる

		if (cStairMinBound.y < 0)
		{
			a = cStairMinBound.y *= -1;
			cStairMaxBound.y += a;

			cStairMinBound.y = 0;
		}

		CSetBounds(cStairMinBound, cStairMaxBound);
	}

	hStairMinBound.x *= m_scale.x;
	hStairMinBound.y *= m_scale.y;
	hStairMinBound.z *= m_scale.z;
	hStairMaxBound.x *= m_scale.x;
	hStairMaxBound.y *= m_scale.y;
	hStairMaxBound.z *= m_scale.z;

	if (hStairMinBound.y < 0)
	{
		a = hStairMinBound.y *= -1;
		hStairMaxBound.y += a;

		hStairMinBound.y = 0;
	}
	HSetBounds(hStairMinBound, hStairMaxBound);

	m_mmovespeed = m_pos;

}



//憑依判定用
void Stair::Set()
{
	moveok = true;
}

void Stair::SetF()
{
	moveok = false;
}

bool Stair::SetR()
{
	return moveok;
}



//ブロック同士がぶつかった時に返す

void Stair::OBJPos()
{
	m_pos = m_oldPos;
}

void Stair::OBJPosy()
{
	m_pos.y = m_oldPos.y;
}




void Stair::Set1()
{
	ok = true;
}

void Stair::SetF1()
{
	ok = false;
}

bool Stair::SetR1()
{
	return ok;
}

void Stair::SetColgravity()
{
	//if (colgravity == true)
	//{
	//	colgravity = false;
	//}
	//else/* if (colgravity == false)*/
	//{
		colgravity = true;
	//}
}

void Stair::SetColgravityfalse()
{
	colgravity = false;
}

void Stair::SetStairTop()
{
	if (StairTop == true)
	{
		StairTop = false;
	}
	else/* if (colgravity == false)*/
	{
		StairTop = true;
	}
}

void Stair::MoveStair(float y)
{
	m_pos.y = y;
}

void Stair::framepls()
{
	frame = 25;
}


bool Stair::IsGravity()
{
	return gravity;
}


bool Stair::IsXZ()
{
	return xz;
}

bool Stair::IsStairTop()
{
	return StairTop;
}

bool Stair::IsMove()
{
	return moveok;
}

bool Stair::IsReverse()
{
	return m_reverse;
}

void Stair::ExtractSlopeVertexCoordinates(Model& slopeModel)
{
	const Model::Mesh* slopeMesh = slopeModel.GetMesh(0); // Slope.fbxが1つのメッシュしか持たないと仮定

	if (slopeMesh) {
		const std::vector<Model::Vertex>& vertices = slopeMesh->vertices;

		// 頂点座標は 'vertices' ベクターからアクセスできます
		for (const auto& vertex : vertices) {
			DirectX::XMFLOAT3 position = vertex.pos;
			// 頂点座標 (position.x, position.y, position.z) を使って何かを行います
		}
	}
}

void Stair::SetSlope()
{
	m_pos.x += 0.005f;
}

void Stair::SetReverseSlope()
{
	m_pos.x -= 0.005f;
}

void Stair::SetSlopeY(float y)
{
	m_pos.y = y;
}

