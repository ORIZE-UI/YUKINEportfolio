#pragma once
// ShadowP.h
#include "Model.h"
#include "CameraDebug.h"
#include "CameraBase.h"
#include "Collision.h"
#include "GameObject.h"
#include"ShaderList.h"
#include"Soundtest.h"
#include "LibEffekseer.h"

class ShadowP : public GameObject
{
public:
	ShadowP();
	~ShadowP();

	void Update(float tick);
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
	void SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
	void SetPos(float x, float y, float z);
	void ShadowPPosY();
	void oldX();
	float ShadowPPosX();
	DirectX::XMFLOAT3 NowPos();
	void ShadowPPos();
	void ShadowPupY();
	void ShadowPpushX();
	void Use();
	void NotUse();
	void Jump();
	bool isUse();
	bool IsJump();
	float GetMove();
	void SetFooting(bool footing);

	void SetKeikai(bool waring);

	void SetDeath(bool death);
	bool IsDeath() { return m_IsDeath; }

	//bool IsJump();
private:
	Model* m_pModel;
	VertexShader* m_pVS;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_oldPos;
	DirectX::XMFLOAT3 m_firstPos;


	Model::AnimeNo m_animeWalk;	// 影の歩行アニメーション
	Model::AnimeNo m_keikai;
	Model::AnimeNo m_death;

	bool m_IsAlterDir; // 移動方向の反転
	bool m_LastDir; // 最後に向いていた方向
	float m_rotationY; // Y軸回転
	bool m_Jump;
	bool m_footing;
	float m_moveY;
	float m_JumpY;
	bool m_IsKeikai;
	bool m_IsDeath;

	// エフェクト
	Effekseer::Handle		m_EffectHandle; // エフェクトハンドル
	Effekseer::EffectRef	m_Effect; // エフェクト

	IXAudio2SourceVoice* m_pSVSESdPly; // サウンドソース
	XAUDIO2_BUFFER* m_pSDSESdPly; // サウンドバッファ

	IXAudio2SourceVoice* m_pSVSESdSPly; // サウンドソース
	XAUDIO2_BUFFER* m_pSDSESdSPly; // サウンドバッファ
};

