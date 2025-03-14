#include "Lift_Obj.hpp"
#include "Geometory.h"
#include "Input.h"
#include"Gamepad.h"
#include "GameObject.h"

InputManager imanagerLf = InputManager();


// プレイヤーとの当たり判定用
//DirectX::XMFLOAT3 liftobj_MinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
//DirectX::XMFLOAT3 liftobj_MaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);

// ブロック同士の当たり判定用
//DirectX::XMFLOAT3 c_liftobj_MinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
//DirectX::XMFLOAT3 c_liftobj_MaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);

Lift_Obj::Lift_Obj()
	: m_pos(0.0f, 0.0f, 0.0f), m_oldPos(0.0f, 0.0f, 0.0f)
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_rotY(0.0f), m_lastRotY(0.0f)
	, m_direction(0.0f, 0.0f, 1.0f)
	, m_rotMat(DirectX::XMMatrixIdentity())
	, moveOk(false)
	, m_pModel(nullptr), m_pVS(nullptr)
	, m_heightPosY(0.0f), m_lowPosY(0.0f)
	, m_moveSpeed(0.0f)
	, m_RiseFlag(false)
	, m_move(false)
	, m_LiftUp(false)
	, liftobj_MinBound(-0.5f, -0.5f, -0.5f)//当たり判定用
	, liftobj_MaxBound(0.5f, 0.5f, 0.5f)
	, c_liftobj_MinBound(-0.5f, -0.5f, -0.5f)
	, c_liftobj_MaxBound(0.5f, 0.5f, 0.5f)

{
	m_pModel = new Model();
	
	if (!m_pModel->Load("Assets/Model/Block/TuriyukaM (1).fbx", Model::Flip::XFlip))
	{
		MessageBox(nullptr, "Liftモデルの読み込みに失敗しました_リフト", "エラー", MB_OK | MB_ICONWARNING);
	}
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "Lift頂点シェーダーの読み込みに失敗しました", "エラー", MB_OK | MB_ICONWARNING);
	}
	m_pModel->SetVertexShader(m_pVS);


	// それぞれの当たり判定の大きさを設定
	//SetBounds(liftobj_MinBound, liftobj_MaxBound);
	SetBounds(minBound, maxBound);
	CSetBounds(cminBound, cmaxBound);

}

Lift_Obj::~Lift_Obj()
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

void Lift_Obj::Update()
{
	m_oldPos = m_pos; // 前の位置を保存

	imanagerLf.addKeycode(0, 0, GAMEPAD_KEYTYPE::LTrigger, XINPUT_GAMEPAD_LEFT_SHOULDER);
	imanagerLf.addKeycode(1, 0, GAMEPAD_KEYTYPE::RTrigger, XINPUT_GAMEPAD_RIGHT_SHOULDER);

	imanagerLf.inspect();

	// 上昇フラグが`true`の場合
	//if (m_RiseFlag)
	//{
	//	// 最高点に到達していない場合

	//	m_pos.y += m_moveSpeed * 0.01f; // 上昇させる
	//	// 最高点に到達した場合
	//	if (m_pos.y >= m_heightPosY)
	//	{
	//		m_pos.y = m_heightPosY; // 最高点に設定
	//		m_RiseFlag = true; // 上昇フラグを`false`に設定
	//	}

	//}
	//if (!m_RiseFlag)// 上昇フラグが`false`の場合

	//{
	//	m_pos.y -= m_moveSpeed * 0.01f; // 下降させる

	//		// 最低点に到達した場合
	//	if (m_pos.y <= m_lowPosY)
	//	{
	//		m_pos.y = m_lowPosY; // 最低点に設定
	//		m_RiseFlag = false; // 上昇フラグを`true`に設定
	//	}

	//}

	if (m_move == true)
	{
		if (imanagerLf.getKey(1) & 0b011)
		{
			// 最高点に到達していない場合
			m_LiftUp = true;
			m_pos.y += m_moveSpeed * 0.01f; // 上昇させる
			// 最高点に到達した場合
			if (m_pos.y >= m_heightPosY)
			{
				m_pos.y = m_heightPosY; // 最高点に設定
				//m_RiseFlag = true; // 上昇フラグを`false`に設定
			}

		}
		if (imanagerLf.getKey(0) & 0b011)

		{
			m_LiftUp = false;
			m_pos.y -= m_moveSpeed * 0.01f; // 下降させる

				// 最低点に到達した場合
			if (m_pos.y <= m_lowPosY)
			{
				m_pos.y = m_lowPosY; // 最低点に設定
				//m_RiseFlag = false; // 上昇フラグを`true`に設定
			}

		}
	}
	

	SetBounds(liftobj_MinBound, liftobj_MaxBound);
	CSetBounds(c_liftobj_MinBound, c_liftobj_MaxBound);
}

