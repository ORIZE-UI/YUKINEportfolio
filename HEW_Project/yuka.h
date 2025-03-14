#pragma once
#pragma once
#include "CameraBase.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"

class Yuka : public GameObject
{
public:
	Yuka();
	~Yuka();

	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
	void SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 GetminBounds();
	DirectX::XMFLOAT3 GetmaxBounds();
	DirectX::XMFLOAT3 Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);


	void CSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max);
	DirectX::XMFLOAT3 CGetminBounds();
	DirectX::XMFLOAT3 CGetmaxBounds();
	DirectX::XMFLOAT3 CAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b);


	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);



private:
	Model* m_pNoEntryModel;
	VertexShader* m_pNoEntryVS;
	DirectX::XMFLOAT3 m_oldPos;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;


	DirectX::XMFLOAT3 m_direction;
	DirectX::XMMATRIX m_rotationMatrix;
	float m_lastRotationY;


	DirectX::XMFLOAT3 NoEntryMinBound;//プレイヤー
	DirectX::XMFLOAT3 NoEntryMaxBound;


	DirectX::XMFLOAT3 cNoEntryMinBound;//ブロック
	DirectX::XMFLOAT3 cNoEntryMaxBound;

};

