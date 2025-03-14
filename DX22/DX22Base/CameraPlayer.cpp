#include "cameraPlayer.h"
#include "Input.h"

const float	CameraPlayer::MIN_DISTANCE = 0.2f;
const float	CameraPlayer::MAX_DISTANCE = 50.0f;
const float	CameraPlayer::MIN_ROTATE_Y = -1.0f * (3.141592f / 2.0f - 0.01f);
const float	CameraPlayer::MAX_ROTATE_Y = 3.141592f / 2.0f - 0.01f;

CameraPlayer::CameraPlayer(Player* pPlayer)
	: m_pPlayer(pPlayer)
	, m_fDistanceLookPos(5.0f)
	, m_fRotateXZ(0.0f)
	, m_fRotateY(0.0f)
{

}

CameraPlayer::~CameraPlayer()
{

}

void CameraPlayer::Update(void)
{
	if (!m_pPlayer)
	{
		return;
	}

	const float	c_fRotSpd = 0.1f;

	m_lookDir = m_pPlayer->GetPos();

	// ----------カメラの位置を移動----------
#if 0
	if (IsKeyPress(VK_UP))
	{
		m_fRotateY = m_fRotateY + c_fRotSpd > MAX_ROTATE_Y ? MAX_ROTATE_Y : m_fRotateY + c_fRotSpd;
	}
	else if (IsKeyPress(VK_DOWN))
	{
		m_fRotateY = m_fRotateY - c_fRotSpd < MIN_ROTATE_Y ? MIN_ROTATE_Y : m_fRotateY - c_fRotSpd;
	}

	if (IsKeyPress(VK_LEFT))
	{
		m_fRotateXZ += c_fRotSpd;
	}
	else if (IsKeyPress(VK_RIGHT))
	{
		m_fRotateXZ -= c_fRotSpd;
	}

	m_pos.x = cosf(m_fRotateY) * sinf(m_fRotateXZ) * m_fDistanceLookPos + m_lookDir.x;
	m_pos.y = sinf(m_fRotateY) * m_fDistanceLookPos + m_lookDir.y;
	m_pos.z = cosf(m_fRotateY) * cosf(m_fRotateXZ) * m_fDistanceLookPos + m_lookDir.z;
#else

// プレイヤーに追従するカメラ
	DirectX::XMFLOAT3 playerLookDir = m_pPlayer->GetLookDir();
	DirectX::XMFLOAT3 playerPos = m_pPlayer->GetPos();
	DirectX::XMVECTOR vPlayerPos = DirectX::XMLoadFloat3(&playerPos);
	DirectX::XMVECTOR vCamPos = DirectX::XMLoadFloat3(&playerLookDir);
	vCamPos = DirectX::XMVectorNegate(vCamPos);
	vCamPos = DirectX::XMVectorScale(vCamPos, m_fDistanceLookPos);
	vCamPos = DirectX::XMVectorAdd(vCamPos, vPlayerPos);
	vCamPos = DirectX::XMVectorAdd(vCamPos,
		DirectX::XMVectorSet(0.0f, 1.5f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3(&m_pos, vCamPos);

#endif
}