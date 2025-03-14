#include "goalPoll.h"

GoalPoll::GoalPoll()
	: m_pCamera(nullptr)
{
	m_pModel = new Model();
	if (!m_pModel->Load("Assets/Model/goalPoll.fbx", 1.0f, Model::XFlip))
	{
		MessageBoxA(nullptr, "goalPoll.fbx", "Error", MB_OK);
	}

	//頂点シェーダ読込
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBoxA(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pModel->SetVertexShader(m_pVS);	//モデルに頂点シェーダを設定
}

GoalPoll::~GoalPoll()
{
	delete m_pModel;
	delete m_pVS;
}

void GoalPoll::Draw(void)
{
	DirectX::XMFLOAT4X4 mat[3];

	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(0.6f, 0.4f, 0.3f);
	DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(0.0f);
	DirectX::XMMATRIX move = DirectX::XMMatrixTranslation(26.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX world = scale * rotY * move;

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();

	m_pVS->WriteBuffer(0, mat);
	m_pVS->Bind();

	m_pModel->Draw();
}

void GoalPoll::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}