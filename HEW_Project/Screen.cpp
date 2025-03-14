
#include"Screen.h"
#include "Geometory.h"
#include "Input.h"

Screen::Screen()
	: m_pos(0.0f, -4.4f, -9.0f)
	, m_scale(0.525f,0.27f,1.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	
{
	m_pNoEntryModel = new Model;

	if (!m_pNoEntryModel->Load("Assets/Model/Block/Screen.fbx",Model::Flip::ZFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー_スクリーン", "Error", MB_OK);
	}
	m_pNoEntryVS = new VertexShader();
	if (FAILED(m_pNoEntryVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pNoEntryModel->SetVertexShader(m_pNoEntryVS);

	
}

Screen::~Screen()
{
	if (m_pNoEntryModel)
	{
		delete m_pNoEntryModel;
		m_pNoEntryModel = nullptr;
	}
	if (m_pNoEntryVS)
	{
		delete m_pNoEntryVS;
		m_pNoEntryVS = nullptr;
	}
}

void Screen::Update()
{

}

void Screen::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
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

	m_pNoEntryVS->WriteBuffer(0, mat);    //配列の先頭アドレスを指定して、まとめて変換行列を渡す
	m_pNoEntryModel->Draw();
}



