#include "CameraObject.h"
#include "Input.h"

CameraObject::CameraObject(ObjectCamera* pCamera)
	: m_pobjCamera(pCamera)
	, m_radXZ(0.0f)
	, m_radY(0.0f)
	, m_radius(0.0f)
{
	m_pos.y = m_pos.y + 2.0f;
}

CameraObject::~CameraObject()
{
}

void CameraObject::Update()
{
	

	const float Speed = 0.05f;

//	W, A, S, D, Q, Eでカメラの視点を動かす
//	回転
	/*if (IsKeyPress(VK_UP)) { m_radY += Speed; }
	if (IsKeyPress(VK_DOWN)) { m_radY -= Speed; }
	if (IsKeyPress(VK_LEFT)) { m_radXZ += Speed; }
	if (IsKeyPress(VK_RIGHT)) { m_radXZ -= Speed; }
*/
	const float radius = 5.0f;
	m_pos.x = 0.0f;//cosf(m_radY) * sinf(m_radXZ) * radius + m_look.x;
	m_pos.y = 1.23f;//sinf(m_radY) * radius + m_look.y;
	
		m_pos.z = 4.84f;//cosf(m_radY) * cosf(m_radXZ) * radius + m_look.z;
		
}
