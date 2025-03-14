#include "ObjectTomegu.h"

Tomegu::Tomegu()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_scale(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, reverse(false)
	, m_rotationY(0.0f)
{
	m_pTomeguTomegu = new Model;
	m_pTomeguR = new Model;


	if (!m_pTomeguTomegu->Load("Assets/Model/Block/tomegu.fbx"))
	{
		MessageBox(nullptr, "留め具モデルの読み込みに失敗しました", "エラー", MB_OK | MB_ICONWARNING);
	}
	/*if (!m_pTomeguR->Load("Assets/Model/Block/tomegu.fbx"))
	{
		MessageBox(nullptr, "留め具モデルの読み込みに失敗しました", "エラー", MB_OK | MB_ICONWARNING);
	}*/
	m_pTomeguVS = new VertexShader();
	if (FAILED(m_pTomeguVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "留め具シェーダーの読み込みに失敗しました", "エラー", MB_OK | MB_ICONWARNING);
	}
	m_pTomeguTomegu->SetVertexShader(m_pTomeguVS);
	//m_pTomeguR->SetVertexShader(m_pTomeguVS);

	

}

Tomegu::~Tomegu()
{
	if (m_pTomeguTomegu)
	{
		delete m_pTomeguTomegu;
		m_pTomeguTomegu = nullptr;
	}
	/*if (m_pTomeguR)
	{
		delete m_pTomeguR;
		m_pTomeguR = nullptr;
	}*/
}

void Tomegu::Update()
{
	
		
	
}

void Tomegu::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::XMFLOAT4X4 mat[3];

	DirectX::XMMATRIX MoT = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	DirectX::XMMATRIX MoS = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	
	DirectX::XMMATRIX world = MoS  *MoT;
	//world = [ワールド行列の設定];
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	mat[1] = viewMatrix; // 与えられた viewMatrix を使う
	mat[2] = projectionMatrix; // 与えられた projectionMatrix を使う

	m_pTomeguVS->WriteBuffer(0, mat);    //配列の先頭アドレスを指定して、まとめて変換行列を渡す
	m_pTomeguTomegu->Draw();

}

void Tomegu::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
{

	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;

}
