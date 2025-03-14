#pragma once
#pragma once
#include "CameraBase.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"
#include"Soundtest.h"

class Tomegu : public GameObject
{
public:
	Tomegu();
	~Tomegu();

	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, bool reverse);
	



	

	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);






private:
	Model* m_pTomeguTomegu;
	Model* m_pTomeguR;

	VertexShader* m_pTomeguVS;
	DirectX::XMFLOAT3 m_oldPos;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;


	DirectX::XMFLOAT3 m_direction;
	DirectX::XMMATRIX m_rotationMatrix;
	float m_rotationY;

	bool reverse;

};

