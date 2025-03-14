#pragma once
#include "Model.h"
#include "Shader.h"
#include"Soundtest.h"
#include "CameraBase.h"
#include "GameObject.h"
class Lever : public GameObject
{
public:
	Lever();
	~Lever();

	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
	void SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 GetminBounds();
	DirectX::XMFLOAT3 GetmaxBounds();
	DirectX::XMFLOAT3 Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

	void HSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 HGetminBounds();
	DirectX::XMFLOAT3 HGetmaxBounds();
	DirectX::XMFLOAT3 HAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);


	


	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);
	void SetMoveTrue();
	void SetMoveFalse();
	void Set();
	void SetF();
	bool SetR();


	void OBJPos();
	void OBJPosy();






private:
	Model* m_pLeverModelN;
	Model* m_pLeverModelUP;
	Model* m_pLeverModelDown;

	VertexShader* m_pLeverVS;
	DirectX::XMFLOAT3 m_oldPos;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;




	bool m_move;


	bool ok;
	float a = 0.0f;



	
	DirectX::XMFLOAT3 LeverMinBound;
	DirectX::XMFLOAT3 LeverMaxBound;

	DirectX::XMFLOAT3 hLeverMinBound;
	DirectX::XMFLOAT3 hLeverMaxBound;

	DirectX::XMFLOAT3 cLeverMinBound;
	DirectX::XMFLOAT3 cLeverMaxBound;



};

