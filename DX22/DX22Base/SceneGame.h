#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Model.h"
#include "Shader.h"
#include "cameraBase.h"
#include "player.h"
#include "itemUI.h"
#include "Field.h"
#include "Road.h"
#include "SceneBase.h"
#include "Fade.h"
#include "number.h"
#include "spItem.h"
#include "signal.h"
#include "goalPoll.h"
#include "meter.h"
#include "CPUCar.h"
#include "Skydome.h"

class SceneGame : public SceneBase
{
public:
	SceneGame();
	~SceneGame();
	void Update(float tick)	override;
	void Draw()	override;

private:
	enum CameraKind
	{
		CAM_PLAYER,
		CAM_DEBUG,
		MAX_CAMERA
	};

	Player*		m_pPlayer;
	CameraBase*	m_apCamera[MAX_CAMERA];
	CameraKind	m_mainCamera;
	ItemUI*		m_pItemUI;
	Field*		m_pField;
	Road*		m_pRoad;
	Fade*		m_pFade;
	Number*		m_pRapNumber;
	Number*		m_pMaxRapNumber;
	Number*		m_pTimerNumber;
	CSpeedItem*	m_pSpeedItem;
	bool		m_bLastHitGoal;
	Signal*		m_pSignal;
	GoalPoll*	m_pGoalPoll;
	Meter*		m_pMeter;
	CPUCar*		m_pCPUCar;
	Skydome*	m_pSkydome;

	static const int	MAX_RAP;
};

#endif