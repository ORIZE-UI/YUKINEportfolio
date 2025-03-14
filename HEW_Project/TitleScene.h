#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

// インクルード部
#include <DirectXMath.h>
#include "Texture.h"
#include "Sprite.h"
#include "Fade.hpp"
#include "Curtain.h"

class SceneManager;
//#include "SceneManager.hpp"

#include "Model.h"
#include "Shader.h"
#include "CameraBase.h"
#include "Objectcamera.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Object.h"
#include"CameraObject.h"

#include"BackShadow.h"
#include "Soundtest.h"
#include"Coin.h"
#include"CoinCntUI.h"
#include"ItemUI.h"





enum class CameraKindTitle
{
	CAM_OBJ,	//
	CAM_SHADOW,	//レンダー用カメラ
	CAM_DEBUG,	//でバック用のカメラ
	MAX_CAMERA	//カメラ最大数
};


class SceneTitle
{
public:
	SceneTitle();
	~SceneTitle();
	void Update(SceneManager* pSceneManager);
	void Draw();

private:
	Texture*		m_pTexture;
	Fade*			m_pFade;
	CurtainUI*		m_pCurtainUI;

	Screen*			m_pScreen;

	ObjectMng*		m_pObjectMng;
	VertexShader*	m_pVS;

	ObjectCamera*	m_pobjcamera;
	CameraKindTitle	m_mainCamera;
	CameraBase*		m_pCamera[static_cast<int>(CameraKindTitle::MAX_CAMERA)];

	BackShadow*		m_pBackShadow;

	ItemUI*			m_pUI;
	Coin*			m_pCoin;
	CoinCntUI*		m_pCoinCntUI;

	PixelShader*	m_pPS;

	RenderTarget* m_pRTV;
	DepthStencil* m_pDSV;

	IXAudio2SourceVoice*  m_pSourceTitle; // サウンドソース
	XAUDIO2_BUFFER*  m_pSoundTitle; // サウンドバッファ
	//Sound m_Sound;
};

#endif // __SCENE_TUTORIAL_H__
