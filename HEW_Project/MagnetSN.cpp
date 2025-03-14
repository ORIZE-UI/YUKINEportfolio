#include "MagnetSN.h"
#include "Geometory.h"
#include "Input.h"
#include"Gamepad.h"

InputManager imanagerMg = InputManager();


Magnet::Magnet()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_scale(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	, moveok(false)
	, m_isPossessed(true)
	

{
	m_pMgtM = new Model;
	m_pMgtS = new Model;

	if (!m_pMgtM->Load("Assets/Model/Block/MagnetN.fbx", 0.5f, Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラーMgnN", "Error", MB_OK);
	}
	if (!m_pMgtS->Load("Assets/Model/Block/MagnetS.fbx", 0.5f, Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラーMgnS", "Error", MB_OK);
	}
	m_pObjectVS = new VertexShader();
	if (FAILED(m_pObjectVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pMgtM->SetVertexShader(m_pObjectVS);
	m_pMgtS->SetVertexShader(m_pObjectVS);

	lastDrawnModel = nullptr;

}

Magnet::~Magnet()
{
	if (m_pMgtM)
	{
		delete m_pMgtM;
		m_pMgtM = nullptr;
	}
	if (m_pMgtS)
	{
		delete m_pMgtS;
		m_pMgtS = nullptr;
	}
	if (m_pObjectVS)
	{
		delete m_pObjectVS;
		m_pObjectVS = nullptr;
	}
}

void Magnet::Update()
{
	

}

void Magnet::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	imanagerMg.addKeycode(0, 0, GAMEPAD_KEYTYPE::LTrigger, XINPUT_GAMEPAD_LEFT_SHOULDER);
	imanagerMg.addKeycode(1, 0, GAMEPAD_KEYTYPE::RTrigger, XINPUT_GAMEPAD_RIGHT_SHOULDER);

	imanagerMg.inspect();

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


	if (!lastDrawnModel)
	{
		m_pMgtM->Draw();
	}

	if (moveok == true)
	{
		if (imanagerMg.getKey(1) & 0b011)
		{
			m_pMgtM->Draw();
			lastDrawnModel = m_pMgtM;
		}
		else if (imanagerMg.getKey(0) & 0b011)
		{
			m_pMgtS->Draw();
			lastDrawnModel = m_pMgtS;
		}
	}

	// if 文を抜けた後にも最後に描画されたモデルを描画する
	if (lastDrawnModel)
	{
		lastDrawnModel->Draw();
	}
	
		


	/*if ()
	{
		m_pMgtM->Draw();
	}
	else if ()
	{
		m_pMgtS->Draw();
	}*/
}


DirectX::XMFLOAT3 Magnet::Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}


DirectX::XMFLOAT3 Magnet::CAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

void Magnet::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;
}

//憑依判定用
void Magnet::Set()
{
	moveok = true;
}

void Magnet::SetF()
{
	moveok = false;
}

bool Magnet::SetR()
{
	return moveok;
}

