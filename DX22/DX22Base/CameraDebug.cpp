#include "cameraDebug.h"
#include "Input.h"
#include <math.h>

const float	CameraDebug::MIN_DISTANCE = 0.2f;
const float	CameraDebug::MAX_DISTANCE = 10.0f;
const float	CameraDebug::MIN_ROTATE_Y = -1.0f * (3.141592f / 2.0f - 0.01f);
const float	CameraDebug::MAX_ROTATE_Y = 3.141592f / 2.0f - 0.01f;

CameraDebug::CameraDebug(void)
	: m_fDistanceLookPos(5.0f)
	, m_fRotateXZ(0.0f)
	, m_fRotateY(0.0f)
{
}

void CameraDebug::Update(void)
{
	const float c_fMoveSpd = 0.5f;
	const float	c_fRotSpd = 0.1f;

	// ----------’Ž‹“_‚ðˆÚ“®----------
	if (IsKeyPress(VK_UP))
	{
		m_lookDir.z -= c_fMoveSpd;
	}
	else if (IsKeyPress(VK_DOWN))
	{
		m_lookDir.z += c_fMoveSpd;
	}

	if (IsKeyPress(VK_LEFT))
	{
		m_lookDir.x += c_fMoveSpd;
	}
	else if (IsKeyPress(VK_RIGHT))
	{
		m_lookDir.x -= c_fMoveSpd;
	}

	if (IsKeyPress(VK_SHIFT))
	{
		m_lookDir.y += c_fMoveSpd;
	}
	else if (IsKeyPress(VK_CONTROL))
	{
		m_lookDir.y -= c_fMoveSpd;
	}

	// ----------ƒJƒƒ‰‚ÌˆÊ’u‚ðˆÚ“®----------
	if (IsKeyPress('W'))
	{
		m_fRotateY = m_fRotateY + c_fRotSpd > MAX_ROTATE_Y ? MAX_ROTATE_Y : m_fRotateY + c_fRotSpd;
	}
	else if (IsKeyPress('S'))
	{
		m_fRotateY = m_fRotateY - c_fRotSpd < MIN_ROTATE_Y ? MIN_ROTATE_Y : m_fRotateY - c_fRotSpd;
	}

	if (IsKeyPress('A'))
	{
		m_fRotateXZ += c_fRotSpd;
	}
	else if (IsKeyPress('D'))
	{
		m_fRotateXZ -= c_fRotSpd;
	}

	if (IsKeyPress('Q'))
	{
		m_fDistanceLookPos = m_fDistanceLookPos + c_fMoveSpd > MAX_DISTANCE ? MAX_DISTANCE : m_fDistanceLookPos + c_fMoveSpd;
	}
	else if (IsKeyPress('E'))
	{
		m_fDistanceLookPos = m_fDistanceLookPos - c_fMoveSpd < MIN_DISTANCE ? MIN_DISTANCE : m_fDistanceLookPos - c_fMoveSpd;
	}

	m_pos.x = cosf(m_fRotateY) * sinf(m_fRotateXZ) * m_fDistanceLookPos + m_lookDir.x;
	m_pos.y = sinf(m_fRotateY) * m_fDistanceLookPos + m_lookDir.y;
	m_pos.z = cosf(m_fRotateY) * cosf(m_fRotateXZ) * m_fDistanceLookPos + m_lookDir.z;
}