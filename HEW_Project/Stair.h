#pragma once
#include "CameraBase.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"
#include"Soundtest.h"

class Stair : public GameObject
{
public:
	Stair();
	~Stair();

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

	DirectX::XMFLOAT3 GetStairMaxBounds();
	DirectX::XMFLOAT3 GetPos();

	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, bool reverse);
	void Set();
	void SetF();
	bool SetR();


	void OBJPos();
	void OBJPosy();
	

	void Set1();
	void SetF1();
	void SetColgravity();
	void SetColgravityfalse();
	void SetStairTop();

	void MoveStair(float y);
	void framepls();

	bool SetR1();
	bool IsGravity();
	bool IsXZ();
	bool IsStairTop();
	bool IsMove();
	bool IsReverse();
	void ExtractSlopeVertexCoordinates(Model& slopeModel);

	void SetSlope();
	void SetReverseSlope();
	void SetSlopeY(float y);

	
private:
	Model* m_pStairModelL;
	Model* m_pStairModelHL;
	Model* m_pStairModelR;
	Model* m_pStairModelHR;

	VertexShader* m_pStairVS;
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
	bool colgravity;
	bool xz;
	bool StairTop;
	bool m_reverse;
	float a = 0.0f;



	IXAudio2SourceVoice* m_pSVSEBlk; // サウンドソース
	XAUDIO2_BUFFER* m_pSDSEBlk; // サウンドバッファ

	DirectX::XMFLOAT3 StairMinBound;
	DirectX::XMFLOAT3 StairMaxBound;

	DirectX::XMFLOAT3 hStairMinBound;
	DirectX::XMFLOAT3 hStairMaxBound;

	DirectX::XMFLOAT3 cStairMinBound;
	DirectX::XMFLOAT3 cStairMaxBound;

	int  frame = 25;

};