void Lift_Obj::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::XMFLOAT4X4 mat[3];

	// ワールド行列の設定
	DirectX::XMMATRIX world = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z)
	);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	mat[1] = viewMatrix; // 引数の `viewMatrix` を利用
	mat[2] = projectionMatrix; // 引数の `projectionMatrix` を利用

	m_pVS->WriteBuffer(0, mat);
	m_pModel->Draw();
}

void Lift_Obj::SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 Lift_Obj::GetminBounds()
{
	return liftobj_MinBound;
}

DirectX::XMFLOAT3 Lift_Obj::GetmaxBounds()
{
	return liftobj_MaxBound;
}

DirectX::XMFLOAT3 Lift_Obj::Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}

void Lift_Obj::CSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
{
	cminBound = CAdd(m_pos, min);
	cmaxBound = CAdd(m_pos, max);
}

DirectX::XMFLOAT3 Lift_Obj::CGetminBounds()
{
	return c_liftobj_MinBound;
}

DirectX::XMFLOAT3 Lift_Obj::CGetmaxBounds()
{
	return c_liftobj_MaxBound;
}

DirectX::XMFLOAT3 Lift_Obj::CAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}

void Lift_Obj::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ
	, float liftheight, float liftlow, float liftspeed)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;

	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;


	liftobj_MinBound.x *= m_scale.x;
	liftobj_MinBound.y *= m_scale.y;
	liftobj_MinBound.z *= m_scale.z;
	liftobj_MaxBound.x *= m_scale.x;
	liftobj_MaxBound.y *= m_scale.y;
	liftobj_MaxBound.z *= m_scale.z;

	SetBounds(liftobj_MinBound, liftobj_MaxBound);

	c_liftobj_MinBound.x *= m_scale.x;
	c_liftobj_MinBound.y *= m_scale.y;
	c_liftobj_MinBound.z *= m_scale.z;
	c_liftobj_MaxBound.x *= m_scale.x;
	c_liftobj_MaxBound.y *= m_scale.y;
	c_liftobj_MaxBound.z *= m_scale.z;

	//これがないとy軸の当たり判定おかしくなる

	if (c_liftobj_MinBound.y < 0)
	{
		a = c_liftobj_MinBound.y *= -1;
		c_liftobj_MaxBound.y += a;

		c_liftobj_MinBound.y = 0;
	}
	c_liftobj_MinBound.y = c_liftobj_MaxBound.y * -1;
	CSetBounds(c_liftobj_MinBound, c_liftobj_MaxBound);

	m_heightPosY = liftheight;
	m_lowPosY = liftlow;
	m_moveSpeed = liftspeed;

}

// ブロックとリフトが当たった時に返す
void Lift_Obj::LiftObj_SetPos()
{
	//m_pos = m_oldPos;
}

/**
 * @brief 最高点の位置を設定する
 * @param height  最高点の位置
 */
void Lift_Obj::SetHeightPosY(float height)
{
	m_heightPosY = height;
}

