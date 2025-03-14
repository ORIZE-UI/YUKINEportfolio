#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Model.h"
#include "Shader.h"
#include "cameraBase.h"
#include "Texture.h"
#include "MoveEmitter.h"
#include "TrailEffect.h"
#include <DirectXMath.h>

class Player : public Vehicle
{
public:
	enum class AccelKind
	{
		Road,
		OutSide,
		SpeedUpItem,
	};

	Player();
	~Player();

	void	Update(void);
	void	Draw(void);
	void	SetCamera(CameraBase* pCamera);
	DirectX::XMFLOAT3 GetPos() const override;
	void	SetPos(DirectX::XMFLOAT3);
	DirectX::XMFLOAT3 GetLookDir(void);
	void	AddRap(void);
	int		GetRap(void);
	int		GetLastHitRoadPoly(void);
	void	SetLastHitRoadPoly(int);
	void	TimerStart(void);
	void	TimerStop(void);
	int		GetTimerFrame(void);
	bool	IsTimerActive(void);
	void	SetAccelKind(AccelKind);
	AccelKind	GetAccelKind(void);
	DirectX::XMFLOAT3	GetSpeed(void);

private:
	Model*				m_pModel;
	VertexShader*		m_pVS;

	CameraBase*			m_pCamera;
	DirectX::XMFLOAT3	m_pos;
	Texture*			m_pShadowTexture;
	DirectX::XMFLOAT3	m_lookDir;
	DirectX::XMFLOAT3	m_speed;
	Texture*			m_engelWing;
	Effect*				m_pEffect;
	MoveEmitter*		m_pMoveEmitter;
	TrailEffect*		m_pTrailEffect;
	Texture*			m_pTrailTex;
	int					m_nCntRap;	//周回数
	int					m_nLastHitRoadPoly;
	int					m_nFrameTimer;
	bool				m_bIsActiveTimer;
	float				m_fAccelLen;
	float				m_fMaxSpeed;
	AccelKind			m_accelKind;
	int					m_nCntSpeedUpFrame;

	// ギア関連
	int m_currentGear; // 現在のギア
	float m_engineRPM; // エンジン回転数
	bool m_bClutchPressed; // クラッチ操作状態
	void UpdateGearSystem(); // ギアシステムの更新

	static const float	ROT_SPEED;
	static const float	DECREASE_SPEED;
	static const int	SPEEDUPITEM_FRAME;

	static const float GEAR_RATIOS[6]; // 各ギアの比率
	static const float MAX_RPM;
	static const float SHIFT_UP_RPM;
	static const float SHIFT_DOWN_RPM;
};

#endif