//#pragma once
//#include "CameraBase.h"
//#include "GameObject.h"
//#include "Model.h"
//#include "Shader.h"
//#include"Soundtest.h"
//
//class Object : public GameObject
//{
//public:
//	Object();
//	~Object();
//
//	void Update();
//	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
//	void SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
//	DirectX::XMFLOAT3 GetminBounds();
//	DirectX::XMFLOAT3 GetmaxBounds() ;
//	DirectX::XMFLOAT3 Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
//
//	void HSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
//	DirectX::XMFLOAT3 HGetminBounds();
//	DirectX::XMFLOAT3 HGetmaxBounds();
//	DirectX::XMFLOAT3 HAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
//
//
//	void CSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max);
//	DirectX::XMFLOAT3 CGetminBounds();
//	DirectX::XMFLOAT3 CGetmaxBounds();
//	DirectX::XMFLOAT3 CAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
//
//	DirectX::XMFLOAT3 GetObjectMaxBounds();
//	DirectX::XMFLOAT3 GetPos();
//
//	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, bool hyoui, bool Auto, bool XZM);
//
//	//void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, bool hyoui, bool Auto);
//
//	//void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, bool hyoui);
//
//	
//	void Set();
//	void SetF();
//	bool SetR();
//	
//
//	void OBJPos();
//	void OBJPosy();
//
//
//	void Set1();
//	void SetF1();
//
//	void SetColgravity();
//	void SetColgravityfalse();
//	void framepls();
//	void SetObjectTop();
//	void MoveObject(float y);
//	void SetSlope();
//	void SetReverseSlope();
//
//	void MoveXfalse();
//	void MoveXtrue();
//
//
//	bool SetR1();
//	bool IsGravity();
//	bool IsXZ();
//	bool IsObjectTop();
//	bool IsMove();
//	bool possessionok();
//	bool IsAutoMove();
//
//	bool MVX();
//
//private:
//	Model* m_pObjectModel;
//	Model* m_pObjectModelH;
//	Model* m_pObjectModelMoveX;
//	Model* m_pObjectModelMoveZ;
//	Model* m_pObjectTomegu;
//
//	VertexShader* m_pObjectVS;
//	DirectX::XMFLOAT3 m_oldPos;
//	DirectX::XMFLOAT3 m_pos;
//	DirectX::XMFLOAT3 m_scale;
//	DirectX::XMFLOAT3 m_jmp;
//	DirectX::XMFLOAT3 m_mmovespeed;
//
//	float m_rotationY;
//	DirectX::XMFLOAT3 m_direction;
//	DirectX::XMMATRIX m_rotationMatrix;
//	float m_lastRotationY;
//	bool moveok;
//	
//
//	bool ok;
//	bool gravity;
//	bool xz;
//	bool objectTop;
//	bool colgravity;
//	bool possession;
//	bool Automove;
//	bool XZMOVE;
//	bool MoveX;
//	
//	float a = 0.0f;
//
//	
//	
//
//	IXAudio2SourceVoice* m_pSVSEBlk; // サウンドソース
//	XAUDIO2_BUFFER* m_pSDSEBlk; // サウンドバッファ
//
//	DirectX::XMFLOAT3 objectMinBound;
//	DirectX::XMFLOAT3 objectMaxBound;
//
//	DirectX::XMFLOAT3 hobjectMinBound;
//	DirectX::XMFLOAT3 hobjectMaxBound;
//
//	DirectX::XMFLOAT3 cobjectMinBound;
//	DirectX::XMFLOAT3 cobjectMaxBound;
//
//	int frame = 30;
//
//
//};
//


#pragma once
#include "CameraBase.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"
#include"Soundtest.h"

class Object : public GameObject
{
public:
	Object();
	~Object();

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


	void CSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max);
	DirectX::XMFLOAT3 CGetminBounds();
	DirectX::XMFLOAT3 CGetmaxBounds();
	DirectX::XMFLOAT3 CAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

	DirectX::XMFLOAT3 GetObjectMaxBounds();
	DirectX::XMFLOAT3 GetPos();

	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, bool hyoui, bool Auto, bool XZM);

	//void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, bool hyoui, bool Auto);

	//void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, bool hyoui);


	void Set();
	void SetF();
	bool SetR();


	void OBJPos();
	void OBJPosy();


	void Set1();
	void SetF1();

	void SetColgravity();
	void SetColgravityfalse();
	void framepls();
	void SetObjectTop();
	void MoveObject(float y);
	void SetSlope();
	void SetReverseSlope();

	void MoveXfalse();
	void MoveXtrue();

	void OnBoxfalse();
	void OnBoxtrue();

	bool SetR1();
	bool IsGravity();
	bool IsXZ();
	bool IsObjectTop();
	bool IsMove();
	bool possessionok();
	bool IsAutoMove();

	bool MVX();

	bool IsOnBox();

private:
	Model* m_pObjectModel;
	Model* m_pObjectModelH;
	Model* m_pObjectModelMoveX;
	Model* m_pObjectModelMoveZ;
	Model* m_pObjectTomegu;

	VertexShader* m_pObjectVS;
	DirectX::XMFLOAT3 m_oldPos;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_jmp;
	DirectX::XMFLOAT3 m_mmovespeed;

	float m_rotationY;
	DirectX::XMFLOAT3 m_direction;
	DirectX::XMMATRIX m_rotationMatrix;
	float m_lastRotationY;
	bool moveok;


	bool ok;
	bool gravity;
	bool xz;
	bool objectTop;
	bool colgravity;
	bool possession;
	bool Automove;
	bool XZMOVE;
	bool MoveX;
	bool OnBox;

	float a = 0.0f;




	IXAudio2SourceVoice* m_pSVSEBlk; // サウンドソース
	XAUDIO2_BUFFER* m_pSDSEBlk; // サウンドバッファ

	DirectX::XMFLOAT3 objectMinBound;
	DirectX::XMFLOAT3 objectMaxBound;

	DirectX::XMFLOAT3 hobjectMinBound;
	DirectX::XMFLOAT3 hobjectMaxBound;

	DirectX::XMFLOAT3 cobjectMinBound;
	DirectX::XMFLOAT3 cobjectMaxBound;

	int frame = 25;


};

