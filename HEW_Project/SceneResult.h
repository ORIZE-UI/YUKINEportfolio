#ifndef __SCENE_RESULT_H__
#define __SCENE_RESULT_H__

// インクルード部
#include <DirectXMath.h>
#include "Texture.h"
#include "Sprite.h"
#include "Fade.hpp"
#include "Shader.h"
#include "CameraBase.h"
#include "Objectcamera.h"
#include "CameraObject.h"
#include "BackShadow.h"
#include "Soundtest.h"
#include "SceneGame.h"

class SceneManager;

enum class CameraKindResult
{
	CAM_OBJ,	//
	CAM_SHADOW,	//レンダー用カメラ
	CAM_DEBUG,	//でバック用のカメラ
	MAX_CAMERA	//カメラ最大数
};


class SceneResult
{
public:
	SceneResult();
	~SceneResult();
	void Update(SceneManager* pSceneManager);
	void BGDraw();
	void ClearDraw();
	void NextDraw();
	void ResultCoinDraw(float x);

private:
	Texture*		 m_pBGTexture;
	Texture*		 m_pClearIcon;
	Texture*		 m_pNextIcon;
	Texture*		 m_pCoinIcon;

	SceneGame*		 m_pSceneGame;
	SceneManager*	 m_pSceneManager;
	Fade*			 m_pFade;
	Screen*			 m_pScreen;

	VertexShader*	 m_pVS;
	PixelShader*	 m_pPS;

	ObjectCamera*	 m_pobjcamera;
	CameraKindResult m_mainCamera;
	CameraBase*		 m_pCamera[static_cast<int>(CameraKindResult::MAX_CAMERA)];

	CurtainUI*		 m_pCurtainUI;

	int m_GetCoinNum1 = 0;
	int m_GetCoinNum2 = 0;
	int m_GetCoinNum3 = 0;

	RenderTarget* m_pRTV;
	DepthStencil* m_pDSV;

	IXAudio2SourceVoice* m_pSourceVoiceResult; // サウンドソース
	XAUDIO2_BUFFER*		 m_pSoundResult; // サウンドバッファ


	IXAudio2SourceVoice* m_pSourceVoiceClear; // サウンドソース
	XAUDIO2_BUFFER*		 m_pSoundResultClear; // サウンドバッファ
	//Sound m_Sound;
};

#endif // __SCENE_RESULT_H__

