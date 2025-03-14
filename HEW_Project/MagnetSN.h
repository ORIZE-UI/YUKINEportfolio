#pragma once
#include "CameraBase.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"




class Magnet : public GameObject
{



public:



	Magnet();
	~Magnet();

	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

	DirectX::XMFLOAT3 Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

	DirectX::XMFLOAT3 CAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);


	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);
	void Set();
	void SetF();
	bool SetR();



	





private:
	Model* m_pMgtM;
	Model* m_pMgtS;
	Model* lastDrawnModel;

	VertexShader* m_pObjectVS;
	DirectX::XMFLOAT3 m_oldPos;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;

	float m_rotationY;
	DirectX::XMFLOAT3 m_direction;
	DirectX::XMMATRIX m_rotationMatrix;
	float m_lastRotationY;
	bool moveok;

	bool m_isPossessed;
};
