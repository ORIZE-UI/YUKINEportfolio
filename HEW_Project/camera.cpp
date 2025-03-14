
#include"Objectcamera.h"
#include"CameraObject.h"
#include "Geometory.h"
#include "Input.h"

ObjectCamera::ObjectCamera()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_scale(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	
{
	m_pObjectCameramodel = new Model;

	if (!m_pObjectCameramodel->Load("Assets/Model/Block/BoxS.fbx", 0.5f, Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー,カメラ用", "Error", MB_OK);
	}
	m_pObjectCameraVS = new VertexShader();
	if (FAILED(m_pObjectCameraVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pObjectCameramodel->SetVertexShader(m_pObjectCameraVS);


}

ObjectCamera::~ObjectCamera()
{
	if (m_pObjectCameramodel)
	{
		delete m_pObjectCameramodel;
		m_pObjectCameramodel = nullptr;
	}
	if (m_pObjectCameraVS)
	{
		delete m_pObjectCameraVS;
		m_pObjectCameraVS = nullptr;
	}
}

void ObjectCamera::Update()
{
	
}

void ObjectCamera::Draw()
{
	
}

//


void ObjectCamera::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;

	

}



void ObjectCamera::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}
