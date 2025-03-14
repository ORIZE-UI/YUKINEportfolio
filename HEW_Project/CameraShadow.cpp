#include "CameraShadow.h"
#include "Input.h"
#include "DirectX.h"

CameraShadow::CameraShadow()
	: m_radXZ(0.0f)
	, m_radY(0.0f)
	, m_radius(0.0f)
{
	m_look.x = 0.0f;
	m_look.y = 0.0f;
	m_look.z = 2.0f;

	m_pos.x = 0.0f;
	m_pos.y = 0.0f;
	m_pos.z = 5.0f;

	ID3D11DeviceContext* pContext = GetContext();

}

CameraShadow::~CameraShadow()
{
}

void CameraShadow::Update()
{
	//m_look = m_pos;

	//const float Speed = 0.05f;
	////カメラの視点を動かす
	////回転
	//if (IsKeyPress(VK_UP))		{ m_radY += Speed; }
	//if (IsKeyPress(VK_DOWN))	{ m_radY -= Speed; }
	//if (IsKeyPress(VK_LEFT))	{ m_radXZ += Speed; }
	//if (IsKeyPress(VK_RIGHT))	{ m_radXZ -= Speed; }

	//if (IsKeyPress('W')) { m_radY += Speed; }
	//if (IsKeyPress('S')) { m_radY -= Speed; }
	//if (IsKeyPress('D')) { m_radXZ += Speed; }
	//if (IsKeyPress('A')) { m_radXZ -= Speed; }

	//const float radius = 5.0f;
	//m_pos.x = cosf(m_radY) * sinf(m_radXZ) * radius + m_look.x;
	//m_pos.y = sinf(m_radY) * radius + m_look.y;
	//m_pos.z = cosf(m_radY) * cosf(m_radXZ) * radius + m_look.z;
}
