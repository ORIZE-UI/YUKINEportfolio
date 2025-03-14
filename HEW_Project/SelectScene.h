#pragma once
#ifndef __SELECT_SCENE_H__
#define __SELECT_SCENE_H__

//インクルード部
#include <DirectXMath.h>
#include "Texture.h"
#include "Sprite.h"
#include "Fade.hpp"
#include "Curtain.h"
#include "Shader.h"
#include "CameraBase.h"
#include "Objectcamera.h"
#include "CameraObject.h"

#include "BackShadow.h"
#include "Soundtest.h"

class SceneManager;

enum class CameraKindSelect
{
	CAM_OBJ,	//
	CAM_SHADOW,	//レンダー用カメラ
	CAM_DEBUG,	//でバック用のカメラ
	MAX_CAMERA	//カメラ最大数
};


class SelectScene
{
public:
	SelectScene();
	~SelectScene();
	void Update(SceneManager* pSceneManager);
	void Draw();
	void BackUIDraw();
	void MenuUIDraw();
	void StartUIDraw();
	void Stage1Draw();
	void Stage2Draw();
	void Stage3Draw();
	void Stage4Draw();
	void Stage5Draw();
	int GetSelectStage() const { return m_SelectStage; }

	void CursorUIDraw(float x, float y, float z);	//カーソル描画
	void SetPosition(float x, float y, float z);	//カーソル位置を設定
	DirectX::XMFLOAT3 GetPosition() const;

private:
	Texture*		 m_pTexture;
	Texture*		 m_pBackTexture;
	Texture*		 m_pMenuTexture;
	Texture*		 m_pStartTexture;
	Texture*		 m_pCursorTexture;
	Texture*		 m_pStage1Texture;
	Texture*		 m_pStage2Texture;
	Texture*		 m_pStage3Texture;
	Texture*		 m_pStage4Texture;
	Texture*		 m_pStage5Texture;

	SceneManager*	 m_pSceneManager;
	Fade*			 m_pFade;
	Screen*			 m_pScreen;

	VertexShader*	 m_pVS;
	PixelShader*	 m_pPS;

	ObjectCamera*	 m_pobjcamera;
	CameraKindSelect m_mainCamera;
	CameraBase*		m_pCamera[static_cast<int>(CameraKindSelect::MAX_CAMERA)];

	CurtainUI*		 m_pCurtainUI;

	RenderTarget* m_pRTV;
	DepthStencil* m_pDSV;

	int m_SelectStage;

	IXAudio2SourceVoice* m_pSourceVoiceSelect;	 // サウンドソース
	XAUDIO2_BUFFER*		 m_pSoundSelect;			 // サウンドバッファ
	//Sound m_Sound;

	DirectX::XMFLOAT3 m_CursorPos;	//カーソルの現在の位置

};
#endif
