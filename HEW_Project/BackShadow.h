#pragma once
//#ifndef ___BACK_SHADOW_H___
//#define ___BACK_SHADOW_H___

#include "Shader.h"
#include "CameraShadow.h"

#include "Player.h"
#include "ObjectManager.h"
#include "ShadowP.h"
#include "Screen.h"

#include "Coin.h"
#include "Goal.h"

#include "LibEffekseer.h"
#include "Effekseer/Effekseer.h"

class BackShadow
{
public:
	typedef struct {
		int maxX;	// 最大サイズX
		int minX;	// 最小サイズX
		int maxY;	// 最大サイズY
		int minY;	// 最小サイズY
	}Box;			// 当たり判定BOX

public:
	BackShadow();
	~BackShadow();

	void Update(float tick);
	void Draw(ObjectCamera* m_pobjcamera, ObjectMng* Obj, Coin* Coin1, Coin* Coin2, Coin* Coin3, Goal* Goal);
	void SetShadowCamera(CameraBase* pCamera);
	void ShadowCollision(int nFeetAlpha, int nBodyAlpha, int nHeadAlpha);
	void ShadowBackCollision(int nFeetAlpha, int nBodyAlpha);
	bool ShadowWarningCollision(int nLeftAlpha, int nRightAlpha);
	bool ShadowDeathCollision(int nLeftAlphaIN, int nRightAlphaIN, int nLeftAlphaOUT, int nRightAlphaOUT);
	void ShadowUnderCollision(BYTE underAlpha, BYTE underAlpha2);
	bool ShadowEdgeCollision(int h, UINT width);
	void CoinCollection(Coin* Coin1, Coin* Coin2, Coin* Coin3);//コインの所得処理
	void GoalCollision(Goal* Goal);//ゴール
	bool IsHit(Box Box1, Box Box2);
	bool IsDeath() { return m_pShadowPlayer->IsDeath(); } // 死亡判定
	

private:
	// カメラ
	CameraBase* m_pCamera;	// レンダーのカメラ

	// レンダー
	Texture* m_pRTVTexture;	// レンダーテクスチャー
	RenderTarget* m_pRTV_BS;// レンダーターゲット
	DepthStencil* m_pDSV_BS;// 深度バッファ

	//影の当たり判定用
	DirectX::XMFLOAT3 m_SPpos;	// 影プレイヤーの位置XYZ
	int m_indexX;				// 影プレイヤーの配列のインデックスX
	int m_indexY;				// 影プレイヤーの配列のインデックスY
	int m_castPosX;				// 影プレイヤーの位置の変数Xのfloatをintにcastする用
	int m_castPosY;				// 影プレイヤーの位置の変数Yのfloatをintにcastする用
	float m_SPposX;				// 影プレイヤーの位置のXの値
	float m_SPposY;				// 影プレイヤーの位置のYの値
	BYTE m_alpha;				// レンダーウィンドウのα値
	BYTE m_alpha2;				// レンダーウィンドウのα値(サブ)
	BYTE m_underAlpha;			// 足元のα値
	BYTE m_underAlpha2;			// 足元のめり込み解消
	BYTE m_PleyerSenter;		// 影君のデス判定用
	bool m_collisionFlag;		// 当たり判定をとったらtrueになる
	int m_nFeetAlpha;			// 足元のα値の個数
	int m_nBodyAlpha;			// 胴体のα値の個数
	int m_nHeadAlpha;			// 頭のα値の個数
	int m_nBackFeetAlpha;		// 後ろの足元のα値の個数
	int m_nBackBodyAlpha;		// 後ろの胴体のα値の個数
	int m_nBackHeadAlpha;		// 後ろの頭のα値の個数
	int m_nWarningRAlpha;		// 警戒判定右側のα値の個数
	int m_nWarningLAlpha;		// 警戒判定左側のα値の個数
	int m_nDeathRAlpha;			// 死亡判定右側のα値の個数
	int m_nDeathLAlpha;			// 死亡判定左側のα値の個数
	bool m_LRcheck;				// 進行方向確認
	
	//影のみに描きたい物
	ShadowP* m_pShadowPlayer;	// 影プレイヤー

	//1コイン
	DirectX::XMFLOAT3 m_1Cpos;
	int m_cast1CposX;
	int m_cast1CposY;

	//2コイン
	DirectX::XMFLOAT3 m_2Cpos;
	int m_cast2CposX;
	int m_cast2CposY;

	//3コイン
	DirectX::XMFLOAT3 m_3Cpos;
	int m_cast3CposX;
	int m_cast3CposY;

	//コインのサイズ
	DirectX::XMFLOAT3 m_Csize;
	int m_castCsizeX;
	int m_castCsizeY;

	//ゴール
	DirectX::XMFLOAT3 m_Goalpos;
	int m_castGoalposX;
	int m_castGoalposY;

	//ゴールのサイズ
	DirectX::XMFLOAT3 m_Goalsize;
	int m_castGoalsizeX;
	int m_castGoalsizeY;

	//スクリーン
	Screen* m_pScreen;


	//sound
	IXAudio2SourceVoice* m_pSVSESdCoin; // サウンドソース
	XAUDIO2_BUFFER* m_pSDSESdCoin; // サウンドバッファ


	PixelShader* m_pPS[3];

	int m_PS; // ピクセルシェーダーの選択

	Effekseer::Handle m_EffectHandle; // エフェクトハンドル
	Effekseer::EffectRef m_EffectCoin; // エフェクト Coin
	Effekseer::EffectRef m_EffectGoal; // エフェクト Goal

};

//#endif // !___BACK_SHADOW_H___

