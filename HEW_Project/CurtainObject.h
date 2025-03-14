#pragma once
#include "GameObject.h"
#include "Object.h"
#include "Model.h"

class Curtain : public GameObject
{
public:
	Curtain();
	~Curtain();

	void Update();
	void RightDraw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
	void LeftDraw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);

private:
	Model* m_pModel;	// モデル
	VertexShader* m_pVS; // 頂点シェーダー

	DirectX::XMFLOAT3 m_pos;	// 位置
	DirectX::XMFLOAT3 m_scale;	// 大きさ

};
