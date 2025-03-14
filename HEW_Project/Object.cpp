//#include "Object.h"
//#include "Geometory.h"
//#include "Input.h"
//#include <chrono>
//
////minbound maxboundをメンバ変数に
////create部分にスケールと当たり判定をかけ合わせる処理を追加　yは追加で計算必要
//
//
////InputManager imanagerOB = InputManager();
//
////DirectX::XMFLOAT3 objectMinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);//プレイヤーとの当たり判定用
////DirectX::XMFLOAT3 objectMaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
////
////DirectX::XMFLOAT3 hobjectMinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);//憑依用
////DirectX::XMFLOAT3 hobjectMaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
////
////DirectX::XMFLOAT3 cobjectMinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);//ブロック同士用
////DirectX::XMFLOAT3 cobjectMaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
//
//std::chrono::steady_clock::time_point lastSoundPlayTime;
//const std::chrono::milliseconds soundInterval = std::chrono::milliseconds(3000);//再生時間三秒の時
//
//
//#define Max_X (2.23f)
//#define Min_X (-2.23f)
//
//#define Max_Z (3.36f)
//#define Min_Z (1.8f)
//
//#define MaxPosX (2.0f)
//#define MinPosX (-2.0f)
//
//#define MaxPosZ (3.0f)
//#define MinPosZ (1.5f)
//
//Object::Object()
//	: m_pos(0.0f, 0.0f, 0.0f)
//	, m_mmovespeed(0.0f, 0.0f, 0.0f)
//	, m_scale(0.0f, 0.0f, 0.0f)
//	, m_oldPos(0.0f, 0.0f, 0.0f)
//	, m_jmp(0.0f,0.0f,0.0f)
//	, m_direction(0.0f, 0.0f, 0.0f)
//	, m_rotationMatrix(DirectX::XMMatrixIdentity())
//	, moveok(false)
//	, m_pSVSEBlk(nullptr)//スピーカ
//	,m_pSDSEBlk(nullptr)//サウンドデータ
//	, objectMinBound(-0.5f, -0.5f, -0.5f)//当たり判定用
//	, objectMaxBound(0.5f, 0.5f, 0.5f)
//	, hobjectMinBound(-0.5f, -0.5f, -0.5f)
//	, hobjectMaxBound(0.5f, 0.5f, 0.5f)
//	, cobjectMinBound(-0.5f, -0.5f, -0.5f)
//	, cobjectMaxBound(0.5f, 0.5f, 0.5f)
//	, ok(true)
//	, gravity(false)
//	, xz(false)
//	, colgravity(true)
//	, objectTop(false)
//	, possession(true)
//	, Automove(true)
//	, MoveX(true)
//	, XZMOVE(true)
//{
//	m_pObjectModel = new Model;
//	m_pObjectModelH = new Model;
//	m_pObjectModelMoveX = new Model;
//	m_pObjectModelMoveZ = new Model;
//
//	/*if (!m_pObjectModel->Load("Assets/Model/Block/Slope.fbx",0.1,  Model::Flip::XFlip)) {
//		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
//	}*/
//	if (!m_pObjectModel->Load("Assets/Model/Block/BoxS.fbx", Model::Flip::XFlip)) {
//		MessageBox(NULL, "モデルの読み込みエラー_boxBoxS", "Error", MB_OK);
//	}
//	if (!m_pObjectModelH->Load("Assets/Model/Block/HyouiBox.fbx", Model::Flip::XFlip)) {
//		MessageBox(NULL, "モデルの読み込みエラー_boxHyouiBox", "Error", MB_OK);
//	}
//	if (!m_pObjectModelMoveX->Load("Assets/Model/Block/WaruBox1.fbx", Model::Flip::XFlip)) {
//		MessageBox(NULL, "モデルの読み込みエラー_boxWaruBox1", "Error", MB_OK);
//	}
//	if (!m_pObjectModelMoveZ->Load("Assets/Model/Block/WaruBox2.fbx", Model::Flip::XFlip)) {
//		MessageBox(NULL, "モデルの読み込みエラー_boxWaruBox2", "Error", MB_OK);
//	}
//
//
//
//	/*if (!m_pObjectModel->Load("Assets/Stage/Butai.fbx", Model::Flip::XFlip)) {
//		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
//	}*/
//	m_pObjectVS = new VertexShader();
//	if (FAILED(m_pObjectVS->Load("Assets/Shader/VS_Model.cso")))
//	{
//		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
//	}
//	m_pObjectModel->SetVertexShader(m_pObjectVS);
//	m_pObjectModelH->SetVertexShader(m_pObjectVS);
//	m_pObjectModelMoveX->SetVertexShader(m_pObjectVS);
//	m_pObjectModelMoveZ->SetVertexShader(m_pObjectVS);
//
//
//	SetBounds(minBound, maxBound);
//	HSetBounds(hminBound, hmaxBound);
//	CSetBounds(cminBound, cmaxBound);
//	
//	m_pSDSEBlk = LoadSound("Assets/Sound/SE/Blockgaugokuoto_Oobayashi.wav");
//
//}
//
//Object::~Object()
//{
//	if (m_pObjectModel)
//	{
//		delete m_pObjectModel;
//		m_pObjectModel = nullptr;
//	}
//	if (m_pObjectModelH)
//	{
//		delete m_pObjectModelH;
//		m_pObjectModelH = nullptr;
//	}
//	if (m_pObjectModelMoveX)
//	{
//		delete m_pObjectModelMoveX;
//		m_pObjectModelMoveX = nullptr;
//	}
//	if (m_pObjectModelMoveZ)
//	{
//		delete m_pObjectModelMoveZ;
//		m_pObjectModelMoveZ = nullptr;
//	}
//
//	if (m_pObjectVS)
//	{
//		delete m_pObjectVS;
//		m_pObjectVS = nullptr;
//	}
//	//m_pSVSEBlk->Stop();
//}
//
//void Object::Update()
//{
//
//	m_oldPos = m_pos;
//
//	float moveSpeed = 0.007f; // 移動速度の調整
//	float rotationSpeed = 10.0f;
//
//	if (colgravity == true)
//	{
//		m_pos.y -= 0.005f;
//	}
//
//	if (m_pos.y <= 0.0f)
//	{
//		//ok = false;
//		gravity = false;
//	}
//
//	/*imanagerOB.addKeycode(0, 0, GAMEPAD_KEYTYPE::ThumbLL, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
//	imanagerOB.addKeycode(1, 0, GAMEPAD_KEYTYPE::ThumbLR, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
//	imanagerOB.addKeycode(2, 0, GAMEPAD_KEYTYPE::ThumbLU, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
//	imanagerOB.addKeycode(3, 0, GAMEPAD_KEYTYPE::ThumbLD, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
//	imanagerOB.addKeycode(4, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);
//
//	imanagerOB.inspect();*/
//
//
//
//	//// 左スティックのX軸とY軸方向の入力を取得
//	//float leftStickX1 = static_cast<float>(imanagerOB.getKey(0));
//	//float leftStickX2 = static_cast<float>(imanagerOB.getKey(1));
//	//float leftStickZ1 = static_cast<float>(imanagerOB.getKey(2));
//	//float leftStickZ2 = static_cast<float>(imanagerOB.getKey(3));
//
//
//
//	// 移動方向ベクトルを計算
//	//DirectX::XMFLOAT3 moveDirection = DirectX::XMFLOAT3(leftStickX1 - leftStickX2, 0.0f, leftStickZ1 - leftStickZ2);
//
//	//// 移動方向ベクトルを正規化（長さが1になるように）
//	//DirectX::XMVECTOR directionVector = DirectX::XMVectorSet(moveDirection.x, 0.0f, moveDirection.z, 0.0f);
//	//directionVector = DirectX::XMVector3Normalize(directionVector);
//	//DirectX::XMFLOAT3 normalizedDirection;
//	//DirectX::XMStoreFloat3(&normalizedDirection, directionVector);
//
//	//// 移動方向ベクトルから回転角度を計算
//	//float rotationAngle = atan2(normalizedDirection.x, normalizedDirection.z);
//	//m_rotationY = rotationAngle;
//
//	//if (moveok == true)//憑依時
//	//{
//	//	m_pos.x -= moveSpeed * moveDirection.x;
//	//	m_pos.z -= moveSpeed * moveDirection.z;
//	//}
//
//	auto currentTime = std::chrono::steady_clock::now();
//	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSoundPlayTime);
//
//	//m_jmp = m_pos;
//
//	//憑依時移動
//	if (moveok == true)
//	{
//#if _DEBUG
//		//現在の座標を表示
//		if (IsKeyPress('X'))
//		{
//			char x[256];
//			snprintf(x, sizeof(x), "x座標 %f", m_pos.x);
//			MessageBox(0, x, "憑依中のオブジェクトの座標", MB_OK);
//		}
//		if (IsKeyPress('Y'))
//		{
//			char y[256];
//			snprintf(y, sizeof(y), "y座標 %f", m_pos.y);
//			MessageBox(0, y, "憑依中のオブジェクトの座標", MB_OK);
//		}
//		if (IsKeyPress('Z'))
//		{
//			char z[256];
//			snprintf(z, sizeof(z), "z座標 %f", m_pos.z);
//			MessageBox(0, z, "憑依中のオブジェクトの座標", MB_OK);
//		}
//		if (IsKeyTrigger('C'))
//		{
//			char c[256];
//			sprintf_s(c,"x座標 %f \n y座標 %f\n z座標 %f", m_pos.x, m_pos.y, m_pos.z);
//			MessageBox(0, c, "憑依中のオブジェクトの座標", MB_OK);
//		}
//#endif
//		if (IsKeyPress(VK_UP) || IsKeyPress('W'))
//		{
//			m_pos.z -= moveSpeed;
//
//			if (m_pos.y <= 0.1f)
//			{
//				if (elapsedTime >= soundInterval)
//				{
//					m_pSVSEBlk = PlaySound(m_pSDSEBlk);
//
//					// 最後のサウンド再生時間を更新
//					lastSoundPlayTime = currentTime;
//				}
//			}
//			xz = true;
//		}
//		else if (IsKeyPress(VK_DOWN) || IsKeyPress('S'))
//		{
//			m_pos.z += moveSpeed;
//			if (m_pos.y <= 0.1f)
//			{
//				if (elapsedTime >= soundInterval)
//				{
//					m_pSVSEBlk = PlaySound(m_pSDSEBlk);
//
//					// 最後のサウンド再生時間を更新
//					lastSoundPlayTime = currentTime;
//				}
//			}
//			xz = true;
//		}
//		else if (IsKeyPress(VK_RIGHT) || IsKeyPress('D'))
//		{
//			m_pos.x -= moveSpeed;
//			if (m_pos.y <= 0.1f)
//			{
//				if (elapsedTime >= soundInterval)
//				{
//					m_pSVSEBlk = PlaySound(m_pSDSEBlk);
//
//					// 最後のサウンド再生時間を更新
//					lastSoundPlayTime = currentTime;
//				}
//			}
//			xz = true;
//		}
//		else if (IsKeyPress(VK_LEFT) || IsKeyPress('A'))
//		{
//			m_pos.x += moveSpeed;
//			if (m_pos.y <= 0.1f)
//			{
//				if (elapsedTime >= soundInterval)
//				{
//					m_pSVSEBlk = PlaySound(m_pSDSEBlk);
//
//					// 最後のサウンド再生時間を更新
//					lastSoundPlayTime = currentTime;
//				}
//			}
//			xz = true;
//		}
//		else
//		{
//			xz = false;
//		}
//
//		if (IsKeyPress('U'))
//		{
//			m_pos.y -= moveSpeed;
//		}
//		if (IsKeyPress('I'))
//		{
//			m_pos.y += moveSpeed;
//		}
//
//
//		if (ok == false)
//		{
//			if (IsKeyPress(VK_SPACE))
//			{
//				frame -= moveSpeed * 0.01;
//				// スペースキーが押されたら上昇を実行.ゲージを減少
//			   //m_pos.y += 0.07f;
// 				m_pos.y += frame * 0.001f;
//
//				if (m_pos.y > 2.5f)
//				{
//					m_pos.y = m_oldPos.y;
//				}
//			}
//			if (frame <= 0.0f || !(IsKeyPress(VK_SPACE)))
//			{
//				m_pos.y -= 0.05f;
//				gravity = true;
//			}
//			if (m_pos.y <= 0.0f&&frame <= 0)
//			{
//				frame = 30;
//			}
//
//
//
//		}
//		else if (ok == true)
//		{
//			m_pos.y -= 0.05f;
//			gravity = true;
//			//m_jmp = m_pos;
//		}
//		
//		//if (ok == true)
//		//{
//		//	m_pos.y -= 0.5f;
//		//	gravity = true;
//		//	//m_jmp = m_pos;
//		//}
//	}
//	
//	
//
//		if (m_pos.x >= Max_X || m_pos.x <= Min_X
//			|| m_pos.z >= Max_Z || m_pos.z <=Min_Z
//			|| m_pos.y >= 2.0f)
//		{
//			OBJPos();
//		}
//		if (m_pos.y <= 0.0f)
//		{
//			SetF1();
//			m_pos.y = 0.001f;
//			gravity = false;
//		}
//
//		float amoveSpeed = 0.002f;
//
//		if (Automove == true && possession == false && XZMOVE == true)
//		{
//
//			if (MoveX == true)	// オブジェクトの移動がtrueなら
//			{
//				
//				m_pos.x += amoveSpeed;
//				//X軸
//				
//				if (m_pos.x >= MaxPosX)	// 指定した範囲(右)まで移動
//				{
//					m_pos.x = MaxPosX;
//					MoveX = false;
//				}
//				xz = true;
//			}
//			else
//			{
//				m_pos.x -= amoveSpeed;
//				if (m_pos.x <= MinPosX)	// 指定した範囲(左)まで移動
//				{
//					m_pos.x = MinPosX;
//					MoveX = true;
//				}
//				xz = true;
//			}
//			xz = true;
//		}
//
//		if (Automove == true && possession == false && XZMOVE == false)
//		{
//
//			if (MoveX == true)	// オブジェクトの移動がtrueなら
//			{
//
//				m_pos.z += amoveSpeed;
//
//				//Z軸
//				if (m_pos.z >= MaxPosZ)	//指定した範囲(奥)まで移動
//				{
//					m_pos.z = MaxPosZ;
//					MoveX = false;
//				}
//				xz = true;
//			}
//			else
//			{
//				m_pos.z -= amoveSpeed;//Z軸
//				if (m_pos.z <= MinPosZ)	// 指定した範囲(奥)まで移動
//				{
//					m_pos.z = MinPosZ;
//					MoveX = true;
//				}
//				xz = true;
//			}
//			xz = true;
//		}
//		SetBounds(objectMinBound, objectMaxBound);  //最小値と最大値をセット
//		HSetBounds(hobjectMinBound, hobjectMaxBound);//憑依用の当たり判定
//		CSetBounds(cobjectMinBound, cobjectMaxBound);//ブロック同士の当たり判定
//
//
//}
//void Object::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
//{
//	DirectX::XMFLOAT4X4 mat[3];
//
//	DirectX::XMMATRIX MoT = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
//	DirectX::XMMATRIX MoS = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
//	DirectX::XMMATRIX world = MoS * MoT;
//	//world = [ワールド行列の設定];
//	world = DirectX::XMMatrixTranspose(world);
//	DirectX::XMStoreFloat4x4(&mat[0], world);
//
//	mat[1] = viewMatrix; // 与えられた viewMatrix を使う
//	mat[2] = projectionMatrix; // 与えられた projectionMatrix を使う
//	
//	m_pObjectVS->WriteBuffer(0, mat);    //配列の先頭アドレスを指定して、まとめて変換行列を渡す
//	if (moveok==false&&Automove==false)
//	{
//		m_pObjectModel->Draw();
//		
//	}
//	if (Automove == true && possession == false && XZMOVE==true)
//	{
//		m_pObjectModelMoveX->Draw();
//	}
//	
//	if (Automove == true && possession == false && XZMOVE == false)
//	{
//		m_pObjectModelMoveZ->Draw();
//	}
//
//	if (moveok==true)
//	{
//		m_pObjectModelH->Draw();
//	}
//	
//}
//
//
//void Object::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
//{
//	minBound = Add(m_pos, min);
//	maxBound = Add(m_pos, max);
//}
//
//DirectX::XMFLOAT3 Object::GetminBounds() 
//{
//	return minBound;
//}
//
//DirectX::XMFLOAT3 Object::GetmaxBounds() 
//{
//	return maxBound;
//}
//
//DirectX::XMFLOAT3 Object::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
//{
//	DirectX::XMFLOAT3 result;
//	result.x = a.x + b.x;
//	result.y = a.y + b.y;
//	result.z = a.z + b.z;
//	return result;
//}
////_
//
//
////憑依当たり判定
//void Object::HSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
//{
//	hminBound = HAdd(m_pos, min);
//	hmaxBound = HAdd(m_pos, max);
//}
//
//DirectX::XMFLOAT3 Object::HGetminBounds()
//{
//	return hminBound;
//}
//
//DirectX::XMFLOAT3 Object::HGetmaxBounds()
//{
//	return hmaxBound;
//}
//
//DirectX::XMFLOAT3 Object::HAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
//{
//	DirectX::XMFLOAT3 result;
//	result.x = a.x + b.x;
//	result.y = a.y + b.y;
//	result.z = a.z + b.z;
//	return result;
//}
////_
//
//
////ブロック同士
//void Object::CSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
//{
//	cminBound = CAdd(m_pos, min);
//	cmaxBound = CAdd(m_pos, max);
//}
//
//DirectX::XMFLOAT3 Object::CGetminBounds()
//{
//	return cminBound;
//}
//
//DirectX::XMFLOAT3 Object::CGetmaxBounds()
//{
//	return cmaxBound;
//}
//
//DirectX::XMFLOAT3 Object::CAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
//{
//	DirectX::XMFLOAT3 result;
//	result.x = a.x + b.x;
//	result.y = a.y + b.y;
//	result.z = a.z + b.z;
//	return result;
//}
//
//DirectX::XMFLOAT3 Object::GetObjectMaxBounds()
//{
//	return cobjectMaxBound;
//}
//
//DirectX::XMFLOAT3 Object::GetPos()
//{
//	return m_pos;
//}
//
//void Object::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ,bool hyoui
//,bool Auto, bool XZM)
//{
//	m_pos.x = posX;
//	m_pos.y = posY;
//	m_pos.z = posZ;
//	m_scale.x = scaleX;
//	m_scale.y = scaleY;
//	m_scale.z = scaleZ;
//
//	possession = hyoui;
//
//	Automove = Auto;
//
//	XZMOVE = XZM;
//	//
//	objectMinBound.x *= m_scale.x;
//	objectMinBound.y *= m_scale.y;
//	objectMinBound.z *= m_scale.z;
//	objectMaxBound.x *= m_scale.x;
//	objectMaxBound.y *= m_scale.y;
//	objectMaxBound.z *= m_scale.z;
//
//	if (objectMinBound.y < 0)
//	{
//		a = objectMinBound.y *= -1;
//		objectMaxBound.y += a;
//
//		objectMinBound.y = 0;
//	}
//
//	SetBounds(objectMinBound, objectMaxBound);
//
//	cobjectMinBound.x *= m_scale.x;
//	cobjectMinBound.y *= m_scale.y;
//	cobjectMinBound.z *= m_scale.z;
//	cobjectMaxBound.x *= m_scale.x;
//	cobjectMaxBound.y *= m_scale.y;
//	cobjectMaxBound.z *= m_scale.z;
//
//	//これがないとy軸の当たり判定おかしくなる
//
//	if (cobjectMinBound.y < 0)
//	{
//		a = cobjectMinBound.y *= -1;
//		cobjectMaxBound.y += a;
//
//		cobjectMinBound.y = 0;
//	}
//
//	CSetBounds(cobjectMinBound, cobjectMaxBound);
//
//
//	hobjectMinBound.x *= m_scale.x;
//	hobjectMinBound.y *= m_scale.y;
//	hobjectMinBound.z *= m_scale.z;
//	hobjectMaxBound.x *= m_scale.x;
//	hobjectMaxBound.y *= m_scale.y;
//	hobjectMaxBound.z *= m_scale.z;
//
//
//	if (hobjectMinBound.y < 0)
//	{
//		a = hobjectMinBound.y *= -1;
//		hobjectMaxBound.y += a;
//
//		hobjectMinBound.y = 0;
//	}
//
//	HSetBounds(hobjectMinBound, hobjectMaxBound);
//	
//	m_mmovespeed = m_pos;
//
//}
//
//
//
////憑依判定用
//void Object::Set()
//{
//	moveok = true;
//}
//
//void Object::SetF()
//{
//	moveok = false;
//}
//
//bool Object::SetR()
//{
//	return moveok;
//}
//
//
//
////ブロック同士がぶつかった時に返す
//
//void Object::OBJPos()
//{
//	m_pos = m_oldPos;
//}
//
//void Object::OBJPosy()
//{
//	m_pos.y = m_oldPos.y;
//}
//
//
//
//void Object::Set1()
//{
//	ok = true;
//}
//
//void Object::SetF1()
//{
//	ok = false;
//}
//
//bool Object::SetR1()
//{
//	return ok;
//}
//
//bool Object::IsGravity()
//{
//	return gravity;
//}
//
//bool Object::IsXZ()
//{
//	return xz;
//}
//
//void Object::MoveObject(float y)
//{
//	m_pos.y = y;
//}
//bool Object::IsObjectTop()
//{
//	return objectTop;
//}
//
//bool Object::IsMove()
//{
//	return moveok;
//}
//
//void Object::SetObjectTop()
//{
//	if (objectTop == true)
//	{
//		objectTop = false;
//	}
//	else/* if (colgravity == false)*/
//	{
//		objectTop = true;
//	}
//}
//
//void Object::SetColgravity()
//{
//	colgravity = true;
//
//}
//
//void Object::SetColgravityfalse()
//{
//	colgravity = false;
//}
//
//void Object::framepls()
//{
//	frame=30;
//}
//
//void Object::SetSlope()
//{
//	m_pos.x += 0.0005f;
//	//m_pos.y = m_oldPos.y;
//	//moveok = false;
//}
//
//void Object::SetReverseSlope()
//{
//	m_pos.x -= 0.0005f;
//}
//
//bool Object::possessionok()
//{
//	return possession;
//}
//
//bool Object::IsAutoMove()
//{
//	return Automove;
//}
//
//bool Object::MVX()
//{
//	return MoveX;
//}
//
//void Object::MoveXfalse()
//{
//	MoveX = false;
//}
//
//void Object::MoveXtrue()
//{
//	MoveX = true;
//}

