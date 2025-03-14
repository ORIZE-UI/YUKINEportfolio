#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Model.h"
#include "Shader.h"

#include "CameraBase.h"
#include "Objectcamera.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Object.h"
//#include"CameraObject.h"

#include "BackShadow.h"

#include "Soundtest.h"

#include "Coin.h"
#include "CoinCntUI.h"
#include "ItemUI.h"
#include "Goal.h"
#include "Curtain.h"
#include "Haikei.h"

#include "Fade.hpp"
#include"CurtainObject.h"
#include "SceneManager.hpp"

#include "Texture.h"
#include "Sprite.h"

class SceneManager;

enum CameraKind
{
	CAM_OBJ,	//
	CAM_SHADOW,	//レンダー用カメラ
	CAM_DEBUG,	//でバック用のカメラ
	MAX_CAMERA	//カメラ最大数
};


class SceneGame
{
public:
	SceneGame(int selectNum = 0);
	~SceneGame();
	void Update(SceneManager* pSceneManager, float tick);
	void Draw();

	int GetCoinNum1();	//リザルトにコインの情報を渡す
	int GetCoinNum2();
	int GetCoinNum3();

private:
	Screen* m_pScreen;

	ObjectMng* m_pObjectMng;
	VertexShader* m_pVS;

	ObjectCamera* m_pobjcamera;

	CameraKind		m_mainCamera;
	CameraBase*		m_pCamera[MAX_CAMERA];

	BackShadow*		m_pBackShadow;

	ItemUI*			m_pUI;
	Coin*			m_pCoin;
	CoinCntUI*		m_pCoinCntUI;
	CurtainUI*		m_pCurtainUI;
	Haikei*         m_pHaikei;

	Curtain*		m_pCurtain;
	//ゴール（仮状態の処理有）
	Goal*			m_pGoal;

	// フェードイン・アウト
	Fade* 		m_pFade;

	RenderTarget*	m_pRTV;
	DepthStencil*	m_pDSV;

	IXAudio2SourceVoice* m_pSourceVoice; // サウンドソース
	XAUDIO2_BUFFER* m_pSound; // サウンドバッファ

	IXAudio2SourceVoice* m_pSourceVoiceGoal; // サウンドソース
	XAUDIO2_BUFFER* m_pSoundGoal; // サウンドバッファ
	//Sound m_Sound;

	SceneManager* m_pSceneManager;

	bool m_bGoal = false;

	
	Texture* m_pGoalTexture;
	Texture* m_pDeadTexture;

	PixelShader* m_pPS;
};

#endif // __SCENE_GAME_H__
