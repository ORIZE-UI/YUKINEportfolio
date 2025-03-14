//Curtain.h
//カーテン表示UI

#pragma once
#ifndef __CURTAIN_UI_H__
#define __CURTAIN_UI_H__

#include "Texture.h"		//Texture.hの導入

class CurtainUI				//カーテンクラス
{
public:
	CurtainUI();			//コンストラクタ
	~CurtainUI();			//デストラクタ

	void Update();			//更新
	void LeftDraw();		//描画
	void RightDraw();

	void StageCurtainDraw();	//ステージカーテン描画

	void Start(bool isUp, float Time); 

	bool IsPlay();			//再生中かどうか

private:
	Texture* m_pTexture;	//テクスチャ

	Texture* m_pStageCurtainTex; //ステージカーテン用テクスチャ

	float m_fPosY; // Y座標 緞帳を下げたり上げたりするため
	float m_fTime; // 時間
	float m_fTotalTime; // 総時間

	float m_fMaxPosY = 500.0f; // 最大Y座標
	float m_fMinPosY = -330.0f; // 最小Y座標

	// 緞帳を下げるか上げるか
	bool m_isUp; // 上げる
};
#endif
