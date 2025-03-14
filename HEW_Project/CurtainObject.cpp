#include "CurtainObject.h"
#include "Geometory.h"
#include "Input.h"
#include "GameObject.h"

#define CURTAIN_POS_X    4.0f
#define CURTAIN_POS_Y    -0.3f
#define CURTAIN_POS_Z    2.2f

Curtain::Curtain()
	: m_pos(0.0f, -0.5f, 2.0f),
	 m_scale(0.4f, 0.4f, -0.4f)
{
	m_pModel = new Model();
	if (!m_pModel->Load("Assets/Model/Block/Curtain.fbx", Model::Flip::XFlip))
	{
		MessageBox(nullptr, "Curtainモデルの読み込みに失敗しました", "エラー", MB_OK | MB_ICONWARNING);
	}
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "Curtain頂点シェーダーの読み込みに失敗しました", "エラー", MB_OK | MB_ICONWARNING);
	}
	m_pModel->SetVertexShader(m_pVS);
}

Curtain::~Curtain()
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

void Curtain::Update()
{
}

void Curtain::RightDraw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	m_pos.x = -CURTAIN_POS_X;
	m_pos.y = CURTAIN_POS_Y;
	m_pos.z = CURTAIN_POS_Z;

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
void Curtain::LeftDraw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	m_pos.x = CURTAIN_POS_X;
	m_pos.y = CURTAIN_POS_Y;
	m_pos.z = CURTAIN_POS_Z;
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

void Curtain::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;

	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;

}
