#include "BackShadow.h"
#include "Sprite.h"
#include "Geometory.h"
#include "Input.h"
#include <iostream>
#include <string>

#define RTV_3D_SIZE_WIDTH	(1280.0f / 1.3f)	// 3D空間上のレンダーの表示サイズX
#define RTV_3D_SIZE_HEIGHT	(-720.0f / 1.3f)	// 3D空間上のレンダーの表示サイズY
#define RTV_3D_POS_WIDTH	(640.0f)			// 3D空間上のレンダー表示の原点X
#define RTV_3D_POS_HEIGHT	(400.0f)			// 3D空間上のレンダー表示の原点Y
#define SHADOWPLAYER_SIZE_Y	(110)				// 影Playerの縦幅
#define SHADOWPLAYER_SIZE_X	(30)				// 影Playerの横幅の半分
#define RESET_POSISHION		(2000)				// 影コインの取得後のポジション


int testw = 10;
int testh = 10;

BackShadow::Box BoxCoin1;
BackShadow::Box BoxCoin2;
BackShadow::Box BoxCoin3;
BackShadow::Box BoxGoal;
BackShadow::Box BoxShadowPlayer;

BackShadow::BackShadow()
	: m_pRTVTexture(nullptr)
	, m_pRTV_BS(nullptr)
	, m_pDSV_BS(nullptr)
	, m_pCamera(nullptr)
	, m_pShadowPlayer(nullptr)
	, m_pScreen(nullptr)//27
	, m_indexX(0)
	, m_indexY(0)
	, m_castPosX(0)
	, m_castPosY(0)
	, m_SPposX(0.0f)
	, m_SPposY(0.0f)
	, m_SPpos(0.0f, 0.0f, 0.0f)
	, m_alpha{0}
	, m_alpha2{0}
	, m_underAlpha{0}
	, m_underAlpha2{0}
	, m_PleyerSenter{0}
	, m_nFeetAlpha(0)
	, m_nBodyAlpha(0)
	, m_nHeadAlpha(0)
	, m_nBackFeetAlpha(0)
	, m_nBackBodyAlpha(0)
	, m_nBackHeadAlpha(0)
	, m_nWarningRAlpha(0)
	, m_nWarningLAlpha(0)
	, m_nDeathRAlpha(0)
	, m_nDeathLAlpha(0)
	, m_1Cpos(0.0f, 0.0f, 0.0f)		// コイン1の座標
	, m_cast1CposX(0)				// コイン1のX変換座標用
	, m_cast1CposY(0)				// コイン1のY変換座標用
	, m_2Cpos(0.0f, 0.0f, 0.0f)		// コイン2の座標
	, m_cast2CposX(0)		   		// コイン2のX変換座標用
	, m_cast2CposY(0)		   		// コイン2のY変換座標用
	, m_3Cpos(0.0f, 0.0f, 0.0f)		// コイン3の座標
	, m_cast3CposX(0)				// コイン3のX変換座標用
	, m_cast3CposY(0)				// コイン3のY変換座標用
	, m_castCsizeX(0)				// コインのXサイズ
	, m_castCsizeY(0)				// コインのYサイズ
	, m_Goalpos(0.0f,0.0f,0.0f)		// ゴールの座標
	, m_castGoalposX(0)
	, m_castGoalposY(0)
	, m_castGoalsizeX(0)			// ゴールのXサイズ
	, m_castGoalsizeY(0)			// ゴールのYサイズ
	, m_collisionFlag(false)
	, m_LRcheck(false)
	,m_pSDSESdCoin(nullptr)
	,m_pSVSESdCoin(nullptr)
	,m_PS(1)

{
	// レンダー表示関連の確保
	m_pRTVTexture = new Texture;
	m_pRTV_BS = new RenderTarget;
	m_pDSV_BS = new DepthStencil;

	// レンダーターゲット制作
	m_pRTV_BS->Create(DXGI_FORMAT_R8G8B8A8_UNORM, 1280/* / 2*/, 720/* / 2*/);// レンダー内の大きさ
	m_pDSV_BS->Create(m_pRTV_BS->GetWidth(), m_pRTV_BS->GetHeight(), true);
	SetRenderTargets(1, &m_pRTV_BS, m_pDSV_BS);

	// 影のみに表示するものの確保
	m_pShadowPlayer = new ShadowP();
	m_pScreen = new Screen();//27

	// コピー用フォーマット作成
	m_pRTV_BS->CreateReadBuffer();

	// 影の当たり判定用変数初期化
	m_SPpos = m_pShadowPlayer->NowPos();

	// 進行方向初期化
	m_LRcheck = m_pShadowPlayer->isUse();

	m_pSDSESdCoin = LoadSound("Assets/Sound/SE/Coinkaisyuu_Oobayashi.wav");

	// PixelShader
	m_pPS[0] = new PixelShader;
	m_pPS[1] = new PixelShader;
	m_pPS[2] = new PixelShader;
	m_pPS[0]->Load("Assets/Shader/PS_Shadow.cso");
	m_pPS[1]->Load("Assets/Shader/PS_Binarization.cso");
	m_pPS[2]->Load("Assets/Shader/PS_Sprite.cso");

	m_EffectCoin = LibEffekseer::Create("Assets/effect/coinGet.efkefc");
	m_EffectGoal = LibEffekseer::Create("Assets/effect/Goal.efkefc");
}

