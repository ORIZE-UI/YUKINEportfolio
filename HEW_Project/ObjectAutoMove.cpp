#include "ObjectAutoMove.h"
#include "Geometory.h"
#include "Input.h"

//InputManager imanagerOB = InputManager();

//DirectX::XMFLOAT3 objectMinBoundAuto = DirectX::XMFLOAT3(-0.1f, -0.5f, -0.1f);//プレイヤーとの当たり判定用
//DirectX::XMFLOAT3 objectMaxBoundAuto = DirectX::XMFLOAT3(0.2f, 0.5f, -0.05f);

//DirectX::XMFLOAT3 hobjectMinBoundAuto = DirectX::XMFLOAT3(-0.25f, -0.5f, -0.1f);//憑依用
//DirectX::XMFLOAT3 hobjectMaxBoundAuto = DirectX::XMFLOAT3(0.25f, 0.5f, 0.35f);

//DirectX::XMFLOAT3 cobjectMinBoundAuto = DirectX::XMFLOAT3(-0.3f, -0.5f, -0.3f);//ブロック同士用
//DirectX::XMFLOAT3 cobjectMaxBoundAuto = DirectX::XMFLOAT3(0.3f, 0.5f, 0.5f);



ObjectAuto::ObjectAuto()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_scale(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	, moveok(false)
	, m_MaxPosX(1.5f)
	, m_MinPosX(-1.5f)
	, m_MoveX(false)
	, m_isPossessed(true)
{

	m_pObjectModel = new Model;

	if (!m_pObjectModel->Load("Assets/Model/Block/test_black_cube_tex_plus.fbx", 0.05f, Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
	}
	m_pObjectVS = new VertexShader();
	if (FAILED(m_pObjectVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pObjectModel->SetVertexShader(m_pObjectVS);

//	SetBounds(minBoundAuto, maxBoundAuto);
//	HSetBounds(hminBound, hmaxBound);
//	CSetBounds(cminBoundAuto, cmaxBoundAuto);

}

ObjectAuto::~ObjectAuto()
{
	if (m_pObjectModel)
	{
		delete m_pObjectModel;
		m_pObjectModel = nullptr;
	}
	if (m_pObjectVS)
	{
		delete m_pObjectVS;
		m_pObjectVS = nullptr;
	}
}

void ObjectAuto::Update()
{
	m_oldPos = m_pos;

	if (m_MoveX == true)	// オブジェクトの移動がtrueなら
	{
		m_pos.x += 0.01f;
		if (m_pos.x >= m_MaxPosX)	// 指定した範囲(右)まで移動
		{
			m_pos.x = m_MaxPosX;
			m_MoveX = false;
		}
	}
	if (m_MoveX == false)
	{
		m_pos.x -= 0.01f;
		if (m_pos.x <= m_MinPosX)	// 指定した範囲(左)まで移動
		{
			m_pos.x = m_MinPosX;
			m_MoveX = true;
		}
	}

	// bool objmove = true;
	//if (objmove = true)		// オブジェクトが右に動く
	//{
	//	m_pos.x += 0.01f;
	//}
	//if (objmove = false)	// オブジェクトが左に動く
	//{
	//	m_pos.x -= 0.01f;
	//}


	/*imanagerOB.addKeycode(0, 0, GAMEPAD_KEYTYPE::ThumbLL, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(1, 0, GAMEPAD_KEYTYPE::ThumbLR, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(2, 0, GAMEPAD_KEYTYPE::ThumbLU, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(3, 0, GAMEPAD_KEYTYPE::ThumbLD, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(4, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);

	imanagerOB.inspect();*/

	float moveSpeed = 0.03f; // 移動速度の調整

	float rotationSpeed = 10.0f;

	//// 左スティックのX軸とY軸方向の入力を取得
	//float leftStickX1 = static_cast<float>(imanagerOB.getKey(0));
	//float leftStickX2 = static_cast<float>(imanagerOB.getKey(1));
	//float leftStickZ1 = static_cast<float>(imanagerOB.getKey(2));
	//float leftStickZ2 = static_cast<float>(imanagerOB.getKey(3));

	// 移動方向ベクトルを計算
	//DirectX::XMFLOAT3 moveDirection = DirectX::XMFLOAT3(leftStickX1 - leftStickX2, 0.0f, leftStickZ1 - leftStickZ2);

	//// 移動方向ベクトルを正規化（長さが1になるように）
	//DirectX::XMVECTOR directionVector = DirectX::XMVectorSet(moveDirection.x, 0.0f, moveDirection.z, 0.0f);
	//directionVector = DirectX::XMVector3Normalize(directionVector);
	//DirectX::XMFLOAT3 normalizedDirection;
	//DirectX::XMStoreFloat3(&normalizedDirection, directionVector);

	//// 移動方向ベクトルから回転角度を計算
	//float rotationAngle = atan2(normalizedDirection.x, normalizedDirection.z);
	//m_rotationY = rotationAngle;

	//if (moveok == true)//憑依時
	//{
	//	m_pos.x -= moveSpeed * moveDirection.x;
	//	m_pos.z -= moveSpeed * moveDirection.z;
	//}

		if (moveok == true)
	{
			

		if (IsKeyPress(VK_UP))
		{
			m_pos.z += moveSpeed;
		}
		if (IsKeyPress(VK_DOWN))
		{
			m_pos.z -= moveSpeed;
		}
		if (IsKeyPress(VK_RIGHT))
		{
			m_pos.x += moveSpeed;
		}
		if (IsKeyPress(VK_LEFT))
		{
			m_pos.x -= moveSpeed;
		}
	}

//	SetBounds(objectMinBoundAuto, objectMaxBoundAuto);  //最小値と最大値をセット

//	HSetBounds(hobjectMinBoundAuto, hobjectMaxBoundAuto);//憑依用の当たり判定
//	CSetBounds(cobjectMinBoundAuto, cobjectMaxBoundAuto);//ブロック同士の当たり判定

//	time++;
}

void ObjectAuto::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
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
	m_pObjectModel->Draw();

}

//
//void ObjectAutoMove::SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
//{
//	minBoundAuto = Add(m_pos, min);
//	maxBoundAuto = Add(m_pos, max);
//}
//
//DirectX::XMFLOAT3 ObjectAutoMove::GetminBounds()
//{
//	return minBoundAuto;
//}
//
//DirectX::XMFLOAT3 ObjectAutoMove::GetmaxBounds()
//{
//	return maxBoundAuto;
//}

DirectX::XMFLOAT3 ObjectAuto::Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_


//憑依当たり判定
//void ObjectAutoMove::HSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
//{
//	hminBound = HAdd(m_pos, min);
//	hmaxBound = HAdd(m_pos, max);
//}
//DirectX::XMFLOAT3 ObjectAutoMove::HGetminBounds()
//{
//	return hminBound;
//}
//
//DirectX::XMFLOAT3 ObjectAutoMove::HGetmaxBounds()
//{
//	return hmaxBound;
//}
//
//DirectX::XMFLOAT3 ObjectAutoMove::HAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
//{
//	DirectX::XMFLOAT3 result;
//	result.x = a.x + b.x;
//	result.y = a.y + b.y;
//	result.z = a.z + b.z;
//	return result;
//}


////ブロック同士
//void ObjectAutoMove::CSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
//{
//	cminBoundAuto = CAdd(m_pos, min);
//	cmaxBoundAuto = CAdd(m_pos, max);
//}
//
//DirectX::XMFLOAT3 ObjectAutoMove::CGetminBounds()
//{
//	return cminBoundAuto;
//}
//
//DirectX::XMFLOAT3 ObjectAutoMove::CGetmaxBounds()
//{
//	return cmaxBoundAuto;
//}

DirectX::XMFLOAT3 ObjectAuto::CAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

void ObjectAuto::CreateAuto(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;
}

//憑依判定用
void ObjectAuto::Set()
{
	moveok = true;
}

void ObjectAuto::SetF()
{
	moveok = false;
}

bool ObjectAuto::SetR()
{
	return moveok;
}

//ブロック同士がぶつかった時に返す

void ObjectAuto::OBJPos()
{
	m_pos = m_oldPos;
}

void ObjectAuto::Modelchg()
{
	if (m_pObjectModel->Load("Assets/Model/test_model/test_block.fbx", 0.05f, Model::Flip::XFlip));
}

void ObjectAuto::Modelchg2()
{
	//if (m_pObjectModel->Load("Assets/Model/Block/test_black_cube_tex_plus.fbx", 0.05f, Model::Flip::XFlip));
	if (m_pObjectModel->Load("Assets/Model/Block/BoxS.fbx", 0.05f, Model::Flip::XFlip));
}