#include "Object.h"
#include "Geometory.h"
#include "Input.h"
#include"Gamepad.h"
#include <chrono>

InputManager imanagerOB = InputManager();

std::chrono::steady_clock::time_point lastSoundPlayTime;
const std::chrono::milliseconds soundInterval = std::chrono::milliseconds(1000);//再生時間三秒の時


#define Max_X (2.23f)
#define Min_X (-2.23f)

#define Max_Z (3.36f)
#define Min_Z (1.8f)

#define MaxPosX (2.0f)
#define MinPosX (-2.0f)

#define MaxPosZ (3.0f)
#define MinPosZ (1.5f)

Object::Object()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_mmovespeed(0.0f, 0.0f, 0.0f)
	, m_scale(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_jmp(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	, moveok(false)
	, m_pSVSEBlk(nullptr)//スピーカ
	, m_pSDSEBlk(nullptr)//サウンドデータ
	, objectMinBound(-0.5f, -0.5f, -0.5f)//当たり判定用
	, objectMaxBound(0.5f, 0.5f, 0.5f)
	, hobjectMinBound(-0.5f, -0.5f, -0.5f)
	, hobjectMaxBound(0.5f, 0.5f, 0.5f)
	, cobjectMinBound(-0.5f, -0.5f, -0.5f)
	, cobjectMaxBound(0.5f, 0.5f, 0.5f)
	, ok(true)
	, gravity(false)
	, xz(false)
	, colgravity(true)
	, objectTop(false)
	, possession(true)
	, Automove(true)
	, MoveX(true)
	, XZMOVE(true)
{
	m_pObjectModel = new Model;
	m_pObjectModelH = new Model;
	m_pObjectModelMoveX = new Model;
	m_pObjectModelMoveZ = new Model;

	/*if (!m_pObjectModel->Load("Assets/Model/Block/Slope.fbx",0.1,  Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
	}*/
	if (!m_pObjectModel->Load("Assets/Model/Block/BoxS.fbx", Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー_boxBoxS", "Error", MB_OK);
	}
	if (!m_pObjectModelH->Load("Assets/Model/Block/HyouiBox.fbx", Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー_boxHyouiBox", "Error", MB_OK);
	}
	if (!m_pObjectModelMoveX->Load("Assets/Model/Block/WaruBox1.fbx", Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー_boxWaruBox1", "Error", MB_OK);
	}
	if (!m_pObjectModelMoveZ->Load("Assets/Model/Block/WaruBox2.fbx", Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー_boxWaruBox2", "Error", MB_OK);
	}



	/*if (!m_pObjectModel->Load("Assets/Stage/Butai.fbx", Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
	}*/
	m_pObjectVS = new VertexShader();
	if (FAILED(m_pObjectVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pObjectModel->SetVertexShader(m_pObjectVS);
	m_pObjectModelH->SetVertexShader(m_pObjectVS);
	m_pObjectModelMoveX->SetVertexShader(m_pObjectVS);
	m_pObjectModelMoveZ->SetVertexShader(m_pObjectVS);


	SetBounds(minBound, maxBound);
	HSetBounds(hminBound, hmaxBound);
	CSetBounds(cminBound, cmaxBound);

	m_pSDSEBlk = LoadSound("Assets/Sound/SE/Blockgaugokuoto_Oobayashi.wav");

}

Object::~Object()
{
	if (m_pObjectModel)
	{
		delete m_pObjectModel;
		m_pObjectModel = nullptr;
	}
	if (m_pObjectModelH)
	{
		delete m_pObjectModelH;
		m_pObjectModelH = nullptr;
	}
	if (m_pObjectModelMoveX)
	{
		delete m_pObjectModelMoveX;
		m_pObjectModelMoveX = nullptr;
	}
	if (m_pObjectModelMoveZ)
	{
		delete m_pObjectModelMoveZ;
		m_pObjectModelMoveZ = nullptr;
	}

	if (m_pObjectVS)
	{
		delete m_pObjectVS;
		m_pObjectVS = nullptr;
	}
	//m_pSVSEBlk->Stop();
}

void Object::Update()
{

	m_oldPos = m_pos;

	float moveSpeed = 0.003f; // 移動速度の調整
	float rotationSpeed = 10.0f;

	if (colgravity == true)
	{
		m_pos.y -= 0.005f;
	}

	if (m_pos.y <= 0.0f)
	{
		//ok = false;
		gravity = false;
	}

	imanagerOB.addKeycode(0, 0, GAMEPAD_KEYTYPE::ThumbLL, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(1, 0, GAMEPAD_KEYTYPE::ThumbLR, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(2, 0, GAMEPAD_KEYTYPE::ThumbLU, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(3, 0, GAMEPAD_KEYTYPE::ThumbLD, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(4, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_A);

	imanagerOB.inspect();



	// 左スティックのX軸とY軸方向の入力を取得
	float leftStickX1 = static_cast<float>(imanagerOB.getKey(0));
	float leftStickX2 = static_cast<float>(imanagerOB.getKey(1));
	float leftStickZ1 = static_cast<float>(imanagerOB.getKey(2));
	float leftStickZ2 = static_cast<float>(imanagerOB.getKey(3));



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
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSoundPlayTime);


	if (moveok == true)//憑依時
	{
		if ((imanagerOB.getKey(0) & 0b011) || (imanagerOB.getKey(1) & 0b011) ||
			(imanagerOB.getKey(2) & 0b011) || (imanagerOB.getKey(3) & 0b011))
		{
			if (m_pos.y <= 0.1f)
			{
				if (elapsedTime >= soundInterval)
				{
					m_pSVSEBlk = PlaySound(m_pSDSEBlk);

					// 最後のサウンド再生時間を更新
					lastSoundPlayTime = currentTime;
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
			if (imanagerOB.getKey(4) & 0b011)
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
			if (frame <= 0.0f || !(imanagerOB.getKey(4) & 0b011))
			{
				m_pos.y -= 0.05f;
				gravity = true;
			}
			if (m_pos.y <= 0.0f&&frame <= 0)
			{
				frame = 30;
			}



		}
		else if (ok == true)
		{
			m_pos.y -= 0.05f;
			gravity = true;
			//m_jmp = m_pos;
		}

		//if (ok == true)
		//{
		//	m_pos.y -= 0.5f;
		//	gravity = true;
		//	//m_jmp = m_pos;
		//}
	}

	
	






	////m_jmp = m_pos;

	////憑依時移動
	//if (moveok == true)
	//{

	//	if ( IsKeyPress('W'))
	//	{
	//		m_pos.z -= moveSpeed;

	//		if (m_pos.y <= 0.1f)
	//		{
	//			if (elapsedTime >= soundInterval)
	//			{
	//				m_pSVSEBlk = PlaySound(m_pSDSEBlk);

	//				// 最後のサウンド再生時間を更新
	//				lastSoundPlayTime = currentTime;
	//			}
	//		}
	//		xz = true;
	//	}
	//	else if ( IsKeyPress('S'))
	//	{
	//		m_pos.z += moveSpeed;
	//		if (m_pos.y <= 0.1f)
	//		{
	//			if (elapsedTime >= soundInterval)
	//			{
	//				m_pSVSEBlk = PlaySound(m_pSDSEBlk);

	//				// 最後のサウンド再生時間を更新
	//				lastSoundPlayTime = currentTime;
	//			}
	//		}
	//		xz = true;
	//	}
	//	else if ( IsKeyPress('D'))
	//	{
	//		m_pos.x -= moveSpeed;
	//		if (m_pos.y <= 0.1f)
	//		{
	//			if (elapsedTime >= soundInterval)
	//			{
	//				m_pSVSEBlk = PlaySound(m_pSDSEBlk);

	//				// 最後のサウンド再生時間を更新
	//				lastSoundPlayTime = currentTime;
	//			}
	//		}
	//		xz = true;
	//	}
	//	else if ( IsKeyPress('A'))
	//	{
	//		m_pos.x += moveSpeed;
	//		if (m_pos.y <= 0.1f)
	//		{
	//			if (elapsedTime >= soundInterval)
	//			{
	//				m_pSVSEBlk = PlaySound(m_pSDSEBlk);

	//				// 最後のサウンド再生時間を更新
	//				lastSoundPlayTime = currentTime;
	//			}
	//		}
	//		xz = true;
	//	}
	//	else
	//	{
	//		xz = false;
	//	}

	//	


	//	if (ok == false)
	//	{
	//		if (IsKeyPress(VK_SPACE))
	//		{
	//			frame -= moveSpeed * 0.01;
	//			// スペースキーが押されたら上昇を実行.ゲージを減少
	//		   //m_pos.y += 0.07f;
	//			m_pos.y += frame * 0.001f;

	//			if (m_pos.y > 2.5f)
	//			{
	//				m_pos.y = m_oldPos.y;
	//			}
	//		}
	//		if (frame <= 0.0f || !(IsKeyPress(VK_SPACE)))
	//		{
	//			m_pos.y -= 0.05f;
	//			gravity = true;
	//		}
	//		if (m_pos.y <= 0.0f&&frame <= 0)
	//		{
	//			frame = 30;
	//		}



	//	}
	//	else if (ok == true)
	//	{
	//		m_pos.y -= 0.05f;
	//		gravity = true;
	//		//m_jmp = m_pos;
	//	}

	//	//if (ok == true)
	//	//{
	//	//	m_pos.y -= 0.5f;
	//	//	gravity = true;
	//	//	//m_jmp = m_pos;
	//	//}
	//}



	if (m_pos.x >= Max_X || m_pos.x <= Min_X
		|| m_pos.z >= Max_Z || m_pos.z <= Min_Z
		|| m_pos.y >= 2.0f)
	{
		OBJPos();
	}
	if (m_pos.y <= 0.0f)
	{
		SetF1();
		m_pos.y = 0.001f;
		gravity = false;
	}

	float amoveSpeed = 0.002f;

	if (Automove == true && possession == false && XZMOVE == true)
	{

		if (MoveX == true)	// オブジェクトの移動がtrueなら
		{

			m_pos.x += amoveSpeed;
			//X軸

			if (m_pos.x >= MaxPosX)	// 指定した範囲(右)まで移動
			{
				m_pos.x = MaxPosX;
				MoveX = false;
			}
			xz = true;
		}
		else
		{
			m_pos.x -= amoveSpeed;
			if (m_pos.x <= MinPosX)	// 指定した範囲(左)まで移動
			{
				m_pos.x = MinPosX;
				MoveX = true;
			}
			xz = true;
		}
		xz = true;
	}

	if (Automove == true && possession == false && XZMOVE == false)
	{

		if (MoveX == true)	// オブジェクトの移動がtrueなら
		{

			m_pos.z += amoveSpeed;

			//Z軸
			if (m_pos.z >= MaxPosZ)	//指定した範囲(奥)まで移動
			{
				m_pos.z = MaxPosZ;
				MoveX = false;
			}
			xz = true;
		}
		else
		{
			m_pos.z -= amoveSpeed;//Z軸
			if (m_pos.z <= MinPosZ)	// 指定した範囲(奥)まで移動
			{
				m_pos.z = MinPosZ;
				MoveX = true;
			}
			xz = true;
		}
		xz = true;
	}
	SetBounds(objectMinBound, objectMaxBound);  //最小値と最大値をセット
	HSetBounds(hobjectMinBound, hobjectMaxBound);//憑依用の当たり判定
	CSetBounds(cobjectMinBound, cobjectMaxBound);//ブロック同士の当たり判定


}
void Object::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
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

	m_pObjectVS->WriteBuffer(0, mat);    //配列の先頭アドレスを指定して、まとめて変換行列を渡す
	if (moveok == false && Automove == false)
	{
		m_pObjectModel->Draw();

	}
	if (Automove == true && possession == false && XZMOVE == true)
	{
		m_pObjectModelMoveX->Draw();
	}

	if (Automove == true && possession == false && XZMOVE == false)
	{
		m_pObjectModelMoveZ->Draw();
	}

	if (moveok == true)
	{
		m_pObjectModelH->Draw();
	}

}


void Object::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 Object::GetminBounds()
{
	return minBound;
}

DirectX::XMFLOAT3 Object::GetmaxBounds()
{
	return maxBound;
}

DirectX::XMFLOAT3 Object::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_


//憑依当たり判定
void Object::HSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	hminBound = HAdd(m_pos, min);
	hmaxBound = HAdd(m_pos, max);
}

DirectX::XMFLOAT3 Object::HGetminBounds()
{
	return hminBound;
}

DirectX::XMFLOAT3 Object::HGetmaxBounds()
{
	return hmaxBound;
}

DirectX::XMFLOAT3 Object::HAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_


//ブロック同士
void Object::CSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	cminBound = CAdd(m_pos, min);
	cmaxBound = CAdd(m_pos, max);
}

DirectX::XMFLOAT3 Object::CGetminBounds()
{
	return cminBound;
}

DirectX::XMFLOAT3 Object::CGetmaxBounds()
{
	return cmaxBound;
}

DirectX::XMFLOAT3 Object::CAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

DirectX::XMFLOAT3 Object::GetObjectMaxBounds()
{
	return cobjectMaxBound;
}

DirectX::XMFLOAT3 Object::GetPos()
{
	return m_pos;
}

void Object::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, bool hyoui
	, bool Auto, bool XZM)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;

	possession = hyoui;

	Automove = Auto;

	XZMOVE = XZM;
	//
	objectMinBound.x *= m_scale.x;
	objectMinBound.y *= m_scale.y;
	objectMinBound.z *= m_scale.z;
	objectMaxBound.x *= m_scale.x;
	objectMaxBound.y *= m_scale.y;
	objectMaxBound.z *= m_scale.z;

	if (objectMinBound.y < 0)
	{
		a = objectMinBound.y *= -1;
		objectMaxBound.y += a;

		objectMinBound.y = 0;
	}

	SetBounds(objectMinBound, objectMaxBound);

	cobjectMinBound.x *= m_scale.x;
	cobjectMinBound.y *= m_scale.y;
	cobjectMinBound.z *= m_scale.z;
	cobjectMaxBound.x *= m_scale.x;
	cobjectMaxBound.y *= m_scale.y;
	cobjectMaxBound.z *= m_scale.z;

	//これがないとy軸の当たり判定おかしくなる

	if (cobjectMinBound.y < 0)
	{
		a = cobjectMinBound.y *= -1;
		cobjectMaxBound.y += a;

		cobjectMinBound.y = 0;
	}

	CSetBounds(cobjectMinBound, cobjectMaxBound);


	hobjectMinBound.x *= m_scale.x;
	hobjectMinBound.y *= m_scale.y;
	hobjectMinBound.z *= m_scale.z;
	hobjectMaxBound.x *= m_scale.x;
	hobjectMaxBound.y *= m_scale.y;
	hobjectMaxBound.z *= m_scale.z;


	if (hobjectMinBound.y < 0)
	{
		a = hobjectMinBound.y *= -1;
		hobjectMaxBound.y += a;

		hobjectMinBound.y = 0;
	}

	HSetBounds(hobjectMinBound, hobjectMaxBound);

	m_mmovespeed = m_pos;

}



//憑依判定用
void Object::Set()
{
	moveok = true;
}

void Object::SetF()
{
	moveok = false;
}

bool Object::SetR()
{
	return moveok;
}



//ブロック同士がぶつかった時に返す

void Object::OBJPos()
{
	m_pos = m_oldPos;
}

void Object::OBJPosy()
{
	m_pos.y = m_oldPos.y;
}



void Object::Set1()
{
	ok = true;
}

void Object::SetF1()
{
	ok = false;
}

bool Object::SetR1()
{
	return ok;
}

bool Object::IsGravity()
{
	return gravity;
}

bool Object::IsXZ()
{
	return xz;
}

void Object::MoveObject(float y)
{
	m_pos.y = y;
}
bool Object::IsObjectTop()
{
	return objectTop;
}

bool Object::IsMove()
{
	return moveok;
}

void Object::SetObjectTop()
{
	if (objectTop == true)
	{
		objectTop = false;
	}
	else/* if (colgravity == false)*/
	{
		objectTop = true;
	}
}

void Object::SetColgravity()
{
	colgravity = true;

}

void Object::SetColgravityfalse()
{
	colgravity = false;
}

void Object::framepls()
{
	frame = 25;
}

void Object::SetSlope()
{
	m_pos.x += 0.0005f;
	//m_pos.y = m_oldPos.y;
	//moveok = false;
}

void Object::SetReverseSlope()
{
	m_pos.x -= 0.0005f;
}

bool Object::possessionok()
{
	return possession;
}

bool Object::IsAutoMove()
{
	return Automove;
}

bool Object::MVX()
{
	return MoveX;
}

bool Object::IsOnBox()
{
	return OnBox;
}

void Object::MoveXfalse()
{
	MoveX = false;
}

void Object::MoveXtrue()
{
	MoveX = true;
}

void Object::OnBoxfalse()
{
	OnBox = false;
}

void Object::OnBoxtrue()
{
	OnBox = true;
}
