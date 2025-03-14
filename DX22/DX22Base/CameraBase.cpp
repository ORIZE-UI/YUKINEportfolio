#include "cameraBase.h"

CameraBase::CameraBase()
	: m_pos(0.0f, 0.0f, -3.0f), m_lookDir(0.0f, 0.0f, 0.0f), m_up(0.0f, 1.0f, 0.0f)
	, m_fovy(60.0f * (3.141592f / 180.0f)), m_aspect(16.0f / 9.0f), m_near(0.3f), m_far(100.0f)
{
}

DirectX::XMFLOAT4X4 CameraBase::GetViewMatrix()
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMMATRIX view;

	// ビュー行列作成
	view = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0.0f),
		DirectX::XMVectorSet(m_lookDir.x, m_lookDir.y, m_lookDir.z, 0.0f),
		DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 0.0f)
	);

	// 転置
	view = DirectX::XMMatrixTranspose(view);

	// 格納
	DirectX::XMStoreFloat4x4(&mat, view);

	return mat;
}

DirectX::XMFLOAT4X4 CameraBase::GetProjectionMatrix()
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMMATRIX proj;

	// プロジェクション行列を作成
	proj = DirectX::XMMatrixPerspectiveFovLH(
		m_fovy, m_aspect, m_near, m_far
	);

	// 転置
	proj = DirectX::XMMatrixTranspose(proj);

	// 格納
	DirectX::XMStoreFloat4x4(&mat, proj);

	return mat;
}

DirectX::XMFLOAT3 CameraBase::GetPos()
{
	return m_pos;
}

DirectX::XMFLOAT3 CameraBase::GetLook()
{
	return m_lookDir;
}