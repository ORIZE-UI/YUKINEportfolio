//侵入禁止エリア　床 

#include"yuka.h"
#include "Geometory.h"
#include "Input.h"

Yuka::Yuka()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_scale(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	, NoEntryMinBound(-0.5f, -0.5f, -0.5f)
	, NoEntryMaxBound(0.5f, 0.5f, 0.5f)
	, cNoEntryMinBound(-0.5f, -0.5f, -0.5f)
	, cNoEntryMaxBound(0.5f, 0.5f, 0.5f)

{
	m_pNoEntryModel = new Model;

	if (!m_pNoEntryModel->Load("Assets/Stage/StageFloor.fbx", 0.1f, Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー_床", "Error", MB_OK);
	}
	m_pNoEntryVS = new VertexShader();
	if (FAILED(m_pNoEntryVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pNoEntryModel->SetVertexShader(m_pNoEntryVS);

	SetBounds(minBound, maxBound);

	CSetBounds(cminBound, cmaxBound);
}

Yuka::~Yuka()
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

void Yuka::Update()
{
	SetBounds(NoEntryMinBound, NoEntryMaxBound);

	CSetBounds(cNoEntryMinBound, cNoEntryMaxBound);//ブロック同士の当たり判定
}

void Yuka::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
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

//
void Yuka::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 Yuka::GetminBounds()
{
	return minBound;
}

DirectX::XMFLOAT3 Yuka::GetmaxBounds()
{
	return maxBound;
}

DirectX::XMFLOAT3 Yuka::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_



void Yuka::CSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	cminBound = CAdd(m_pos, min);
	cmaxBound = CAdd(m_pos, max);
}

DirectX::XMFLOAT3 Yuka::CGetminBounds()
{
	return cminBound;
}

DirectX::XMFLOAT3 Yuka::CGetmaxBounds()
{
	return cmaxBound;
}

DirectX::XMFLOAT3 Yuka::CAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}


void Yuka::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;

	NoEntryMinBound.x *= m_scale.x;
	NoEntryMinBound.y *= m_scale.y;
	NoEntryMinBound.z *= m_scale.z;
	NoEntryMaxBound.x *= m_scale.x;
	NoEntryMaxBound.y *= m_scale.y;
	NoEntryMaxBound.z *= m_scale.z;

	SetBounds(NoEntryMinBound, NoEntryMaxBound);


	cNoEntryMinBound.x *= m_scale.x;
	cNoEntryMinBound.y *= m_scale.y;
	cNoEntryMinBound.z *= m_scale.z;
	cNoEntryMaxBound.x *= m_scale.x;
	cNoEntryMaxBound.y *= m_scale.y;
	cNoEntryMaxBound.z *= m_scale.z;

	CSetBounds(cNoEntryMinBound, cNoEntryMaxBound);

}


