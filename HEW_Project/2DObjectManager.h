#pragma once
#include "SObjects.h"
#include "CameraBase.h"
#include "CameraDebug.h"
#include "Player.h"
#include "Collision.h"
#include "ShadowP.h"



class Object2D
{
public:
	//std::shared_ptr<Model> LoadModel(const std::string & filePath, float scale, Model::Flip flip);
	Object2D();
	~Object2D();
	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
private:
	SObject* m_pSObjects;
	CameraDebug* m_pObjectCamera;
	Player* m_pPlayer;
	AABB* aabb;
	ShadowP* m_pShadowP;
	int m_num;
	bool m_IsUse;


	

		//std::shared_ptr<Model> LoadModel(const std::string & filePath, float scale, Model::Flip flip);

};

