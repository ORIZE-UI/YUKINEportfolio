#pragma once
#include "CameraBase.h"
#include"Objectcamera.h"
#include "Model.h"
#include "Shader.h"
#include"GameObject.h"

class ObjectCamera:public GameObject
{
public:
	ObjectCamera();
	~ObjectCamera();

	void Update();
	void Draw();
	

	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);

	void SetCamera(CameraBase * pCamera);



private:
	Model* m_pObjectCameramodel;
	VertexShader* m_pObjectCameraVS;
	CameraBase*			m_pCamera;
	DirectX::XMFLOAT3 m_oldPos;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;


	DirectX::XMFLOAT3 m_direction;
	DirectX::XMMATRIX m_rotationMatrix;
	float m_lastRotationY;


};

