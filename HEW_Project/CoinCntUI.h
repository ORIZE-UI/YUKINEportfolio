//CoinCntUI.h
//集めたコインを表示するUI
//CntはCountの略

#pragma once
#ifndef __COIN_CNT_UI_H__
#define __COIN_CNT_UI_H__

#include "Texture.h"		//Texture.hの導入
#include "Shader.h"

class CoinCntUI				//集めたコインクラス
{
public:
	CoinCntUI();			//コンストラクタ
	~CoinCntUI();			//デストラクタ

	void Update();			//更新
	void Draw();			//描画

private:
	Texture* m_pTexture;
	PixelShader* m_pPS;
};
#endif