BackShadow::~BackShadow()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_pPS[i])
		{
			delete m_pPS[i];
			m_pPS[i] = nullptr;
		}
	}
	if (m_pRTVTexture)
	{
		delete m_pRTVTexture;
		m_pRTVTexture = nullptr;
	}
	if (m_pShadowPlayer)
	{
		delete m_pShadowPlayer;
		m_pShadowPlayer = nullptr;
	}
}

/**
 * @fn
 * 影(レンダー)の更新
 * 影プレイヤーの更新
 */
void BackShadow::Update(float tick)
{

	m_pShadowPlayer->Update(tick);

	
}

/**
 * @fn
 * 基本動作　＝　影(レンダー)の描画
 * 基本以外　＝　影のテクスチャ情報を取得、当たり判定用変数の更新
 * @brief 影(レンダー)の描画
 * @param (Player* Player) プレイヤーのポインタ
 * @param (ObjectMng* Obj) オブジェのポインタ
 * @sa 参照すべき関数を書けばリンクが貼れる
 * @detail 3D空間にあるものを表示したいときは「引数を増やす」こと
 */
void BackShadow::Draw(ObjectCamera* m_pobjcamera, ObjectMng* Obj, Coin* Coin1, Coin* Coin2, Coin* Coin3, Goal* Goal)
{
	// レンダーの色設定し、その色で画面クリア
	float color[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	m_pRTV_BS->Clear(color);
	//m_pDSV_BS->Clear();

	// レンダーの設定
	SetRenderTargets(1, &m_pRTV_BS, nullptr);
	// ここに表示したいものを持ってくる

	DirectX::XMFLOAT4X4 viewMatrix = m_pCamera->GetViewMatrix();
	DirectX::XMFLOAT4X4 projectionMatrix = m_pCamera->GetProjectionMatrix();

	// 影のブロックの描画
    Obj->Draw(viewMatrix, projectionMatrix,false);	// レンダーターゲット
	m_pShadowPlayer->Draw(viewMatrix, projectionMatrix);

	// ゴールをフィールド上に表示
	if (Goal->IsGoal == false)
		//===============移動チュートリアル================
	//{	Goal->Draw(1000.0f, 300.0f, 0.0f, 60.0f, 60.0f);	}
	//===============拡縮チュートリアル================
	//{	Goal->Draw(1000.0f, 300.0f, 0.0f, 60.0f, 60.0f);	}
	//===============ジャンプチュートリアル================
	//{	Goal->Draw(900.0f, 100.0f, 0.0f, 60.0f, 60.0f);	}
	//===============stage1================
	//{ Goal->Draw(1050.0f, 100.0f, 0.0f, 60.0f, 60.0f);	}
	//===============stage2================
	//{	Goal->Draw(1050.0f, 100.0f, 0.0f, 60.0f, 60.0f);	}
	//===============stage3================
	//{	Goal->Draw(1050.0f, 100.0f, 0.0f, 60.0f, 60.0f);	}
	//===================stage4=========================
	//{	Goal->Draw(1230.0f, 70.0f, 0.0f, 60.0f, 60.0f);	}
	//=======================stage5========================
	//{ Goal->Draw(800.0f, 50.0f, 0.0f, 60.0f, 60.0f);	}
	//=========================stage6=====================
	{ Goal->Draw(1100.0f, 100.0f, 0.0f, 60.0f, 60.0f);	}




	// UI
	// コインをフィールド上に表示
	// Drawの１,２個目の数値をいじればコイン描画座標が変わる
	//======================移動チュートリアル==================
	//if (Coin1->IsCoinCollected == false)
	//{	Coin1->Draw(270.0f, 300.0f, 0.0f,40.0f, 40.0f, 1);	}	// 左 y=120.0f
	//if (Coin2->IsCoinCollected == false)
	//{	Coin2->Draw(500.0f, 320.0f, 0.0f, 40.0f, 40.0f, 2);	}	// 真ん中
	//if (Coin3->IsCoinCollected == false)
	//{	Coin3->Draw(770.0f, 250.0f, 0.0f, 40.0f, 40.0f, 3);}	// 右

	////======================拡縮チュートリアル==================
	//if (Coin1->IsCoinCollected == false)
	//{	Coin1->Draw(270.0f, 300.0f, 0.0f,40.0f, 40.0f, 1);	}	// 左 y=120.0f
	//if (Coin2->IsCoinCollected == false)
	//{	Coin2->Draw(500.0f, 200.0f, 0.0f, 40.0f, 40.0f, 2);	}	// 真ん中
	//if (Coin3->IsCoinCollected == false)
	//{	Coin3->Draw(800.0f, 250.0f, 0.0f, 40.0f, 40.0f, 3);}	// 右

	//======================ジャンプチュートリアル==================
	//if (Coin1->IsCoinCollected == false)
	//{	Coin1->Draw(270.0f, 300.0f, 0.0f,40.0f, 40.0f, 1);	}	// 左 y=120.0f
	//if (Coin2->IsCoinCollected == false)
	//{	Coin2->Draw(500.0f, 200.0f, 0.0f, 40.0f, 40.0f, 2);	}	// 真ん中
	//if (Coin3->IsCoinCollected == false)
	//{	Coin3->Draw(800.0f, 250.0f, 0.0f, 40.0f, 40.0f, 3);}	// 右

		//======================stage1==================
	//if (Coin1->IsCoinCollected == false)
	//{
	//	Coin1->Draw(350.0f, 200.0f, 0.0f, 40.0f, 40.0f, 1);
	//}	// 左 y=120.0f
	//if (Coin2->IsCoinCollected == false)
	//{
	//	Coin2->Draw(600.0f, 200.0f, 0.0f, 40.0f, 40.0f, 2);
	//}	// 真ん中
	//if (Coin3->IsCoinCollected == false)
	//{
	//	Coin3->Draw(800.0f, 250.0f, 0.0f, 40.0f, 40.0f, 3);
	//}	// 右

//======================stage2==================
//if (Coin1->IsCoinCollected == false)
//{	Coin1->Draw(350.0f, 200.0f, 0.0f,40.0f, 40.0f, 1);	}	// 左 y=120.0f
//if (Coin2->IsCoinCollected == false)
//{	Coin2->Draw(600.0f, 200.0f, 0.0f, 40.0f, 40.0f, 2);	}	// 真ん中
//if (Coin3->IsCoinCollected == false)
//{	Coin3->Draw(900.0f, 150.0f, 0.0f, 40.0f, 40.0f, 3);}	// 右

	//======================stage3==================
//if (Coin1->IsCoinCollected == false)
//{	Coin1->Draw(350.0f, 200.0f, 0.0f,40.0f, 40.0f, 1);	}	// 左 y=120.0f
//if (Coin2->IsCoinCollected == false)
//{	Coin2->Draw(600.0f, 200.0f, 0.0f, 40.0f, 40.0f, 2);	}	// 真ん中
//if (Coin3->IsCoinCollected == false)
//{	Coin3->Draw(900.0f, 150.0f, 0.0f, 40.0f, 40.0f, 3);}	// 右

//=======================stage4=========================
//if (Coin1->IsCoinCollected == false)
//{	Coin1->Draw(270.0f, 70.0f, 0.0f,40.0f, 40.0f, 1);	}	// 左 y=120.0f
//if (Coin2->IsCoinCollected == false)
//{	Coin2->Draw(500.0f, 270.0f, 0.0f, 40.0f, 40.0f, 2);	}	// 真ん中
//if (Coin3->IsCoinCollected == false)
//{	Coin3->Draw(1200.0f, 270.0f, 0.0f, 40.0f, 40.0f, 3);}	// 右

	//=======================stage5=========================
	//if (Coin1->IsCoinCollected == false)
	//{
	//	Coin1->Draw(600.0f, 180.0f, 0.0f, 40.0f, 40.0f, 1);
	//}	// 左 y=120.0f
	//if (Coin2->IsCoinCollected == false)
	//{
	//	Coin2->Draw(600.0f, 300.0f, 0.0f, 40.0f, 40.0f, 2);
	//}	// 真ん中
	//if (Coin3->IsCoinCollected == false)
	//{
	//	Coin3->Draw(800.0f, 250.0f, 0.0f, 40.0f, 40.0f, 3);
	//}	// 右

	//========================stage6=========================
	if (Coin1->IsCoinCollected == false)
	{
		Coin1->Draw(300.0f, 150.0f, 0.0f, 40.0f, 40.0f, 1);
	}	// 左 y=120.0f
	if (Coin2->IsCoinCollected == false)
	{
		Coin2->Draw(600.0f, 300.0f, 0.0f, 40.0f, 40.0f, 2);
	}	// 真ん中
	if (Coin3->IsCoinCollected == false)
	{
		Coin3->Draw(950.0f, 150.0f, 0.0f, 40.0f, 40.0f, 3);
	}	// 右

	RenderTarget* pRTV;
	pRTV = GetDefaultRTV();
	SetRenderTargets(1, &pRTV, nullptr); 

	// レンダーターゲットの色情報読み取り
	m_pRTV_BS->Read([&](const void* colorData, UINT width, UINT height) {
		struct Color {
			BYTE r, g, b, a;
		};

		// レンダー	データサイズ情報
		// X軸は左が 0 右が 640
		// Y軸は上が 0 下が 360

		// 当たり判定用変数初期化
		int shadowSizeX = SHADOWPLAYER_SIZE_X;
		int shadowSizeY = SHADOWPLAYER_SIZE_Y;
		// レンダー当たり判定用変数更新
		m_SPpos = m_pShadowPlayer->NowPos();				// 影の座標を所得
		m_SPposX = ((m_SPpos.x - 5.0f) / 10.0f) * (-1);		// X軸をレンダーのウィンドウ座標に合わせて変換
		m_SPposY = ((m_SPpos.y - 3.0f) / 6.0f) * (-1);		// Y軸をレンダーのウィンドウ座標に合わせて変換
		// キャスト
		m_castPosX = static_cast<int>(m_SPposX * width);
		m_castPosY = static_cast<int>(m_SPposY * height);
		m_indexX = m_castPosX;
		m_indexY = m_castPosY;

		// ゴール
		if (Goal->IsGoal == false)
		{	m_Goalpos = Goal->GetPosition();}
		m_Goalsize = Goal->GetSize();

		// コイン1
		if (Coin1->IsCoinCollected == false)
		{	m_1Cpos = Coin1->GetPosition();	}
		m_Csize = Coin1->GetSize();
		// コイン2
		if (Coin2->IsCoinCollected == false)
		{	m_2Cpos = Coin2->GetPosition();	}
		m_Csize = Coin2->GetSize();
		// コイン3
		if (Coin3->IsCoinCollected == false)
		{	m_3Cpos = Coin3->GetPosition();	}
		m_Csize = Coin3->GetSize();

		// ゴールの座標変換
		m_castGoalposX = static_cast<int>(m_Goalpos.x);
		m_castGoalposY = static_cast<int>(m_Goalpos.y);
		// ゴールのサイズ変換
		m_castGoalsizeX = static_cast<int>(m_Goalsize.x);
		m_castGoalsizeY = static_cast<int>(m_Goalsize.y);

		// コインの座標変換
		// コイン1
		m_cast1CposX = static_cast<int>(m_1Cpos.x);
		m_cast1CposY = static_cast<int>(m_1Cpos.y);
		// コイン2
		m_cast2CposX = static_cast<int>(m_2Cpos.x);
		m_cast2CposY = static_cast<int>(m_2Cpos.y);
		// コイン3
		m_cast3CposX = static_cast<int>(m_3Cpos.x);
		m_cast3CposY = static_cast<int>(m_3Cpos.y);
		// コインのサイズ変換
		m_castCsizeX = static_cast<int>(m_Csize.x);
		m_castCsizeY = static_cast<int>(m_Csize.y);

		// プレイヤーの位置とスクリーン左上の座標の差分を、スクリーンの横幅で割ると配列のインデックスが求められる
		// 影のプレイヤーはレンダーにのみ表示している->レンダー幅が影プレイヤーのウィンドウ幅
		// ただし、今回のプレイヤーの位置とスクリーン左上の座標の差分はないためそのままで、かつ、元の値を0～1にしているため掛け算を使用
		// (例)(int)((m_pos.x - screenPos.x) / screenWidth)

		// コイン1
		BoxCoin1.maxX = m_cast1CposX + (m_castCsizeX - 5);	// 右辺
		BoxCoin1.minX = m_cast1CposX - (m_castCsizeX - 5);	// 左辺
		BoxCoin1.maxY = m_cast1CposY + (m_castCsizeX - 5);	// 下辺
		BoxCoin1.minY = m_cast1CposY - (m_castCsizeX - 5);	// 上辺
		// コイン2
		BoxCoin2.maxX = m_cast2CposX + (m_castCsizeX - 5);	// 右辺
		BoxCoin2.minX = m_cast2CposX - (m_castCsizeX - 5);	// 左辺
		BoxCoin2.maxY = m_cast2CposY + (m_castCsizeX - 5);	// 下辺
		BoxCoin2.minY = m_cast2CposY - (m_castCsizeX - 5);	// 上辺
		// コイン3
		BoxCoin3.maxX = m_cast3CposX + (m_castCsizeX - 5);	// 右辺
		BoxCoin3.minX = m_cast3CposX - (m_castCsizeX - 5);	// 左辺
		BoxCoin3.maxY = m_cast3CposY + (m_castCsizeX - 5);	// 下辺
		BoxCoin3.minY = m_cast3CposY - (m_castCsizeX - 5);	// 上辺
		// ゴール
		BoxGoal.maxX  = m_castGoalposX + (m_castGoalsizeX - 5);	// 右辺
		BoxGoal.minX  = m_castGoalposX - (m_castGoalsizeX - 5);	// 左辺
		BoxGoal.maxY  = m_castGoalposY + (m_castGoalsizeY - 5);	// 下辺
		BoxGoal.minY  = m_castGoalposY - (m_castGoalsizeY - 5);	// 上辺
		// 影プレイヤー
		BoxShadowPlayer.maxX = m_castPosX + shadowSizeX;	// 右辺
		BoxShadowPlayer.minX = m_castPosX - shadowSizeX;	// 左辺
		BoxShadowPlayer.maxY = m_castPosY;					// 下辺
		BoxShadowPlayer.minY = m_castPosY - shadowSizeY;	// 上辺


		// コイン1
		if (Coin1->IsCoinCollected == true)
		{
			BoxCoin1.maxX = RESET_POSISHION;	// 右辺
			BoxCoin1.minX = RESET_POSISHION;	// 左辺
			BoxCoin1.maxY = RESET_POSISHION;	// 下辺
			BoxCoin1.minY = RESET_POSISHION;	// 上辺
		}
		// コイン2
		if (Coin2->IsCoinCollected == true)
		{
			BoxCoin2.maxX = RESET_POSISHION;	// 右辺
			BoxCoin2.minX = RESET_POSISHION;	// 左辺
			BoxCoin2.maxY = RESET_POSISHION;	// 下辺
			BoxCoin2.minY = RESET_POSISHION;	// 上辺
		}
		// コイン3
		if (Coin3->IsCoinCollected == true)
		{
			BoxCoin3.maxX = RESET_POSISHION;	// 右辺
			BoxCoin3.minX = RESET_POSISHION;	// 左辺
			BoxCoin3.maxY = RESET_POSISHION;	// 下辺
			BoxCoin3.minY = RESET_POSISHION;	// 上辺
		}
		// ゴール
		if (Goal->GetGoal() == true)
		{
			BoxGoal.maxX = RESET_POSISHION;	// 右辺
			BoxGoal.minX = RESET_POSISHION;	// 左辺
			BoxGoal.maxY = RESET_POSISHION;	// 下辺
			BoxGoal.minY = RESET_POSISHION;	// 上辺
		}

		const Color* pData = reinterpret_cast<const Color*>(colorData);

		// 進行方向チェック
		m_LRcheck = m_pShadowPlayer->isUse();

		// 画面端判定(左右)
		for (UINT h = 0; h < height; ++h)
		{
			for (UINT w = 0; w < width; ++w)
			{
				if (ShadowEdgeCollision(h, width))
				{
					m_pShadowPlayer->Use();
					m_collisionFlag = true;
					break;
				}
			}
			if (m_collisionFlag)
			{
				m_collisionFlag = false;
				break;
			}
		}

		// 足元当たり判定
		m_underAlpha = pData[(m_indexY + 6) * width + m_indexX].a;						// (プレイヤーposY - 高さ) * 横幅 + プレイヤーposX - サイズ - 見たい横幅
		for (int i = 1; i < 4; i++)
		{
			m_underAlpha2 = pData[(m_indexY + i) * width + m_indexX].a;						// (プレイヤーposY - 高さ) * 横幅 + プレイヤーposX - サイズ - 見たい横幅
			// 足元判定
			ShadowUnderCollision(m_underAlpha, m_underAlpha2);
		}
		

		// 必要αデータ初期化
		m_nFeetAlpha = 0;
		m_nBodyAlpha = 0;
		m_nHeadAlpha = 0;
		m_nBackFeetAlpha = 0;
		m_nBackBodyAlpha = 0;

		// 壁、階段当たり判定(ループ)
		for (int j = 0; j < SHADOWPLAYER_SIZE_Y; j++)
		{
			if (m_indexY - j <= 0 || m_indexY - j > 720)
			{// 例外スロー対策
				break;
			}
			if (m_LRcheck == false)
			{
				m_alpha = pData[(m_indexY + 2 - j) * width + m_indexX + SHADOWPLAYER_SIZE_X].a;		// (プレイヤーposY - 高さ) * 横幅 + プレイヤーposX - サイズ - 見たい横幅
				m_alpha2 = pData[(m_indexY + 2 - j) * width + m_indexX - SHADOWPLAYER_SIZE_X].a;	// 背面
			}
			else
			{
				m_alpha = pData[(m_indexY + 2 - j) * width + m_indexX - SHADOWPLAYER_SIZE_X].a;		// (プレイヤーposY - 高さ) * 横幅 + プレイヤーposX - サイズ - 見たい横幅
				m_alpha2 = pData[(m_indexY + 2 - j) * width + m_indexX + SHADOWPLAYER_SIZE_X].a;	// 背面
			}
			// α値の内訳
			if (m_alpha > 240)
			{
				if (j <= SHADOWPLAYER_SIZE_Y - 100)
				{// 足元のα値
					m_nFeetAlpha++;
				}
				else if (j > SHADOWPLAYER_SIZE_Y - 100 && j < SHADOWPLAYER_SIZE_Y - 10)
				{// 胴体のα値
					m_nBodyAlpha++;
				}
				else if (j >= SHADOWPLAYER_SIZE_Y - 10)
				{// 頭のα値
					m_nHeadAlpha++;
				}

				// コイン当たり判定
				CoinCollection(Coin1, Coin2, Coin3);
			}
			if (m_alpha2 > 240)
			{
				if (j <= SHADOWPLAYER_SIZE_Y - 100)
				{// 足元のα値
					m_nBackFeetAlpha++;
				}
				else if (j > SHADOWPLAYER_SIZE_Y - 100 && j < SHADOWPLAYER_SIZE_Y - 10)
				{// 胴体のα値
					m_nBackBodyAlpha++;
				}
			}

		}
		GoalCollision(Goal);
		// 壁、階段当たり判定(関数)
		ShadowCollision(m_nFeetAlpha, m_nBodyAlpha, m_nHeadAlpha);
		ShadowBackCollision(m_nBackFeetAlpha, m_nBackBodyAlpha);

		// あわわわわわ判定
		m_nWarningRAlpha = 0;
		m_nWarningLAlpha = 0;
		for (int i = 0; i < SHADOWPLAYER_SIZE_Y - 30; i++)
		{
			if (m_indexY + 15 - i <= 0 || m_indexY + 15 - i > 720)
			{// 例外スロー対策
				break;
			}
			// 影君の両横の縦長にα値があるか見てる -> |影| こんな感じ
			m_alpha = pData[(m_indexY + 15 - i) * width + m_indexX + (SHADOWPLAYER_SIZE_X + 5)].a;	// (プレイヤーposY - 高さ) * 横幅 + プレイヤーposX - サイズ - 見たい横幅
			if (m_alpha > 240)	// 右
			{	m_nWarningRAlpha++;	}
			m_alpha = pData[(m_indexY + 15 - i) * width + m_indexX - (SHADOWPLAYER_SIZE_X + 5)].a;	// (プレイヤーposY - 高さ) * 横幅 + プレイヤーposX - サイズ - 見たい横幅
			if (m_alpha > 240)	// 左
			{	m_nWarningLAlpha++;	}
			if (ShadowWarningCollision(m_nWarningLAlpha, m_nWarningRAlpha))
			{// α値の合計数で一定数超えていたらtrueを返す
				m_pShadowPlayer->SetKeikai(true);// ここにあわわわ挙動の関数呼び出し
			}
			else
			{
				m_pShadowPlayer->SetKeikai(false);
			}
		}

		// 死亡判定
		m_nWarningRAlpha	= 0;
		m_nWarningLAlpha	= 0;
		m_nDeathRAlpha		= 0;
		m_nDeathLAlpha		= 0;
		for (int i = 0; i < SHADOWPLAYER_SIZE_Y - 30; i++)
		{
			if (m_indexY + 15 - i <= 0 || m_indexY + 15 - i > 720)
			{// 例外スロー対策
				break;
			}
			// 影君の両横の縦長に二重にα値があるか見てる -> ||影|| こんな感じ
			m_alpha = pData[(m_indexY + 15 - i) * width + m_indexX + (SHADOWPLAYER_SIZE_X + 5)].a;	// (プレイヤーposY - 高さ) * 横幅 + プレイヤーposX - サイズ - 見たい横幅
			if (m_alpha > 240)	// 外側右
			{	m_nWarningRAlpha++;	}
			m_alpha = pData[(m_indexY + 15 - i) * width + m_indexX - (SHADOWPLAYER_SIZE_X + 5)].a;	// (プレイヤーposY - 高さ) * 横幅 + プレイヤーposX - サイズ - 見たい横幅
			if (m_alpha > 240)	// 外側左
			{	m_nWarningLAlpha++;	}
			m_alpha = pData[(m_indexY + 15 - i) * width + m_indexX + (SHADOWPLAYER_SIZE_X - 12)].a;	// (プレイヤーposY - 高さ) * 横幅 + プレイヤーposX - サイズ - 見たい横幅
			if (m_alpha > 240)	// 内側右
			{	m_nDeathRAlpha++;	}
			m_alpha = pData[(m_indexY + 15 - i) * width + m_indexX - (SHADOWPLAYER_SIZE_X - 12)].a;	// (プレイヤーposY - 高さ) * 横幅 + プレイヤーposX - サイズ - 見たい横幅
			if (m_alpha > 240)	// 内側左
			{	m_nDeathLAlpha++;	}
			if (ShadowDeathCollision(m_nDeathLAlpha, m_nDeathRAlpha, m_nWarningLAlpha, m_nWarningRAlpha))
			{// α値の合計数で一定数超えていたらtrueを返す
				m_pShadowPlayer->SetDeath(true);// ここに死亡挙動の関数呼び出し
			}
		}
	});



	DirectX::XMFLOAT4X4 mat[3];

	// ワールド行列はXとYのみ考慮
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(RTV_3D_POS_WIDTH, RTV_3D_POS_HEIGHT, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	mat[1] = m_pCamera->GetShadowViewMatrix();
	mat[2] = m_pCamera->GetShadowProjectionMatrix();

	SetSamplerState(SAMPLER_POINT);
	// スプライトの設定

	// 影の表示形式変更
	if (IsKeyPress('1'))
	{
		m_PS = 0;
	}
	if (IsKeyPress('2'))
	{
		m_PS = 1;
	}
	if (IsKeyPress('3'))
	{
		m_PS = 2;
	}

	// エフェクト
	DirectX::XMFLOAT4X4 effectMat[2];
	effectMat[0] = m_pCamera->GetShadowViewMatrix();
	effectMat[1] = m_pCamera->GetShadowProjectionMatrix();

	//std::string str = "Goal X:" + std::to_string(Goal->GetPosition().x) + " Y:" + std::to_string(Goal->GetPosition().y) + " Z:" + std::to_string(Goal->GetPosition().z) + "\n";
	
	//OutputDebugString(str.c_str());


	// Effekseerの仕様上行列を転置前の状態で渡す
	DirectX::XMMATRIX effectView = DirectX::XMLoadFloat4x4(&effectMat[0]);
	effectView = DirectX::XMMatrixTranspose(effectView);
	DirectX::XMStoreFloat4x4(&effectMat[0], effectView);

	DirectX::XMMATRIX effectProj = DirectX::XMLoadFloat4x4(&effectMat[1]);
	effectProj = DirectX::XMMatrixTranspose(effectProj);
	DirectX::XMStoreFloat4x4(&effectMat[1], effectProj);

	Effekseer::Matrix43 EffecMat = LibEffekseer::GetManager()->GetBaseMatrix(m_EffectHandle);
	EffecMat.Translation(0.0f, -1.0f, 0.0f);
	LibEffekseer::GetManager()->SetBaseMatrix(m_EffectHandle, EffecMat);

	LibEffekseer::SetViewPosition(m_pCamera->GetPos());
	LibEffekseer::SetCameraMatrix(effectMat[0], effectMat[1]);

	

	Sprite::SetPixelShader(m_pPS[m_PS]);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetOffset({ 0.0f, 0.0f });
	Sprite::SetSize(DirectX::XMFLOAT2(RTV_3D_SIZE_WIDTH, RTV_3D_SIZE_HEIGHT));
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	Sprite::SetTexture(m_pRTV_BS);
	Sprite::Draw();

	//LibEffekseer::Draw();

	//m_pPS[0]->Bind();
	//m_pPS[0]->WriteBuffer(0, mat);

	Sprite::SetPixelShader(m_pPS[2]);
	SetSamplerState(SAMPLER_LINEAR);
}

/**
 * @fn
 * レンダー用カメラのセット
 */
void BackShadow::SetShadowCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

/**
 * @fn
 * α値を使った当たり判定
 * @brief 影(レンダー)の当たり判定
 * @param (BYTE alpha)	レンダー画面のα値(一ピクセル分)
 * @param (int h)　		レンダー画面の高さの参照値(一ピクセルごと)
 * @param (int w)　		レンダー画面の幅の参照値(一ピクセルごと)
 * @param (UINT width)	レンダー画面の幅
 * @return true or false(何もないならfalseで返す)
 * @detail 今はまだ描画があるか見てるだけ
 */
void BackShadow::ShadowCollision(int nFeetAlpha, int nBodyAlpha, int nHeadAlpha)
{
	// 左右のα値の参照
	if (nHeadAlpha >= 10)
	{// 天井
		m_pShadowPlayer->Use();
		return;
	}
	if (nBodyAlpha > 27)
	{// 壁
		m_pShadowPlayer->Use();
		return;
	}
	if (nFeetAlpha >= 3)
	{// 階段
		m_pShadowPlayer->Jump();
		return;
	}
}

void BackShadow::ShadowBackCollision(int nFeetAlpha, int nBodyAlpha)
{
	// 背面の左右のα値の参照
	if (nBodyAlpha > 30)
	{// 壁
		m_pShadowPlayer->ShadowPpushX();
		return;
	}
	if (nFeetAlpha >= 3)
	{// 階段
		m_pShadowPlayer->Jump();
		return;
	}
}

bool BackShadow::ShadowWarningCollision(int nLeftAlpha, int nRightAlpha)
{
	if (nLeftAlpha > 50 && nRightAlpha > 50)
	{
		return true;
	}
	return false;
}

// 死亡判定
bool BackShadow::ShadowDeathCollision(int nLeftAlphaIN, int nRightAlphaIN, int nLeftAlphaOUT, int nRightAlphaOUT)
{
	if (nLeftAlphaOUT > 50 && nRightAlphaOUT > 50)
	{
		if (nLeftAlphaIN > 50 || nRightAlphaIN > 50)
		{
			return true;
		}
	}
	return false;
}

// 影の足元判定
void BackShadow::ShadowUnderCollision(BYTE underAlpha, BYTE underAlpha2)
{
	// 足元のα値参照
	if (underAlpha > 240)
	{
		m_pShadowPlayer->SetFooting(true);
	}
	else
	{
		m_pShadowPlayer->SetFooting(false);
	}
	if (underAlpha2 > 240)
	{
		m_pShadowPlayer->ShadowPupY();
	}
}

// 画面端(左右)判定
bool BackShadow::ShadowEdgeCollision(int h, UINT width)
{
	if (h * width + 30 == m_indexY * width + m_indexX)
	{// 左の画面端
		return true;
	}
	if (h * width + width - 8 == m_indexY * width + m_indexX)
	{// 右の画面端
		return true;
	}
	return false;
}

//コインの当たり判定＆処理
void BackShadow::CoinCollection(Coin* Coin1, Coin* Coin2, Coin* Coin3)
{
	// コインの取得処理
	// コイン1
	if (IsHit(BoxShadowPlayer, BoxCoin1))
	{
		Coin1->SetCollect(true);
		m_pSVSESdCoin = PlaySound(m_pSDSESdCoin);
		
		float x = Coin1->GetPosition().x;
		// xを100で割る(-のときも考慮)
		if (x < 0)
		{
			x *= -1;
		}
		x = (x - 640.0f) / 1280.0f * 10.0f * (-1.0f);

		float y = Coin1->GetPosition().y;
		if (y < 0)
		{
			y *= -1;
		}
		y = (y - 360.0f) / 720.0f * 6.0f * (-1.0f);

		LibEffekseer::GetManager()->SetScale(m_EffectHandle, 0.05f, 0.05f, 0.05f);
		m_EffectHandle = LibEffekseer::GetManager()->Play(m_EffectCoin, x, y, -2.0f);
	}
	// コイン2
	if (IsHit(BoxShadowPlayer, BoxCoin2))
	{
		Coin2->SetCollect(true);
		m_pSVSESdCoin = PlaySound(m_pSDSESdCoin);
		// エフェクト
		// m_EffectHandle = LibEffekseer::GetManager()->Play(m_EffectCoin, 0.619f, 0.376f, Coin2->GetPosition().z);
		float x = Coin2->GetPosition().x;
		// xを100で割る(-のときも考慮)
		if (x < 0)
		{
			x *= -1;
		}
		x = (x - 640.0f) / 1280.0f * 10.0f * (-1.0f);

		float y = Coin2->GetPosition().y;
		if (y < 0)
		{
			y *= -1;
		}

		y = (y - 360.0f) / 720.0f * 6.0f * (-1.0f);
		LibEffekseer::GetManager()->SetScale(m_EffectHandle, 0.05f, 0.05f, 0.05f);

		m_EffectHandle = LibEffekseer::GetManager()->Play(m_EffectCoin, x, y, -2.0f);

	}
	// コイン3
	if (IsHit(BoxShadowPlayer, BoxCoin3))
	{
		Coin3->SetCollect(true);
		m_pSVSESdCoin = PlaySound(m_pSDSESdCoin);
		// エフェクト
		//m_EffectHandle = LibEffekseer::GetManager()->Play(m_EffectCoin, -4.0f, 0.374f, 0.0f);
		float x = Coin3->GetPosition().x;
		// xを100で割る(-のときも考慮)
		if (x > 1000)
		{
			x *= -1;
		}
		x = (x - 640.0f) / 1280.0f * 10.0f * (-1.0f);

		float y = Coin3->GetPosition().y;
		if (y < 0)
		{
			y *= -1;
		}
		y = (y - 360.0f) / 720.0f * 6.0f * (-1.0f);

		LibEffekseer::GetManager()->SetScale(m_EffectHandle, 0.05f, 0.05f, 0.05f);

		m_EffectHandle = LibEffekseer::GetManager()->Play(m_EffectCoin, x, y, -2.0f);

	}
}

// ゴールの当たり判定＆処理
void BackShadow::GoalCollision(Goal* Goal)
{
	if (IsHit(BoxShadowPlayer, BoxGoal))
	{
		// 影とダイヤ型が重なったらゴールする
		Goal->SetGoal(true);
		if (Goal->GetGoal() != false)
		{
			// エフェクト
			float x = Goal->GetPosition().x;

			if (x > 1000)
			{
				x *= -1;
			}
			x = (x - 640.0f) / 1280.0f * 10.0f * (-1.0f);

			float y = Goal->GetPosition().y;
			if (y < 0)
			{
				y *= -1;
			}
			y = (y - 360.0f) / 720.0f * 6.0f * (-1.0f);
			LibEffekseer::GetManager()->SetScale(m_EffectHandle, 0.5f, 0.5f, 0.5f);

			//m_EffectHandle = LibEffekseer::GetManager()->Play(m_EffectGoal, -4.0f, 1.094f, 0.0f);
			m_EffectHandle = LibEffekseer::GetManager()->Play(m_EffectGoal, x, y, Goal->GetPosition().z);
		}

	}
}

// 短径同士の当たり判定
// 原理→外にいるならfalseを返す
bool BackShadow::IsHit(Box Box1, Box Box2)
{
	// X軸
	if (Box2.maxX < Box1.minX)	return false;
	if (Box1.maxX < Box2.minX)	return false;
	// Y軸
	if (Box2.maxY < Box1.minY)	return false;
	if (Box1.maxY < Box2.minY)	return false;

	return true;
}


