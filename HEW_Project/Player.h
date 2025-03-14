#pragma once
// Player.h
#include "Model.h"
#include "Collision.h"
#include "HCollision.h"
#include "GameObject.h"
#include "Soundtest.h"
#include"ShaderList.h"

//#include"Object.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	void Update(float tick);
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

	void SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 GetminBox();
	DirectX::XMFLOAT3 GetmaxBox();
	DirectX::XMFLOAT3 Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

	void HSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 HGetminBox();
	DirectX::XMFLOAT3 HGetmaxBox();
	DirectX::XMFLOAT3 HAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);


	void PlayerPos();
	void HPlayerPos();
	void RPlayerPos();


	bool Set();
	void SetOk();
	void SetNOk();

	float GetPosX();
	float GetPosY();
	float GetPosZ();

	void SetAnime();
	void SetAnime2();

private:
	Model* m_pModel;
	VertexShader* m_pVS;
	PixelShader* m_pPS;

	Model::AnimeNo m_anime_Levitation;	// 黒子用の浮遊
	Model::AnimeNo m_anime_possession;	// 憑依時のアニメーション
  
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_oldPos;
	float m_rotationY;
	float  m_lastFacingDirection;
	DirectX::XMFLOAT3 m_direction;
	DirectX::XMMATRIX m_rotationMatrix;
	bool IsUse;
	bool ok;

	IXAudio2SourceVoice* m_pSVSEPly; // サウンドソース
	XAUDIO2_BUFFER* m_pSDSEPly; // サウンドバッファ

};