/**
 * @brief 最低点の位置を設定する
 * @param low 最低点の位置
 */
void Lift_Obj::SetLowPosY(float low)
{
	m_lowPosY = low;
}

/**
 * @brief 移動速度を設定する
 * @param speed 移動速度
 */
void Lift_Obj::SetSpeed(float speed)
{
	m_moveSpeed = speed;
}



void Lift_Obj::SetLever()
{
	m_RiseFlag = true;
}

void Lift_Obj::SetFLever()
{
	m_RiseFlag = false;
}

bool Lift_Obj::SetRLever()
{
	return m_RiseFlag;
}





void Lift_Obj::SetMoveTrue()
{
	m_move = true;
}

void Lift_Obj::SetMoveFalse()
{
	m_move = false;
}

bool Lift_Obj::IsMove()
{
	return m_move;
}

bool Lift_Obj::LiftUp()
{
	return m_LiftUp;
}

float Lift_Obj::LiftMoveSpeed()
{
	return m_pos.y;
}

void Lift_Obj::SetPos()
{
	m_pos = m_oldPos;
}

void Lift_Obj::MoveLift(float y)
{
	m_pos.y = y;
}



//#include "Lift_Obj.hpp"
//#include "Geometory.h"
//#include "Input.h"
//#include "GameObject.h"
//
//
//// プレイヤーとの当たり判定用
////DirectX::XMFLOAT3 liftobj_MinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
////DirectX::XMFLOAT3 liftobj_MaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
//
//// ブロック同士の当たり判定用
////DirectX::XMFLOAT3 c_liftobj_MinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
////DirectX::XMFLOAT3 c_liftobj_MaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
//
//Lift_Obj::Lift_Obj()
//	: m_pos(0.0f, 0.0f, 0.0f), m_oldPos(0.0f, 0.0f, 0.0f)
//	, m_scale(1.0f, 1.0f, 1.0f)
//	, m_rotY(0.0f), m_lastRotY(0.0f)
//	, m_direction(0.0f, 0.0f, 1.0f)
//	, m_rotMat(DirectX::XMMatrixIdentity())
//	, moveOk(false)
//	, m_pModel(nullptr), m_pVS(nullptr)
//	, m_heightPosY(0.0f), m_lowPosY(0.0f)
//	, m_moveSpeed(0.0f)
//	, m_RiseFlag(false)
//	, m_move(false)
//	, m_LiftUp(false)
//	, liftobj_MinBound(-0.5f, -0.5f, -0.5f)//当たり判定用
//	, liftobj_MaxBound(0.5f, 0.5f, 0.5f)
//	, c_liftobj_MinBound(-0.5f, -0.5f, -0.5f)
//	, c_liftobj_MaxBound(0.5f, 0.5f, 0.5f)
//
//{
//	m_pModel = new Model();
//	if (!m_pModel->Load("Assets/Model/Block/BoxS.fbx", Model::Flip::XFlip))
//	{
//		MessageBox(nullptr, "Liftモデルの読み込みに失敗しました‗リフト", "エラー", MB_OK | MB_ICONWARNING);
//	}
//	m_pVS = new VertexShader();
//	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
//	{
//		MessageBox(nullptr, "Lift頂点シェーダーの読み込みに失敗しました", "エラー", MB_OK | MB_ICONWARNING);
//	}
//	m_pModel->SetVertexShader(m_pVS);
//
//
//	// それぞれの当たり判定の大きさを設定
//	//SetBounds(liftobj_MinBound, liftobj_MaxBound);
//	SetBounds(minBound, maxBound);
//	CSetBounds(cminBound, cmaxBound);
//
//}
//
//Lift_Obj::~Lift_Obj()
//{
//	if (m_pModel)
//	{
//		delete m_pModel;
//		m_pModel = nullptr;
//	}
//	if (m_pVS)
//	{
//		delete m_pVS;
//		m_pVS = nullptr;
//	}
//}
//
//void Lift_Obj::Update()
//{
//	//m_oldPos = m_pos; // 前の位置を保存
//
//	// 上昇フラグが`true`の場合
//	//if (m_RiseFlag)
//	//{
//	//	// 最高点に到達していない場合
//
//	//	m_pos.y += m_moveSpeed * 0.01f; // 上昇させる
//	//	// 最高点に到達した場合
//	//	if (m_pos.y >= m_heightPosY)
//	//	{
//	//		m_pos.y = m_heightPosY; // 最高点に設定
//	//		m_RiseFlag = true; // 上昇フラグを`false`に設定
//	//	}
//
//	//}
//	//if (!m_RiseFlag)// 上昇フラグが`false`の場合
//
//	//{
//	//	m_pos.y -= m_moveSpeed * 0.01f; // 下降させる
//
//	//		// 最低点に到達した場合
//	//	if (m_pos.y <= m_lowPosY)
//	//	{
//	//		m_pos.y = m_lowPosY; // 最低点に設定
//	//		m_RiseFlag = false; // 上昇フラグを`true`に設定
//	//	}
//
//	//}
//
//	if (m_move == true)
//	{
//		if (IsKeyPress(VK_UP))
//		{
//			// 最高点に到達していない場合
//			m_LiftUp = true;
//			m_pos.y += m_moveSpeed * 0.01f; // 上昇させる
//			// 最高点に到達した場合
//			if (m_pos.y >= m_heightPosY)
//			{
//				m_pos.y = m_heightPosY; // 最高点に設定
//				//m_RiseFlag = true; // 上昇フラグを`false`に設定
//			}
//
//		}
//		if (IsKeyPress(VK_DOWN))
//
//		{
//			m_LiftUp = false;
//			m_pos.y -= m_moveSpeed * 0.01f; // 下降させる
//
//				// 最低点に到達した場合
//			if (m_pos.y <= m_lowPosY)
//			{
//				m_pos.y = m_lowPosY; // 最低点に設定
//				//m_RiseFlag = false; // 上昇フラグを`true`に設定
//			}
//
//		}
//	}
//	/*if (IsKeyPress('1'))
//	{
//		m_RiseFlag = true;
//
//	}
//	if (IsKeyPress('2'))
//	{
//		m_RiseFlag = false;
//	}*/
//
//	SetBounds(liftobj_MinBound, liftobj_MaxBound);
//	CSetBounds(c_liftobj_MinBound, c_liftobj_MaxBound);
//}
//
//void Lift_Obj::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
//{
//	DirectX::XMFLOAT4X4 mat[3];
//
//	// ワールド行列の設定
//	DirectX::XMMATRIX world = DirectX::XMMatrixTranspose(
//		DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
//		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z)
//	);
//	DirectX::XMStoreFloat4x4(&mat[0], world);
//
//	mat[1] = viewMatrix; // 引数の `viewMatrix` を利用
//	mat[2] = projectionMatrix; // 引数の `projectionMatrix` を利用
//
//	m_pVS->WriteBuffer(0, mat);
//	m_pModel->Draw();
//}
//
//void Lift_Obj::SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
//{
//	minBound = Add(m_pos, min);
//	maxBound = Add(m_pos, max);
//}
//
//DirectX::XMFLOAT3 Lift_Obj::GetminBounds()
//{
//	return liftobj_MinBound;
//}
//
//DirectX::XMFLOAT3 Lift_Obj::GetmaxBounds()
//{
//	return liftobj_MaxBound;
//}
//
//DirectX::XMFLOAT3 Lift_Obj::Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
//{
//	DirectX::XMFLOAT3 result;
//	result.x = a.x + b.x;
//	result.y = a.y + b.y;
//	result.z = a.z + b.z;
//
//	return result;
//}
//
//void Lift_Obj::CSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
//{
//	cminBound = CAdd(m_pos, min);
//	cmaxBound = CAdd(m_pos, max);
//}
//
//DirectX::XMFLOAT3 Lift_Obj::CGetminBounds()
//{
//	return c_liftobj_MinBound;
//}
//
//DirectX::XMFLOAT3 Lift_Obj::CGetmaxBounds()
//{
//	return c_liftobj_MaxBound;
//}
//
//DirectX::XMFLOAT3 Lift_Obj::CAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
//{
//	DirectX::XMFLOAT3 result;
//	result.x = a.x + b.x;
//	result.y = a.y + b.y;
//	result.z = a.z + b.z;
//
//	return result;
//}
//
//void Lift_Obj::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ
//    ,float liftheight,float liftlow ,float liftspeed)
//{
//	m_pos.x = posX;
//	m_pos.y = posY;
//	m_pos.z = posZ;
//
//	m_scale.x = scaleX;
//	m_scale.y = scaleY;
//	m_scale.z = scaleZ;
//
//
//	liftobj_MinBound.x *= m_scale.x;
//	liftobj_MinBound.y *= m_scale.y;
//	liftobj_MinBound.z *= m_scale.z;
//	liftobj_MaxBound.x *= m_scale.x;
//	liftobj_MaxBound.y *= m_scale.y;
//	liftobj_MaxBound.z *= m_scale.z;
//
//
//
//
//	SetBounds(liftobj_MinBound, liftobj_MaxBound);
//
//	c_liftobj_MinBound.x *= m_scale.x;
//	c_liftobj_MinBound.y *= m_scale.y;
//	c_liftobj_MinBound.z *= m_scale.z;
//	c_liftobj_MaxBound.x *= m_scale.x;
//	c_liftobj_MaxBound.y *= m_scale.y;
//	c_liftobj_MaxBound.z *= m_scale.z;
//
//	//これがないとy軸の当たり判定おかしくなる
//
//	if (c_liftobj_MinBound.y < 0)
//	{
//		a = c_liftobj_MinBound.y *= -1;
//		c_liftobj_MaxBound.y += a;
//
//		c_liftobj_MinBound.y = 0;
//	}
//
//	CSetBounds(c_liftobj_MinBound, c_liftobj_MaxBound);
//
//	m_heightPosY = liftheight;
//	m_lowPosY = liftlow;
//	m_moveSpeed = liftspeed;
//
//}
//
//// ブロックとリフトが当たった時に返す
//void Lift_Obj::LiftObj_SetPos()
//{
//	//m_pos = m_oldPos;
//}
//
///**
// * @brief 最高点の位置を設定する
// * @param height  最高点の位置
// */
//void Lift_Obj::SetHeightPosY(float height)
//{
//	m_heightPosY = height;
//}
//
///**
// * @brief 最低点の位置を設定する
// * @param low 最低点の位置
// */
//void Lift_Obj::SetLowPosY(float low)
//{
//	m_lowPosY = low;
//}
//
///**
// * @brief 移動速度を設定する
// * @param speed 移動速度
// */
//void Lift_Obj::SetSpeed(float speed)
//{
//	m_moveSpeed = speed;
//}
//
//
//
//void Lift_Obj::SetLever()
//{
//	m_RiseFlag = true;
//}
//
//void Lift_Obj::SetFLever()
//{
//	m_RiseFlag = false;
//}
//
//bool Lift_Obj::SetRLever()
//{
//	return m_RiseFlag;
//}
//
//
//
//
//
//void Lift_Obj::SetMoveTrue()
//{
//	m_move = true;
//}
//
//void Lift_Obj::SetMoveFalse()
//{
//	m_move = false;
//}
//
//bool Lift_Obj::IsMove()
//{
//	return m_move;
//}
//
//bool Lift_Obj::LiftUp()
//{
//	return m_LiftUp;
//}
//
//float Lift_Obj::LiftMoveSpeed()
//{
//	return m_pos.y;
//}
