//#pragma once
//#include "CameraBase.h"
//#include "GameObject.h"
//#include "Model.h"
//#include "Shader.h"
//
//#include<string>
//#include <unordered_map>
//
//
//
//class SObject : public GameObject
//{
//public:
//	std::shared_ptr<Model> LoadModel(const std::string & filePath, float scale, Model::Flip flip);
//
//	SObject();
//	~SObject();
//
//	void Update();
//	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
//	void SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
//	DirectX::XMFLOAT3 GetminBounds();
//	DirectX::XMFLOAT3 GetmaxBounds();
//	DirectX::XMFLOAT3 Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
//	DirectX::XMFLOAT3 Pos();
//	
//	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, const char * modelFilePath, float modelScale, Model::Flip modelFlip);
////	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);
//	void CreateBounds(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);
//	void CreateJump(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);
//	float GetMaxBoundX();
//	//bool IBound();
//	//bool Jump();
//private:
//	Model* m_pObjectModel;
//	VertexShader* m_pObjectVS;
//	DirectX::XMFLOAT3 m_oldPos;
//	DirectX::XMFLOAT3 m_pos;
//	DirectX::XMFLOAT3 m_scale;
//	bool m_IsBound;
//	bool m_IsJump;
//	
//
//
//	std::unordered_map<std::string, std::shared_ptr<Model>> m_loadedModels;
//
//};
//



#pragma once
#include "CameraBase.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"
#include<string>
#include <unordered_map>
class SObject : public GameObject
{
public:
	SObject();
	~SObject();

	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
	void SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 GetminBounds();
	DirectX::XMFLOAT3 GetmaxBounds();
	DirectX::XMFLOAT3 Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
	DirectX::XMFLOAT3 Pos();
	//void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);
	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, const char* modelFilePath, float modelScale, Model::Flip modelFlip);
	void CreateBounds(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, const char* modelFilePath, float modelScale, Model::Flip modelFlip);
	void CreateJump(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, const char* modelFilePath, float modelScale, Model::Flip modelFlip);
	void LoadSharedModel(); // 共有モデルデータを読み込む関数
	void SetBounds();
	float GetMaxBoundX();
	bool IBound();
	bool Jump();
	std::shared_ptr<Model> LoadModel(const std::string & filePath, float scale, Model::Flip flip);
private:
	//Model* m_pObjectModel;
	//VertexShader* m_pObjectVS;
	DirectX::XMFLOAT3 m_oldPos;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;
	bool m_IsBound;
	bool m_IsJump;
	static Model* m_pObjectModel;
	static VertexShader* m_pObjectVS;
	std::unordered_map<std::string, std::shared_ptr<Model>> m_loadedModels;
};
