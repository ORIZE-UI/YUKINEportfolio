#include "SObjects.h"
#include "Geometory.h"
#include "Input.h"

DirectX::XMFLOAT3 SobjectMinBound = DirectX::XMFLOAT3(-0.005f, -0.4f, -0.05f/*-0.5f, -0.5f, -0.1f*/);
DirectX::XMFLOAT3 SobjectMaxBound = DirectX::XMFLOAT3(0.005f/*0.155f*/, 0.0f, 0.15f);

Model* SObject::m_pObjectModel = nullptr;
VertexShader* SObject::m_pObjectVS = nullptr;
SObject::SObject()
	: m_pos(2000.0f, 0.0f, 0.0f/*2.0f, 0.5f, 0.0f*/)
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_IsBound(false)
	, m_IsJump(false)
{
	m_pObjectModel = new Model;
	////if (!m_pModel->Load("Assets/Model/Golem//Golem.FBX"))
	////{
	////	MessageBox(NULL, "読み込んだファイル名", "Error", MB_OK);
	////}

	//// モデルの読み込み処理
	//if (!m_pObjectModel->Load("Assets/Model/Block/blc.fbx", 0.05f, Model::Flip::XFlip)) {
	//	MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
	//}
	m_pObjectVS = new VertexShader();
	if (FAILED(m_pObjectVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pObjectModel->SetVertexShader(m_pObjectVS);


	


	SetBounds(minBound, maxBound);
}

SObject::~SObject()
{
	if (m_pObjectModel)
	{
		delete m_pObjectModel;
		m_pObjectModel = nullptr;
	}
	if (m_pObjectVS)
	{
		delete m_pObjectVS;
		m_pObjectVS = nullptr;
	}
	m_loadedModels.clear();
}

void SObject::Update()
{
	m_oldPos = m_pos;
	//m_pCamera->Update();
	//if (IsKeyPress('J'))
	//{
	//	m_pos.x += 0.05;
	//	//position.x += 0.05;
	//}
	//if (IsKeyPress('H'))
	//{
	//	m_pos.x -= 0.05;
	//	//position.x -= 0.05;
	//}
	//if (IsKeyPress('U'))
	//{
	//	m_pos.z += 0.05;
	//	//position.z += 0.05;
	//}
	//if (IsKeyPress('N'))
	//{
	//	m_pos.z -= 0.05;
	//	//position.z -= 0.05;
	//}
	if (m_IsJump)
	{
		SetBounds(SobjectMinBound, SobjectMaxBound);
	}
	SetBounds(SobjectMinBound, SobjectMaxBound);  //最小値と最大値をセット
}

void SObject::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
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

	m_pObjectVS->WriteBuffer(0, mat);    //配列の先頭アドレスを指定して、まとめて変換行列を渡す
	m_pObjectModel->Draw();
	/*Geometory::SetWorld(mat[0]);
	Geometory::SetView(mat[1]);
	Geometory::SetProjection(mat[2]);

	Geometory::DrawBox();*/
}

void SObject::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 SObject::GetminBounds()
{
	return minBound;
}

DirectX::XMFLOAT3 SObject::GetmaxBounds()
{
	return maxBound;
}

DirectX::XMFLOAT3 SObject::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

DirectX::XMFLOAT3 SObject::Pos()
{
	return m_pos;
}

//void SObject::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
//{
//	m_pos.x = posX;
//	m_pos.y = posY;
//	m_pos.z = posZ;
//	m_scale.x = scaleX;
//	m_scale.y = scaleY;
//	m_scale.z = scaleZ;
//}

void SObject::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, const char * modelFilePath, float modelScale, Model::Flip modelFlip)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;

	if (!m_pObjectModel->Load(modelFilePath, modelScale, modelFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
		// You might want to handle the error in some way, e.g., log it or throw an exception
	}
}

void SObject::CreateBounds(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, const char* modelFilePath, float modelScale, Model::Flip modelFlip)
{
	m_IsBound = true;
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;
	if (!m_pObjectModel->Load(modelFilePath, modelScale, modelFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
		// You might want to handle the error in some way, e.g., log it or throw an exception
	}
}

void SObject::CreateJump(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, const char * modelFilePath, float modelScale, Model::Flip modelFlip)
{
	m_IsJump = true;
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;

	if (!m_pObjectModel->Load(modelFilePath, modelScale, modelFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
		// You might want to handle the error in some way, e.g., log it or throw an exception
	}
	SobjectMaxBound.x = posX + scaleX * 0.5f;
	SobjectMinBound.x = posX - scaleX * 0.5f;
}

void SObject::LoadSharedModel()
{

}

void SObject::SetBounds()
{
	m_IsBound = true;
}

float SObject::GetMaxBoundX()
{
	return SobjectMaxBound.x;
}

bool SObject::IBound()
{
	return m_IsBound;
}

bool SObject::Jump()
{
	return m_IsJump;
}
std::shared_ptr<Model> SObject::LoadModel(const std::string & filePath, float scale, Model::Flip flip)
{
	// 既に読み込まれている場合は、そのモデルを再利用
	if (m_loadedModels.find(filePath) != m_loadedModels.end())
	{
		return m_loadedModels[filePath];
	}

	// 読み込まれていない場合は、新しいモデルを読み込む
	std::shared_ptr<Model> model = std::make_shared<Model>();
	if (model->Load(filePath.c_str(), scale, flip))
	{
		// 読み込み成功時にモデルを保存
		m_loadedModels[filePath] = model;
		return model;
	}
	else
	{
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
		return nullptr;
	}
}



