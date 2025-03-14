#pragma once
#include "CameraBase.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"


#include "CameraDebug.h"





class Screen : public GameObject
{
public:
	Screen();
	~Screen();

	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
	






private:
	Model* m_pNoEntryModel;
	VertexShader* m_pNoEntryVS;
	DirectX::XMFLOAT3 m_oldPos;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;


	DirectX::XMFLOAT3 m_direction;
	DirectX::XMMATRIX m_rotationMatrix;
	float m_lastRotationY;


	

};

