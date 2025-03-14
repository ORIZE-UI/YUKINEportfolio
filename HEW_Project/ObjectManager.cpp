#include "ObjectManager.h"
#include"Input.h"
#include"Gamepad.h"

#define EFFECT_MAX_POS_X		(4.0f)	// 微調整必要 エフェクトが出るXの最大値
#define EFFECT_PLAYER_MAX_POS_X (2.23f)	// playerの移動できるXの最大値

#define EFFECT_MAX_POS_Z		(3.1f)	// エフェクトが出るZの最大値
#define EFFECT_MIN_POS_Z		(0.38f)	// エフェクトが出るZの最小値
#define EFFECT_PLAYER_MAX_POS_Z (3.36f)	// playerが移動できるZの最大値
#define EFFECT_PLAYER_MIN_POS_Z (1.8f)	// playerが移動できるZの最小値


InputManager imanagerO = InputManager();


ObjectMng::ObjectMng(int stage_num)
	: m_pObjects(nullptr)
	, m_pYuka(nullptr)
	, m_pPlayer(nullptr)
	, m_pLift_obj(nullptr)
	, m_pLever(nullptr)
	, m_num(0)
	, m_num1(0)
	, m_num2(0)
	, m_num3(0)
	, m_num4(0)
	, m_num5(0)
	, m_num6(0)
{
	m_pObjectCamera = new CameraDebug();
	aabb = new AABB();
	haabb = new HAABB();
	caabb = new CAABB();

	m_pPlayer = new Player();

	//======各ブロックの配置======
	//すべてのオブジェクトはリスポーン時に位置が重ならないように!!!!
	//リフトは今のｙ軸スケールと上下制限をかえないで　変えると死ぬ
	//左＋　右－　奥ー　手前＋
	//Max_X (2.23f)
	// Min_X (-2.23f)
	//
	// Max_Z (3.36f)
	// Min_Z (1.8f)この値がブロックの配置制限位置
	//


	switch (stage_num)
	{
		//ブロック
		struct Setting { float x, y, z, scaleX, scaleY, scaleZ, hyoui, Auto, XZM; };
		//リフト
		struct Setting1 { float x, y, z, scaleX, scaleY, scaleZ, lifth, liftl, lifts; };
		//レバー
		struct Setting2 { float x, y, z, scaleX, scaleY, scaleZ; };
		//スロープ
		struct Setting3 { float x, y, z, scaleX, scaleY, scaleZ; bool reverse; };
		//床
		struct Setting4 { float x, y, z, scaleX, scaleY, scaleZ; };
		//留め具		被憑依ブロックの前においてね
		struct Setting5 { float x, y, z, scaleX, scaleY, scaleZ; };
		//マグネット	リフトの下においてね
		struct Setting6 { float x, y, z, scaleX, scaleY, scaleZ; };

		// tutorial
	case 0:
	{
		//ブロック
		Setting data[] = {
			{1.0f, 0.0f, 2.5f, 0.25f, 0.25f, 0.25f,true,false},
			{-0.5f, 0.0f, 2.5f, 0.25f, 0.25f, 0.25f,true,false},
		};
		m_num = sizeof(data) / sizeof(data[0]);
		m_pObjects = new Object[m_num];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num; i++) {	//確保したブロックに初期データを設定
			m_pObjects[i].Create(
				data[i].x, data[i].y, data[i].z,
				data[i].scaleX, data[i].scaleY, data[i].scaleZ, data[i].hyoui, data[i].Auto, data[i].XZM
			);
		}
		//リフト
		Setting1 data1[] = {
			{ 0.75f, 10.0f, 2.0f, 0.8f, 0.2f, 0.5f,1.2f,0.5f,0.001f},
		};
		m_num1 = sizeof(data1) / sizeof(data1[0]);
		m_pLift_obj = new Lift_Obj[m_num1];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num1; i++) {		//確保したブロックに初期データを設定
			m_pLift_obj[i].Create(
				data1[i].x, data1[i].y, data1[i].z,
				data1[i].scaleX, data1[i].scaleY, data1[i].scaleZ
				, data1[i].lifth, data1[i].liftl, data1[i].lifts
			);
		}
		//レバー
		Setting2 data2[] = {
			{ 2.0f, 10.0f, 2.5f, 0.15f, 0.15f, 0.15f},
		};
		m_num2 = sizeof(data2) / sizeof(data2[0]);
		m_pLever = new Lever[m_num2];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num2; i++) {	//確保したブロックに初期データを設定
			m_pLever[i].Create(
				data2[i].x, data2[i].y, data2[i].z,
				data2[i].scaleX, data2[i].scaleY, data2[i].scaleZ
			);
		}
		//スロープ
		Setting3 data3[] = {
			{0.5f, 0.0f, 2.5f, 0.25f, 0.25f, 0.25f, false},
			{0.75f, 0.0f, 2.5f, -0.25f, 0.25f, -0.25f, true},	//逆三角形
			{-0.25f, 0.0f, 2.5f, 0.25f, 0.25f, 0.25f, false},
			{-0.75f, 0.0f, 2.5f, -0.25f, 0.25f, -0.25f, true},
		};
		m_num3 = sizeof(data3) / sizeof(data3[0]);
		m_pStair = new Stair[m_num3];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num3; i++) {	//確保したブロックに初期データを設定
			m_pStair[i].Create(
				data3[i].x, data3[i].y, data3[i].z,
				data3[i].scaleX, data3[i].scaleY, data3[i].scaleZ,
				data3[i].reverse
			);
		}
		//床
		Setting4 data4[] = {
			{ 0.0f, 0.0f, 2.3f, 1.45f, 1.2f, 0.7f},
		};
		m_num4 = sizeof(data4) / sizeof(data4[0]);
		m_pYuka = new Yuka[m_num4];			//必要な数だけブロックを確保
		for (int i = 0; i < m_num4; i++) {	//確保したブロックに初期データを設定
			m_pYuka[i].Create(
				data4[i].x, data4[i].y, data4[i].z,
				data4[i].scaleX, data4[i].scaleY, data4[i].scaleZ
			);
		}
		//留め具		被憑依ブロックの前においてね
		Setting5 data5[] = {
		{ 0.0f, -1.0f, 2.3f, 0.3f, 0.3f, 0.3f},
		};
		m_num5 = sizeof(data5) / sizeof(data5[0]);
		m_pTomegu = new Tomegu[m_num5];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num5; i++) {	//確保したブロックに初期データを設定
			m_pTomegu[i].Create(
				data5[i].x, data5[i].y, data5[i].z,
				data5[i].scaleX, data5[i].scaleY, data5[i].scaleZ
			);
		}
		//マグネット	リフトの下においてね
		Setting6 data6[] = {
		{ 1.25f, -1.0f, 2.0f, 0.3f, 0.15f, 0.3f},
		};
		m_num6 = sizeof(data6) / sizeof(data6[0]);
		m_pMagnet = new Magnet[m_num6];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num6; i++) {	//確保したブロックに初期データを設定
			m_pMagnet[i].Create(
				data6[i].x, data6[i].y, data6[i].z,
				data6[i].scaleX, data6[i].scaleY, data6[i].scaleZ
			);
		}
	}
	break;

	// stage1
	case 1:
	{
		//ブロック
		Setting data[] = {
			{1.25f, 0.0f, 2.5f, 0.24f, 0.5f, 0.25f,true,false},
			{0.5f, 0.0f, 2.5f, 0.24f, 0.25f, 0.25f,true,false},
			{0.25f, 0.0f, 2.5f, 0.24f, 0.25f, 0.25f,true,false},
			{0.25f, 0.26f, 2.5f, 0.24f, 0.25f, 0.24f,true,false},
			{0.0f, 0.0f, 2.5f, 0.24f, 0.25f, 0.25f,true,false},
			{-0.25f, 0.0f, 2.5f, 0.24f, 0.5f, 0.25f,true,false},
			//{-0.25f, 0.0f, 2.5f, 0.24f, 0.25f, 0.25f,true,false},
			{0.0f, 0.0f, 3.2f, 0.24f, 0.25f, 0.24f,true,false},
			{-1.0f, 0.0f, 2.5f, 0.24f, 0.25f, 0.25f,true,false},
			{-1.5f, 0.0f, 2.5f, 0.24f, 0.25f, 0.25f,true,false},
			{-1.5f, 0.26f, 2.5f, 0.24f, 0.5f, 0.24f,true,false},
		};
		m_num = sizeof(data) / sizeof(data[0]);
		m_pObjects = new Object[m_num];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num; i++) {	//確保したブロックに初期データを設定
			m_pObjects[i].Create(
				data[i].x, data[i].y, data[i].z,
				data[i].scaleX, data[i].scaleY, data[i].scaleZ, data[i].hyoui, data[i].Auto, data[i].XZM
			);
		}
		//リフト
		Setting1 data1[] = {
			{ 0.75f, 10.0f, 2.0f, 0.8f, 0.2f, 0.5f,1.2f,0.5f,0.001f},
		};
		m_num1 = sizeof(data1) / sizeof(data1[0]);
		m_pLift_obj = new Lift_Obj[m_num1];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num1; i++) {		//確保したブロックに初期データを設定
			m_pLift_obj[i].Create(
				data1[i].x, data1[i].y, data1[i].z,
				data1[i].scaleX, data1[i].scaleY, data1[i].scaleZ
				, data1[i].lifth, data1[i].liftl, data1[i].lifts
			);
		}
		//レバー
		Setting2 data2[] = {
			{ 2.0f, 10.0f, 2.5f, 0.15f, 0.15f, 0.15f},
		};
		m_num2 = sizeof(data2) / sizeof(data2[0]);
		m_pLever = new Lever[m_num2];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num2; i++) {	//確保したブロックに初期データを設定
			m_pLever[i].Create(
				data2[i].x, data2[i].y, data2[i].z,
				data2[i].scaleX, data2[i].scaleY, data2[i].scaleZ
			);
		}
		//スロープ
		Setting3 data3[] = {
			{1.5f, 0.0f, 2.5f, 0.24f, 0.25f, 0.25f, false},
			{1.0f, 0.0f, 2.5f, 0.24f, 0.25f, 0.25f, false},
			{0.75f, 0.0f, 2.5f, 0.24f, 0.25f, 0.25f, false},
			{ 0.0f, 0.0f, 2.8f, 0.24f, 0.25f, 0.25f, false },
		};
		m_num3 = sizeof(data3) / sizeof(data3[0]);
		m_pStair = new Stair[m_num3];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num3; i++) {	//確保したブロックに初期データを設定
			m_pStair[i].Create(
				data3[i].x, data3[i].y, data3[i].z,
				data3[i].scaleX, data3[i].scaleY, data3[i].scaleZ,
				data3[i].reverse
			);
		}
		//床
		Setting4 data4[] = {
			{ 0.0f, 0.0f, 2.3f, 1.45f, 1.2f, 0.7f},
		};
		m_num4 = sizeof(data4) / sizeof(data4[0]);
		m_pYuka = new Yuka[m_num4];			//必要な数だけブロックを確保
		for (int i = 0; i < m_num4; i++) {	//確保したブロックに初期データを設定
			m_pYuka[i].Create(
				data4[i].x, data4[i].y, data4[i].z,
				data4[i].scaleX, data4[i].scaleY, data4[i].scaleZ
			);
		}
		//留め具		被憑依ブロックの前においてね
		Setting5 data5[] = {
			{ 0.0f, -1.0f, 2.3f, 0.3f, 0.3f, 0.3f},
		};
		m_num5 = sizeof(data5) / sizeof(data5[0]);
		m_pTomegu = new Tomegu[m_num5];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num5; i++) {	//確保したブロックに初期データを設定
			m_pTomegu[i].Create(
				data5[i].x, data5[i].y, data5[i].z,
				data5[i].scaleX, data5[i].scaleY, data5[i].scaleZ
			);
		}
		//マグネット	リフトの下においてね
		Setting6 data6[] = {
			{ 1.25f, -1.0f, 2.0f, 0.3f, 0.15f, 0.3f},
		};
		m_num6 = sizeof(data6) / sizeof(data6[0]);
		m_pMagnet = new Magnet[m_num6];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num6; i++) {	//確保したブロックに初期データを設定
			m_pMagnet[i].Create(
				data6[i].x, data6[i].y, data6[i].z,
				data6[i].scaleX, data6[i].scaleY, data6[i].scaleZ
			);
		}
	}
	break;

	// stage2
	case 2:
	{
		//ブロック
		Setting data[] = {
			{1.0f, 0.0f, 2.5f, 0.25f, 0.25f, 0.25f,true,false},
			{0.0f, 0.0f, 2.5f, 0.25f, 0.25f, 0.25f,true,false},
			{-0.75f, 0.0f, 2.5f, 0.25f, 0.5f, 0.25f,false,false},
			{-1.0f, 0.0f, 2.5f, 0.25f, 0.5f, 0.25f,true,false},
			{-1.75f, 0.0f, 2.5f, 0.25f, 0.5f, 0.25f,true,false},
			//{-1.5f, 0.0f, 2.5f, 0.25f, 0.75f, 0.25f,false,false},
		};
		m_num = sizeof(data) / sizeof(data[0]);
		m_pObjects = new Object[m_num];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num; i++) {	//確保したブロックに初期データを設定
			m_pObjects[i].Create(
				data[i].x, data[i].y, data[i].z,
				data[i].scaleX, data[i].scaleY, data[i].scaleZ, data[i].hyoui, data[i].Auto, data[i].XZM
			);
		}
		//リフト
		Setting1 data1[] = {
			{ 0.75f, 10.0f, 2.0f, 0.8f, 0.2f, 0.5f,1.2f,0.5f,0.001f},
		};
		m_num1 = sizeof(data1) / sizeof(data1[0]);
		m_pLift_obj = new Lift_Obj[m_num1];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num1; i++) {		//確保したブロックに初期データを設定
			m_pLift_obj[i].Create(
				data1[i].x, data1[i].y, data1[i].z,
				data1[i].scaleX, data1[i].scaleY, data1[i].scaleZ
				, data1[i].lifth, data1[i].liftl, data1[i].lifts
			);
		}
		//レバー
		Setting2 data2[] = {
			{ 2.0f, 10.0f, 2.5f, 0.15f, 0.15f, 0.15f},
		};
		m_num2 = sizeof(data2) / sizeof(data2[0]);
		m_pLever = new Lever[m_num2];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num2; i++) {	//確保したブロックに初期データを設定
			m_pLever[i].Create(
				data2[i].x, data2[i].y, data2[i].z,
				data2[i].scaleX, data2[i].scaleY, data2[i].scaleZ
			);
		}
		//スロープ
		Setting3 data3[] = {
			{1.75f, 0.0f, 2.6f, 0.25f, 0.25f, 0.25f, false},
			{0.0f, 0.0f, 3.2f, 0.25f, 0.25f, 0.25f, false},
			{-0.5f, 0.0f, 2.0f, 0.25f, 0.25f, 0.25f, false},
			//{-1.8f, 0.0f, 2.5f, 0.25f, 0.25f, 0.25f, false},
			{-0.7f, 0.0f, 3.2f, -0.25f, 0.25f, -0.25f, true},
		};
		m_num3 = sizeof(data3) / sizeof(data3[0]);
		m_pStair = new Stair[m_num3];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num3; i++) {	//確保したブロックに初期データを設定
			m_pStair[i].Create(
				data3[i].x, data3[i].y, data3[i].z,
				data3[i].scaleX, data3[i].scaleY, data3[i].scaleZ,
				data3[i].reverse
			);
		}
		//床
		Setting4 data4[] = {
			{ 0.0f, 0.0f, 2.3f, 1.45f, 1.2f, 0.7f},
		};
		m_num4 = sizeof(data4) / sizeof(data4[0]);
		m_pYuka = new Yuka[m_num4];			//必要な数だけブロックを確保
		for (int i = 0; i < m_num4; i++) {	//確保したブロックに初期データを設定
			m_pYuka[i].Create(
				data4[i].x, data4[i].y, data4[i].z,
				data4[i].scaleX, data4[i].scaleY, data4[i].scaleZ
			);
		}
		//留め具		被憑依ブロックの前においてね
		Setting5 data5[] = {
			{-0.75f, 0.0f, 2.65f, 0.3f, 0.3f, 0.3f},
			//{-1.5f, 0.0f, 2.65f, 0.3f, 0.3f, 0.3f},
		};
		m_num5 = sizeof(data5) / sizeof(data5[0]);
		m_pTomegu = new Tomegu[m_num5];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num5; i++) {	//確保したブロックに初期データを設定
			m_pTomegu[i].Create(
				data5[i].x, data5[i].y, data5[i].z,
				data5[i].scaleX, data5[i].scaleY, data5[i].scaleZ
			);
		}
		//マグネット	リフトの下においてね
		Setting6 data6[] = {
		{ 1.25f, -1.0f, 2.0f, 0.3f, 0.15f, 0.3f},
		};
		m_num6 = sizeof(data6) / sizeof(data6[0]);
		m_pMagnet = new Magnet[m_num6];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num6; i++) {	//確保したブロックに初期データを設定
			m_pMagnet[i].Create(
				data6[i].x, data6[i].y, data6[i].z,
				data6[i].scaleX, data6[i].scaleY, data6[i].scaleZ
			);
		}
	}
	break;

	// stage3
	case 3:
	{
		//ブロック
		Setting data[] = {
			{0.75f, 0.0f, 2.5f, 0.25f, 0.25f, 0.25f,true,false},
			{-0.5f, 0.0f, 2.75f, 0.25f, 0.25f, 0.25f,true,false},
			{-1.0f, 0.0f, 2.5f, 0.25f, 0.5f, 0.25f,true,false},
		};
		m_num = sizeof(data) / sizeof(data[0]);
		m_pObjects = new Object[m_num];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num; i++) {	//確保したブロックに初期データを設定
			m_pObjects[i].Create(
				data[i].x, data[i].y, data[i].z,
				data[i].scaleX, data[i].scaleY, data[i].scaleZ, data[i].hyoui, data[i].Auto, data[i].XZM
			);
		}
		//リフト
		Setting1 data1[] = {
			{ 0.325f, 0.35f, 2.0f, 0.75f, 0.1f, 0.25f,1.2f,0.35f,0.01f},
			{ -0.75f, 0.75f, 2.0f, 0.75f, 0.1f, 0.25f,1.2f,0.5f,0.01f},
			{ -2.25f, 0.75f, 2.25f, 0.5f, 0.1f, 0.25f,1.2f,0.5f,0.01f},
		};
		m_num1 = sizeof(data1) / sizeof(data1[0]);
		m_pLift_obj = new Lift_Obj[m_num1];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num1; i++) {		//確保したブロックに初期データを設定
			m_pLift_obj[i].Create(
				data1[i].x, data1[i].y, data1[i].z,
				data1[i].scaleX, data1[i].scaleY, data1[i].scaleZ
				, data1[i].lifth, data1[i].liftl, data1[i].lifts
			);
		}
		//レバー
		Setting2 data2[] = {
			{ 2.0f, 0.5f, 2.0f, 0.15f, 0.15f, 0.15f},
		};
		m_num2 = sizeof(data2) / sizeof(data2[0]);
		m_pLever = new Lever[m_num2];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num2; i++) {	//確保したブロックに初期データを設定
			m_pLever[i].Create(
				data2[i].x, data2[i].y, data2[i].z,
				data2[i].scaleX, data2[i].scaleY, data2[i].scaleZ
			);
		}
		//スロープ
		Setting3 data3[] = {
			{1.2f, 0.0f, 2.5f, 0.25f, 0.25f, 0.25f, false},
			{0.4f, 0.0f, 2.75f, 0.25f, 0.25f, 0.25f, false},
			//{0.0f, 0.0f, 2.75f, 0.25f, 0.25f, 0.25f, false},
			{-0.7f, 0.0f, 2.5f, 0.25f, 0.25f, 0.25f, false},
		};
		m_num3 = sizeof(data3) / sizeof(data3[0]);
		m_pStair = new Stair[m_num3];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num3; i++) {	//確保したブロックに初期データを設定
			m_pStair[i].Create(
				data3[i].x, data3[i].y, data3[i].z,
				data3[i].scaleX, data3[i].scaleY, data3[i].scaleZ,
				data3[i].reverse
			);
		}
		//床
		Setting4 data4[] = {
			{ 0.0f, 0.0f, 2.3f, 1.45f, 1.2f, 0.7f},
		};
		m_num4 = sizeof(data4) / sizeof(data4[0]);
		m_pYuka = new Yuka[m_num4];			//必要な数だけブロックを確保
		for (int i = 0; i < m_num4; i++) {	//確保したブロックに初期データを設定
			m_pYuka[i].Create(
				data4[i].x, data4[i].y, data4[i].z,
				data4[i].scaleX, data4[i].scaleY, data4[i].scaleZ
			);
		}
		//留め具		被憑依ブロックの前においてね
		Setting5 data5[] = {
			{ 0.0f, -1.0f, 2.3f, 0.3f, 0.3f, 0.3f},
		};
		m_num5 = sizeof(data5) / sizeof(data5[0]);
		m_pTomegu = new Tomegu[m_num5];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num5; i++) {	//確保したブロックに初期データを設定
			m_pTomegu[i].Create(
				data5[i].x, data5[i].y, data5[i].z,
				data5[i].scaleX, data5[i].scaleY, data5[i].scaleZ
			);
		}
		//マグネット	リフトの下においてね
		Setting6 data6[] = {
			{ 0.325f, 0.0f, 2.0f, 0.3f, 0.15f, 0.3f},
			{ -0.75f, 0.0f, 2.0f, 0.3f, 0.15f, 0.3f},
			{ -2.25f, 0.0f, 2.25f, 0.3f, 0.15f, 0.3f},
		};
		m_num6 = sizeof(data6) / sizeof(data6[0]);
		m_pMagnet = new Magnet[m_num6];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num6; i++) {	//確保したブロックに初期データを設定
			m_pMagnet[i].Create(
				data6[i].x, data6[i].y, data6[i].z,
				data6[i].scaleX, data6[i].scaleY, data6[i].scaleZ
			);
		}
	}
	break;

	// stage4
	case 4:
	{
		//ブロック
		Setting data[] = {
			{1.7f, 0.0f, 2.0f, 0.25f, 0.25f, 0.25f,true,false},
			{0.7f, 0.0f, 2.0f, 0.25f, 0.25f, 0.25f,true,false},
			//{1.0f, 1.25f, 2.0f, 0.24f, 0.25f, 0.25f,false,false},
			{0.0f, 0.0f, 2.0f, 0.25f, 0.5f, 0.25f,true,false},
			{-0.625f, 0.0f, 2.0f, 1.0f, 0.25f, 0.25f,false,false},
			{-1.25f, 0.0f, 2.5f, 0.25f, 0.25f, 0.25f,true,false},
			{-1.75f, 0.0f, 2.0f, 0.25f, 0.25f, 0.25f,true,false},
		};
		m_num = sizeof(data) / sizeof(data[0]);
		m_pObjects = new Object[m_num];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num; i++) {	//確保したブロックに初期データを設定
			m_pObjects[i].Create(
				data[i].x, data[i].y, data[i].z,
				data[i].scaleX, data[i].scaleY, data[i].scaleZ, data[i].hyoui, data[i].Auto, data[i].XZM
			);
		}
		//リフト
		Setting1 data1[] = {
			{ 1.25f, 1.0f, 2.0f, 1.0f, 0.1f, 0.25f,1.2f,0.5f,0.01f},
			{ -1.25f, 1.0f, 2.0f, 1.0f, 0.1f, 0.25f,1.2f,0.5f,0.01f},
		};
		m_num1 = sizeof(data1) / sizeof(data1[0]);
		m_pLift_obj = new Lift_Obj[m_num1];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num1; i++) {		//確保したブロックに初期データを設定
			m_pLift_obj[i].Create(
				data1[i].x, data1[i].y, data1[i].z,
				data1[i].scaleX, data1[i].scaleY, data1[i].scaleZ
				, data1[i].lifth, data1[i].liftl, data1[i].lifts
			);
		}
		//レバー
		Setting2 data2[] = {
			{ 2.0f, 0.5f, 2.0f, 0.15f, 0.15f, 0.15f},
		};
		m_num2 = sizeof(data2) / sizeof(data2[0]);
		m_pLever = new Lever[m_num2];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num2; i++) {	//確保したブロックに初期データを設定
			m_pLever[i].Create(
				data2[i].x, data2[i].y, data2[i].z,
				data2[i].scaleX, data2[i].scaleY, data2[i].scaleZ
			);
		}
		//スロープ
		Setting3 data3[] = {
			{1.4f, 0.0f, 2.3f, -0.25f, 0.25f, -0.25f, true},//逆向き
			{1.0f, 0.0f, 2.3f, 0.25f, 0.25f, 0.25f, false},
			{0.33f, 0.0f, 2.0f, -0.25f, 0.25f, -0.25f, true},//逆向き
			{-1.25f, 0.0f, 2.75f, -0.25f, 0.25f,-0.25f, true},//逆向き
			{0.5f, 0.0f, 2.3f, 0.25f, 0.25f, 0.25f, false},
		};
		m_num3 = sizeof(data3) / sizeof(data3[0]);
		m_pStair = new Stair[m_num3];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num3; i++) {	//確保したブロックに初期データを設定
			m_pStair[i].Create(
				data3[i].x, data3[i].y, data3[i].z,
				data3[i].scaleX, data3[i].scaleY, data3[i].scaleZ,
				data3[i].reverse
			);
		}
		//床
		Setting4 data4[] = {
			{ 0.0f, 0.0f, 2.3f, 1.45f, 1.2f, 0.7f},
		};
		m_num4 = sizeof(data4) / sizeof(data4[0]);
		m_pYuka = new Yuka[m_num4];			//必要な数だけブロックを確保
		for (int i = 0; i < m_num4; i++) {	//確保したブロックに初期データを設定
			m_pYuka[i].Create(
				data4[i].x, data4[i].y, data4[i].z,
				data4[i].scaleX, data4[i].scaleY, data4[i].scaleZ
			);
		}
		//留め具		被憑依ブロックの前においてね
		Setting5 data5[] = {
			{ -0.3f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f},
			{ -0.9f, 0.0f, 2.2f, 0.3f, 0.3f, 0.3f},
		};
		m_num5 = sizeof(data5) / sizeof(data5[0]);
		m_pTomegu = new Tomegu[m_num5];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num5; i++) {	//確保したブロックに初期データを設定
			m_pTomegu[i].Create(
				data5[i].x, data5[i].y, data5[i].z,
				data5[i].scaleX, data5[i].scaleY, data5[i].scaleZ
			);
		}
		//マグネット	リフトの下においてね
		Setting6 data6[] = {
			{ 1.25f, 0.0f, 2.0f, 0.3f, 0.15f, 0.3f},
			{ -1.25f, 0.0f, 2.0f, 0.3f, 0.15f, 0.3f},
		};
		m_num6 = sizeof(data6) / sizeof(data6[0]);
		m_pMagnet = new Magnet[m_num6];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num6; i++) {	//確保したブロックに初期データを設定
			m_pMagnet[i].Create(
				data6[i].x, data6[i].y, data6[i].z,
				data6[i].scaleX, data6[i].scaleY, data6[i].scaleZ
			);
		}
	}
	break;

	// stage5
	case 5:
	{
		//ブロック
		Setting data[] = {
			{0.5f, 0.0f, 2.0f, 0.49f, 0.49f, 0.25f,false,true},
			{-0.25f, 0.0f, 2.0f, 0.24f, 0.25f, 0.25f,false,true},
			{-1.0f, 0.0f, 2.0f, 0.24f, 0.5f, 0.25f,true,false},
			{-1.25f, 0.0f, 2.0f, 0.24f, 0.75f, 0.25f,true,false},
			{-1.5f, 0.0f, 2.0f, 0.24f, 0.75f, 0.25f,true,false},
		};
		m_num = sizeof(data) / sizeof(data[0]);
		m_pObjects = new Object[m_num];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num; i++) {	//確保したブロックに初期データを設定
			m_pObjects[i].Create(
				data[i].x, data[i].y, data[i].z,
				data[i].scaleX, data[i].scaleY, data[i].scaleZ, data[i].hyoui, data[i].Auto, data[i].XZM
			);
		}
		//リフト
		Setting1 data1[] = {
			{ 0.345f, 0.6f, 2.0f, 0.5f, 0.1f, 0.25f,1.2f,0.5f,0.01f},    //動かさない
			{ -0.875f, 1.25f, 2.0f, 0.5f, 0.1f, 0.25f,1.2f,0.5f,0.01f},    //動かさない
		};
		m_num1 = sizeof(data1) / sizeof(data1[0]);
		m_pLift_obj = new Lift_Obj[m_num1];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num1; i++) {		//確保したブロックに初期データを設定
			m_pLift_obj[i].Create(
				data1[i].x, data1[i].y, data1[i].z,
				data1[i].scaleX, data1[i].scaleY, data1[i].scaleZ
				, data1[i].lifth, data1[i].liftl, data1[i].lifts
			);
		}
		//レバー
		Setting2 data2[] = {
			{ 2.0f, 0.5f, 2.0f, 0.15f, 0.15f, 0.15f},
			//{ -2.0f, 0.5f, 2.0f, 0.15f, 0.15f, 0.15f},
		};
		m_num2 = sizeof(data2) / sizeof(data2[0]);
		m_pLever = new Lever[m_num2];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num2; i++) {	//確保したブロックに初期データを設定
			m_pLever[i].Create(
				data2[i].x, data2[i].y, data2[i].z,
				data2[i].scaleX, data2[i].scaleY, data2[i].scaleZ
			);
		}
		//スロープ
		Setting3 data3[] = {
			{1.6f, 0.0f, 2.3f, -0.25f, 0.25f, -0.25f, true},//逆向き
			{1.0f, 0.0f, 2.3f, 0.25f, 0.25f, 0.25f, false},
			//{0.5f, 0.0f, 2.8f, -0.25f, 0.25f, -0.25f, true},//逆向き
			{-1.0f, 0.0f, 2.75f, -0.25f, 0.25f,-0.25f, true},//逆向き
			{0.75f, 0.0f, 2.5f, 0.25f, 0.25f, 0.25f, false},
		};
		m_num3 = sizeof(data3) / sizeof(data3[0]);
		m_pStair = new Stair[m_num3];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num3; i++) {	//確保したブロックに初期データを設定
			m_pStair[i].Create(
				data3[i].x, data3[i].y, data3[i].z,
				data3[i].scaleX, data3[i].scaleY, data3[i].scaleZ,
				data3[i].reverse
			);
		}
		//床
		Setting4 data4[] = {
			{ 0.0f, 0.0f, 2.3f, 1.45f, 1.2f, 0.7f},
		};
		m_num4 = sizeof(data4) / sizeof(data4[0]);
		m_pYuka = new Yuka[m_num4];			//必要な数だけブロックを確保
		for (int i = 0; i < m_num4; i++) {	//確保したブロックに初期データを設定
			m_pYuka[i].Create(
				data4[i].x, data4[i].y, data4[i].z,
				data4[i].scaleX, data4[i].scaleY, data4[i].scaleZ
			);
		}
		//留め具		被憑依ブロックの前においてね
		Setting5 data5[] = {
			{ 0.0f, -1.0f, 2.3f, 0.3f, 0.3f, 0.3f},
		};
		m_num5 = sizeof(data5) / sizeof(data5[0]);
		m_pTomegu = new Tomegu[m_num5];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num5; i++) {	//確保したブロックに初期データを設定
			m_pTomegu[i].Create(
				data5[i].x, data5[i].y, data5[i].z,
				data5[i].scaleX, data5[i].scaleY, data5[i].scaleZ
			);
		}
		//マグネット	リフトの下においてね
		Setting6 data6[] = {
			{ 1.25f, 0.0f, 2.0f, 0.3f, 0.15f, 0.3f},
			{ -1.25f, 0.0f, 2.0f, 0.3f, 0.15f, 0.3f},
		};
		m_num6 = sizeof(data6) / sizeof(data6[0]);
		m_pMagnet = new Magnet[m_num6];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num6; i++) {	//確保したブロックに初期データを設定
			m_pMagnet[i].Create(
				data6[i].x, data6[i].y, data6[i].z,
				data6[i].scaleX, data6[i].scaleY, data6[i].scaleZ
			);
		}
	}
	break;

	// stage6 無し
	case 6:
	{
		//ブロック
		Setting data[] = {
			{1.0f, 0.0f, 2.0f, 0.24f, 0.25f, 0.25f,true,false},
			{0.75f, 0.0f, 2.0f, 0.24f, 0.5f, 0.25f,true,false},
			{0.5f, 0.0f, 2.0f, 0.24f, 0.5f, 0.25f,true,false},
			{-0.25f, 0.0f, 2.0f, 0.24f, 1.25f, 0.25f,false,false},    //動かさない
			{-0.5f, 0.0f, 2.0f, 0.24f, 0.5f, 0.25f,true,false},
			{-0.5f, 0.0f, 2.25f, 0.24f, 0.25f, 0.24f,true,false},
			{-0.75f, 0.0f, 2.0f, 0.24f, 0.25f, 0.25f,false,false},    //動かさない
			{-1.625f, 0.0f, 2.0f, 0.5f, 0.25f, 0.25f,true,false},
		};
		m_num = sizeof(data) / sizeof(data[0]);
		m_pObjects = new Object[m_num];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num; i++) {	//確保したブロックに初期データを設定
			m_pObjects[i].Create(
				data[i].x, data[i].y, data[i].z,
				data[i].scaleX, data[i].scaleY, data[i].scaleZ, data[i].hyoui, data[i].Auto, data[i].XZM
			);
		}
		//リフト
		Setting1 data1[] = {
			{ 1.25f, 0.65f, 2.0f, 0.5f, 0.1f, 0.25f,2.0f,0.5f,0.01f},
			{ 0.125f, 0.5f, 2.0f, 0.35f, 0.1f, 0.25f,2.0f,0.5f,0.01f},
			{ -1.125f, 0.5f, 2.0f, 0.5f, 0.1f, 0.25f,2.0f,0.5f,0.01f}
		};
		m_num1 = sizeof(data1) / sizeof(data1[0]);
		m_pLift_obj = new Lift_Obj[m_num1];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num1; i++) {		//確保したブロックに初期データを設定
			m_pLift_obj[i].Create(
				data1[i].x, data1[i].y, data1[i].z,
				data1[i].scaleX, data1[i].scaleY, data1[i].scaleZ
				, data1[i].lifth, data1[i].liftl, data1[i].lifts
			);
		}
		//レバー
		Setting2 data2[] = {
			{ 2.0f, 0.5f, 2.5f, 0.15f, 0.15f, 0.15f},
			{ -2.0f, 0.5f, 2.5f, 0.15f, 0.15f, 0.15f},
		};
		m_num2 = sizeof(data2) / sizeof(data2[0]);
		m_pLever = new Lever[m_num2];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num2; i++) {	//確保したブロックに初期データを設定
			m_pLever[i].Create(
				data2[i].x, data2[i].y, data2[i].z,
				data2[i].scaleX, data2[i].scaleY, data2[i].scaleZ
			);
		}
		//スロープ
		Setting3 data3[] = {
			{1.25f, 0.0f, 2.0f, 0.24f, 0.25f, 0.25f,false},
			{0.5f, 0.0f, 2.25f, 0.24f, 0.24f, 0.25f,false},
			{0.25f, 0.0f, 2.0f, -0.24f, 0.24f, -0.25f,true},
			{-0.5f, 0.25f, 2.25f, -0.24f, 0.24f, -0.25f,true},
			{-1.0f, 0.0f, 2.0f, -0.24f, 0.24f, -0.25f,true},
		};
		m_num3 = sizeof(data3) / sizeof(data3[0]);
		m_pStair = new Stair[m_num3];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num3; i++) {	//確保したブロックに初期データを設定
			m_pStair[i].Create(
				data3[i].x, data3[i].y, data3[i].z,
				data3[i].scaleX, data3[i].scaleY, data3[i].scaleZ,
				data3[i].reverse
			);
		}
		//床
		Setting4 data4[] = {
			{ 0.0f, 0.0f, 2.3f, 1.45f, 1.2f, 0.7f},
		};
		m_num4 = sizeof(data4) / sizeof(data4[0]);
		m_pYuka = new Yuka[m_num4];			//必要な数だけブロックを確保
		for (int i = 0; i < m_num4; i++) {	//確保したブロックに初期データを設定
			m_pYuka[i].Create(
				data4[i].x, data4[i].y, data4[i].z,
				data4[i].scaleX, data4[i].scaleY, data4[i].scaleZ
			);
		}
		//留め具		被憑依ブロックの前においてね
		Setting5 data5[] = {
			{-0.25f, 0.0f, 2.2f,0.3f,0.3f,0.3f},
		};
		m_num5 = sizeof(data5) / sizeof(data5[0]);
		m_pTomegu = new Tomegu[m_num5];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num5; i++) {	//確保したブロックに初期データを設定
			m_pTomegu[i].Create(
				data5[i].x, data5[i].y, data5[i].z,
				data5[i].scaleX, data5[i].scaleY, data5[i].scaleZ
			);
		}
		//マグネット	リフトの下においてね
		Setting6 data6[] = {
			{ 1.125f, 0.0f, 2.0f, 0.3f, 0.15f, 0.3f},
			{ 0.125f, 0.0f, 2.0f, 0.3f, 0.15f, 0.3f},
			{ -1.125f, 0.0f, 2.0f, 0.3f, 0.15f, 0.3f}
		};
		m_num6 = sizeof(data6) / sizeof(data6[0]);
		m_pMagnet = new Magnet[m_num6];		//必要な数だけブロックを確保
		for (int i = 0; i < m_num6; i++) {	//確保したブロックに初期データを設定
			m_pMagnet[i].Create(
				data6[i].x, data6[i].y, data6[i].z,
				data6[i].scaleX, data6[i].scaleY, data6[i].scaleZ
			);
		}
	}
	break;
	}
	// effect
	m_Effect = LibEffekseer::Create("Assets/effect/BoxSet.efkefc");

}

ObjectMng::~ObjectMng()
{
	delete[] m_pObjects;
	//delete[] m_pObjectsAuto;
	delete[] m_pLift_obj;
	delete[] m_pLever;
	delete[] m_pYuka;
	delete[] m_pStair;
	delete[] m_pTomegu;
	delete[] m_pMagnet;

	if (m_pObjectCamera)
	{
		delete m_pObjectCamera;
		m_pObjectCamera = nullptr;
	}
	if (aabb)
	{
		delete aabb;
		aabb = nullptr;
	}
	if (haabb)
	{
		delete haabb;
		haabb = nullptr;
	}
	if (caabb)
	{
		delete caabb;
		caabb = nullptr;
	}
	if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
}

void ObjectMng::Update(float tick)
{
	imanagerO.addKeycode(0, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_A);
	imanagerO.addKeycode(1, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);

	imanagerO.addKeycode(2, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_Y);
	imanagerO.addKeycode(3, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_X);

	imanagerO.inspect();

	float AB = static_cast<float>(imanagerO.getKey(0));
	float BB = static_cast<float>(imanagerO.getKey(1));
	float YB = static_cast<float>(imanagerO.getKey(2));
	float XB = static_cast<float>(imanagerO.getKey(3));

#if TEST
	m_pPlayer->Update(tick);

	//for (int t = 0; t < m_num6; t++)
	//{// 動くブロック
	//	m_pObjectsAuto->Update();
	//}

	for (int t = 0;t < m_num5; t++)
	{// 留め具
		m_pTomegu[t].Update();
	}

	for (int y = 0; y < m_num4; y++)
	{// 床
		m_pYuka[y].Update();
	}

	for (int b = 0; b < m_num3; b++)
	{// ゴール
		m_pStair[b].Update();
	}
	for (int i = 0; i < m_num; i++)
	{
		m_pObjects[i].Update();
	}





	for (int i = 0; i < m_num; i++)
	{

		for (int b = 0; b < m_num3; b++)
		{
			for (int a = 0; a < m_num1; a++)
			{
				//m_pLift_obj[a].Update();
				//if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
				//{
				//	if (Lift_Obj* lift = dynamic_cast<Lift_Obj*>(&m_pLift_obj[a]))
				//	{

				//		//ブロックの衝突
				//		if (m_pObjects[i].col(*lift))
				//		{
				//			m_pObjects[i].OBJPos();
				//		}
				//	}
				//}
				//for (int l = 0; l < m_num2; l++)
				//{

				//	m_pLever[l].Update();//レバーとプレイヤー
				//	if (Lever* lever = dynamic_cast<Lever*>(&m_pLever[l]))
				//	{
				//		if (m_pPlayer->IsCollidingWith(*lever))
				//		{
				//			if (IsKeyPress('3'))
				//			{
				//				m_pLift_obj[a].SetFLever();
				//			}
				//			if (IsKeyPress('4'))
				//			{
				//				m_pLift_obj[a].SetLever();
				//			}
				//		}
				//	}
				//}

				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
				{
					if (Lift_Obj* lift = dynamic_cast<Lift_Obj*>(&m_pLift_obj[a]))
					{
						float liftposY = lift[a].GetMaxBounds().y;
						m_pLift_obj[a].Update();
						//ブロックの衝突
						if (m_pObjects[i].col(*lift))
						{
							//m_pObjects[i].OBJPosy();
							if (gameObject->GetCMinBounds().y + 0.05 >= lift->GetMaxBounds().y)
							{
								m_pObjects[i].MoveObject(lift->GetMaxBounds().y);
								m_pObjects[i].SetF1();
								m_pObjects[i].framepls();
							}
							else if (gameObject->GetCMinBounds().y + 0.03 <= lift->GetMaxBounds().y)
							{
								m_pObjects[i].MoveObject(lift->GetMaxBounds().y);
								m_pObjects[i].SetF1();
								m_pObjects[i].framepls();

							}
							else if (m_pObjects[i].IsXZ())
							{
								m_pObjects[i].OBJPosy();
							}
							if (m_pLift_obj[a].IsMove())
							{
								if (gameObject->GetCMinBounds().y > lift->GetMaxBounds().y)
								{
									m_pObjects[i].MoveObject(liftposY);
								}
							}
							if (gameObject->GetMaxBounds().y <= lift->GetCMinBounds().y)
							{
								if(m_pLift_obj[a].IsMove()==true)
								{
									if (m_pObjects[i].IsMove() == false)
									{
										m_pLift_obj[a].MoveLift(gameObject->GetMaxBounds().y + 0.1f);
									}
									//m_pObjects[i].OBJPosy();

								}
							}
						}
					}
				}
				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pStair[b]))
				{
					if (Lift_Obj* lift = dynamic_cast<Lift_Obj*>(&m_pLift_obj[a]))
					{
						float liftposY = lift[a].GetMaxBounds().y;
						m_pLift_obj[a].Update();
						//ブロックの衝突
						if (m_pStair[b].col(*lift))
						{
							//m_pObjects[i].OBJPosy();
							if (gameObject->GetCMinBounds().y + 0.5 >= lift->GetMaxBounds().y)
							{
								m_pStair[b].SetSlopeY(lift->GetMaxBounds().y);
								m_pStair[b].SetF1();
								m_pStair[b].framepls();
							}
							else if (gameObject->GetCMinBounds().y + 0.03 <= lift->GetMaxBounds().y)
							{
								m_pStair[b].SetSlopeY(lift->GetMaxBounds().y);
								m_pStair[b].SetF1();
								m_pStair[b].framepls();
							}
							else if (m_pObjects[i].IsXZ())
							{
								m_pStair[b].SetSlopeY(lift->GetMaxBounds().y);
							}
							if (m_pLift_obj[a].IsMove())
							{
								m_pStair[b].MoveStair(liftposY);
							}
							if (gameObject->GetMaxBounds().y <= lift->GetCMinBounds().y)
							{
								if (m_pLift_obj[a].IsMove() == true)
								{
									if (m_pStair[b].IsMove() == false)
									{
										m_pLift_obj[a].MoveLift(gameObject->GetMaxBounds().y + 0.1f);
									}
									//m_pObjects[i].OBJPosy();

								}
							}
						}
					}
				}
				for (int l = 0; l < m_num2; l++)
				{
					for (int mg = 0; mg < m_num6; mg++)
					{// ゴール
						m_pMagnet[mg].Update();


						m_pLever[l].Update();//レバーとプレイヤー
						if (Lever* lever = dynamic_cast<Lever*>(&m_pLever[l]))
						{
							if (m_pPlayer->IsCollidingWith(*lever))
							{
								if (IsKeyPress('Q'))
								{
									m_pLever[l].SetMoveTrue();
									m_pMagnet[mg].Set();
									m_pLift_obj[a].SetMoveTrue();
									m_pPlayer->SetOk();
									m_pPlayer->HPlayerPos();
								}
							}
							if (IsKeyPress('E'))
							{
								m_pLever[l].SetMoveFalse();
								m_pMagnet[mg].SetF();
								m_pLift_obj[a].SetMoveFalse();
								m_pPlayer->SetNOk();
								m_pPlayer->PlayerPos();
							}
						}
					}
				}

				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
				{
					//////ブロックとプレイヤー衝突
					//if (m_pPlayer->IsCollidingWith(*gameObject)) {
					//	// 衝突時の処理
					//	m_pPlayer->PlayerPos();
					//}
				}
				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
				{
					//憑依のため・ブロックとプレイヤーが当たった場合
					if ((m_pPlayer->HIsCollidingWith(*gameObject))&&m_pObjects[i].possessionok()==true)
					{

						if (IsKeyPress('Q'))//(imanagerO.getKey(0) & 0b011)
						{
							//// effectこうしん
							// effectこうしん
							float X = m_pPlayer->GetPosX() / EFFECT_PLAYER_MAX_POS_X * EFFECT_MAX_POS_X;

							float Y = m_pPlayer->GetPosY() -0.9f;

							float Z = (m_pPlayer->GetPosZ() - EFFECT_PLAYER_MIN_POS_Z) /
								(EFFECT_PLAYER_MAX_POS_Z - EFFECT_PLAYER_MIN_POS_Z) *
								EFFECT_MAX_POS_Z + EFFECT_MIN_POS_Z;
							m_EffectHandle = LibEffekseer::GetManager()->Play(m_Effect, X, Y, Z);

							//移動させる時
							Effekseer::Matrix43 EffecMat = LibEffekseer::GetManager()->GetBaseMatrix(m_EffectHandle);
							EffecMat.Translation(0.0f, -1.0f, 0.0f);
							LibEffekseer::GetManager()->SetBaseMatrix(m_EffectHandle, EffecMat);
							LibEffekseer::GetManager()->SetScale(m_EffectHandle, 0.2f, 0.2f, 0.2f);

							m_pPlayer->SetOk();
							m_pPlayer->HPlayerPos();
							m_pObjects[i].Set();
							m_pObjects[i].Set1();
							m_pObjects[i].SetColgravityfalse();
							//m_pObjects[i].Modelchg();
						}
					}
					//憑依解除
					/*if (!m_pPlayer->HIsCollidingWith(*gameObject))
					{*/
					if (IsKeyPress('E'))//(imanagerO.getKey(1) & 0b011)
					{
						if (m_pObjects[i].SetR() == true)
						{
							m_pPlayer->SetNOk();
							m_pPlayer->PlayerPos();
							for (int j = 0; j < m_num; j++)
							{
								m_pObjects[j].SetF();
								/*if (m_pObjects[i].SetR() == true)
								{
									m_pObjects[i].SetF();
								}*/
								m_pObjects[j].SetColgravity();
								//m_pObjects[i].Setgravityfalse();
							}
							//gravityをfalseにする

							//m_pObjects[i].SetF1();

							//m_pObjects[i].Update(); //重力を消してからUpdateを経ずにfalseに入っている
							//m_pObjects[i].Modelchg2();
						}
					}
				}



				if (m_pObjects[i].SetR() == true /*|| m_pObjects[i].SetR() == false*/)  //四角形同士
				{
					for (int j = 0; j < m_num; j++)
					{
						if (j == i)
						{
							j++;
						}
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
						{
							if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[j]))
							{

								// ブロックiとブロックjの当たり判定
								if (m_pObjects[i].col(*gameObject2) /*&& m_pObjects[j].col(*gameObject)*/)
								{
									//m_pObjects[j].OBJPos();
									//MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
									if (m_pObjects[i].IsGravity())  //ジャンプ中の重力がある場合
									{
										if (gameObject->GetCMinBounds().y + 0.05f >= gameObject2->GetMaxBounds().y)  //上辺の当たり判定
										{
											//m_pObjects[i].OBJPosy();  //y以外過去座標へ
												m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y + 0.01f);

											if (m_pObjects[j].IsMove() == true)
											{
												m_pObjects[i].Set1();

											}
											else
											{
												m_pObjects[i].SetF1();
												m_pObjects[i].framepls();
											}

										}
										//else if (gameObject->GetMaxBounds().y >= gameObject2->GetCMinBounds().y)  //すり抜け防止
										//{
										//	/*m_pObjects[j].SetObjectTop();
										//	m_pObjects[j].Set();*/
										//	m_pObjects[j].OBJPosy();
										//	//m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y + 0.02);
										//}
										else if (gameObject->GetCMinBounds().y + 0.07 >= gameObject2->GetMaxBounds().y)  //すり抜け防止
										{
											/*m_pObjects[j].SetObjectTop();
											m_pObjects[j].Set();*/
											m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y + 0.01f);
											//m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y + 0.02);
										}
										else if (m_pObjects[i].IsXZ())  //x,z軸から当たった場合
										{
											m_pObjects[i].OBJPos();  //すべて過去座標へ
										}
									}
									else if (gameObject2->GetCMinBounds().y + 0.1f >= gameObject->GetMaxBounds().y/*gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.1*/)
									{
										for (int h = 0; h < m_num; h++)  //ジャンプ中の重力がない時の連動
										{
											m_pObjects[h].Set1();
										}
										m_pObjects[j].Set();
										m_pObjects[j].OBJPosy();
									}
									else if (gameObject2->GetCMinBounds().y >= gameObject->GetMaxBounds().y/*gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.1*/)
									{
										for (int h = 0; h < m_num; h++)  //すり抜け防止
										{
											m_pObjects[h].Set1();
										}
										//m_pObjects[j].Set();
										m_pObjects[j].OBJPosy();
									}
									//else if (gameObject->GetMaxBounds().y >= gameObject2->GetCMinBounds().y)
									//{
									//	/*m_pObjects[j].SetObjectTop();
									//	m_pObjects[j].Set();*/
									//	//m_pObjects[i].OBJPos();
									//}
									else if (m_pObjects[i].IsXZ())  //x,z軸から当たった場合
									{
										/*m_pObjects[i].OBJPos();
										m_pObjects[j].OBJPos();*/
										for (int h = 0; h < m_num; h++)
										{
											m_pObjects[h].OBJPos();
										}
										//m_pObjects[i].SetF();
									}
									else if (m_pObjects[j].IsXZ() && m_pObjects[j].IsAutoMove() == true)  //x,z軸から当たった場合 j automove
									{
										/*m_pObjects[i].OBJPos();
										m_pObjects[j].OBJPos();*/
										/*for (int h = 0; h < m_num; h++)
										{
											m_pObjects[h].OBJPos();
										}*/
										//m_pObjects[i].SetF();
										m_pObjects[i].OBJPos();
										m_pObjects[j].OBJPos();
										if (m_pObjects[j].MVX() == true)
										{
											m_pObjects[j].MoveXfalse();
										}
										else if (m_pObjects[j].MVX() == false)
										{
											m_pObjects[j].MoveXtrue();
										}
									}
									else if (gameObject->GetMaxBounds().y >= gameObject2->GetCMinBounds().y)
									{
										/*m_pObjects[j].SetObjectTop();
										m_pObjects[j].Set();*/
										m_pObjects[i].OBJPos();
									}
									/*else if (m_pObjects[j].IsObjectTop() == true)
									{
										if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.5)
										{
											m_pObjects[j].SetObjectTop();
											m_pObjects[i].SetF1();
										}
									}*/
								}
								//else
								//{
								//	if (m_pObjects[i].IsObjectTop() == true)
								//	{
								//		//m_pObjects[i].OBJPos();
								//		m_pObjects[i].SetObjectTop();
								//		m_pObjects[i].SetF();
								//		m_pObjects[j].OBJPos();
								//	}
								//}
							}
							for (int h = 0; h < m_num3; h++)  //四角形と三角形
							{
								if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
								{
									if (GameObject* gameObject1 = dynamic_cast<GameObject*>(&m_pStair[h]))
									{
										//float liftposY = lift[a].GetMaxBounds().y;
										//m_pStair[h].Update();
										//ブロックの衝突
										if (m_pObjects[i].IsCollisionPoint(*gameObject1))
										{
											/*if (gameObject->GetCMinBounds().y >= gameObject1->GetMaxBounds().y)
											{
												m_pObjects[i].OBJPosy();
											}*/
											if (m_pObjects[i].IsGravity())
											{
												if (gameObject->GetCMinBounds().y + 0.05 >= gameObject1->GetMaxBounds().y)
												{
													//m_pStair[h].OBJPosy();
												/*	m_pStair[h].SetStairTop();
													m_pObjects[i].Set1();
													m_pStair[h].Set1();
													m_pStair[h].Set();*/
													m_pObjects[i].MoveObject(gameObject1->GetMaxBounds().y + 0.01f);
													if (m_pStair[h].IsMove() == true)
													{
														m_pObjects[i].Set1();
													}
													else
													{
														m_pObjects[i].SetF1();
														m_pObjects[i].framepls();
													}
												}
												//else if (gameObject->GetMaxBounds().y >= gameObject1->GetCMinBounds().y)
												//{
												//	/*m_pObjects[j].SetObjectTop();
												//	m_pObjects[j].Set();*/
												//	//m_pObjects[i].MoveObject(gameObject1->GetMaxBounds().y);
												//	m_pStair[h].OBJPosy();
												//}  //仮で消す
												else if (m_pObjects[i].IsXZ())
												{
													m_pObjects[i].OBJPos();
												}
												else
												{
													if (m_pStair[h].IsReverse() == true)
													{
														m_pObjects[i].SetReverseSlope();  //滑り落ちるようにする
													}
													else if(m_pStair[h].IsReverse() == false)
													{
														m_pObjects[i].SetSlope();
													}
												}
											}
											else if (gameObject1->GetCMinBounds().y + 0.2 >= gameObject->GetMaxBounds().y/*gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.1*/)
											{
												for (int h = 0; h < m_num3; h++)
												{
													m_pStair[h].Set1();
												}
												m_pStair[h].Set();
												m_pStair[h].OBJPosy();
											}
											else if (m_pObjects[i].IsXZ())  //連動時にブロック同士が衝突した場合の乗っているブロックの処理
											{
												m_pObjects[i].OBJPos();
												for (int k = 0; k < m_num; k++)
												{
													if (i == k)
													{
														k++;
													}
													if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[k]))
													{
														if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.1)
														{
															m_pObjects[k].OBJPos();
														}
													}
												}
												for (int l = 0; l < m_num3; l++)
												{
													m_pStair[l].OBJPos();
												}

											}
											//else if (gameObject->GetMaxBounds().y >= gameObject1->GetCMinBounds().y)  //追加
											//{
											//	/*m_pObjects[j].SetObjectTop();
											//	m_pObjects[j].Set();*/
											//	m_pObjects[i].OBJPosy();
											//}
											else
											{
												if (m_pStair[h].IsReverse() == true)
												{
													m_pObjects[i].SetReverseSlope();  //滑り落ちるようにする
												}
												else if (m_pStair[h].IsReverse() == false)
												{
													m_pObjects[i].SetSlope();
												}

											}
										}
									}
								}
							}
						}
					}
				}
				if (m_pObjects[i].SetR() == false)  //四角形のmoveがfalseの場合 四角形同士
				{
					for (int j = 0; j < m_num; j++)
					{
						if (j == i)
						{
							j++;
						}
						if (j > m_num)
						{
							continue;
						}
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
						{
							if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[j]))
							{

								// ブロックiとブロックjの当たり判定
								if (m_pObjects[i].col(*gameObject2) /*&& m_pObjects[j].col(*gameObject)*/)
								{
									if (gameObject->GetCMinBounds().y + 0.05 >= gameObject2->GetMaxBounds().y)
									{
										//m_pObjects[i].OBJPosy();
										m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y);
										if (m_pObjects[j].IsMove())
										{
											m_pObjects[i].Set1();
											m_pObjects[j].Set1();
											m_pObjects[i].Set();  //下のブロックのmoveがtrueだった場合連動
										}
									}
									else if (gameObject->GetCMinBounds().y + 0.07 >= gameObject2->GetMaxBounds().y)
									{
										//m_pObjects[i].OBJPosy();
										//m_pObjects[i].OBJPosy();
										m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y);
										if (m_pObjects[j].IsMove())
										{
											m_pObjects[i].Set1();
											m_pObjects[j].Set1();
											m_pObjects[i].Set();  //下のブロックのmoveがtrueだった場合連動
										}
									}
									else if (m_pObjects[j].IsXZ())  //連動時にブロック同士が衝突した場合の乗っているブロックの処理
									{
										if (gameObject2->GetCMinBounds().y + 0.05 >= gameObject->GetMaxBounds().y)
										{
											//m_pObjects[i].OBJPosy();
											//m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y + 0.05);
											//if (m_pObjects[j].IsMove())
											//{
											//	m_pObjects[i].Set();  //下のブロックのmoveがtrueだった場合連動
											//}
											//m_pObjects[j].OBJPos();
											for (int h = 0; h < m_num; h++)
											{
												if (h == i)
												{
													h++;
												}
												if (h == j)
												{
													h++;
												}
												if (h > m_num)
												{
													continue;
												}
												if (GameObject* gameObject3 = dynamic_cast<GameObject*>(&m_pObjects[h]))
												{
													if (m_pObjects[j].col(*gameObject3))
													{
														if (m_pObjects[j].IsXZ())
														{
															if (m_pObjects[h].GetPos().y > gameObject2->GetCMinBounds().y && m_pObjects[h].GetPos().y < gameObject2->GetMaxBounds().y)
															{
																m_pObjects[j].OBJPos();
															}
															else if(gameObject3->GetMaxBounds().y-0.05f>gameObject2->GetCMinBounds().y)
															{
																m_pObjects[j].OBJPos();
															}
														}
													}
												}
										}

											//m_pObjects[j].OBJPos();
											for (int l = 0; l < m_num3; l++)
											{
												//m_pStair[l].OBJPos();  //バグる可能性
												if (GameObject* gameObject3 = dynamic_cast<GameObject*>(&m_pObjects[j]))
												{
													if (GameObject* gameObject1 = dynamic_cast<GameObject*>(&m_pStair[l]))
													{
														//float liftposY = lift[a].GetMaxBounds().y;
														//m_pStair[h].Update();
														//ブロックの衝突
														if (m_pObjects[i].IsCollisionPoint(*gameObject1))
														{


															if (gameObject1->GetCMinBounds().y + 0.1 >= gameObject3->GetMaxBounds().y)
															{

																m_pStair[l].OBJPos();
															}
														}
													}
												}
											}

										}
										if (m_pObjects[j].IsAutoMove() == true)
										{
											m_pObjects[j].OBJPos();

											if (m_pObjects[j].MVX() == true)
											{
												m_pObjects[j].MoveXfalse();
											}
											else if (m_pObjects[j].MVX() == false)
											{
												m_pObjects[j].MoveXtrue();
											}

										}
									}

								}
							}

						}
					}
					for (int h = 0; h < m_num3; h++) //四角形と三角形
					{
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
						{
							if (GameObject* gameObject1 = dynamic_cast<GameObject*>(&m_pStair[h]))
							{
								//float liftposY = lift[a].GetMaxBounds().y;
								//m_pStair[h].Update();
								//ブロックの衝突
								if (m_pObjects[i].IsCollisionPoint(*gameObject1))
								{
									if (gameObject1->GetCMinBounds().y + 0.1 >= gameObject->GetMaxBounds().y)
									{
										m_pStair[h].OBJPosy();
										if (m_pObjects[i].IsMove())  //下のブロックのmoveがtrueだった場合連動
										{
											m_pStair[h].Set();
											m_pStair[h].Set1();
										}
									}
									if (gameObject->GetCMinBounds().y + 0.1 >= gameObject1->GetMaxBounds().y)
									{
										m_pObjects[i].MoveObject(gameObject->GetMaxBounds().y + 0.05f);
										if (m_pStair[h].IsMove())  //下のブロックのmoveがtrueだった場合連動
										{
											m_pObjects[i].Set();
											for (int k = 0; k < m_num; k++)
											{
												m_pObjects[k].Set1();
											}
										}
									}
									if (gameObject1->GetCMinBounds().y >= gameObject->GetMaxBounds().y)
									{
										m_pStair[h].OBJPosy();  //すり抜け防止
									/*	if (m_pObjects[i].IsMove())
										{
											m_pStair[h].Set();
											m_pStair[h].Set1();
										}*/
									}
									else if (m_pObjects[i].IsXZ())
									{ //連動時にブロック同士が衝突した場合の乗っているブロックの処理
										for (int k = 0; k < m_num; k++)
										{
											if (k == i)
											{
												k++;
											}
											//m_pObjects[k].OBJPos();  //スタック問題
											for (int l = 0; l < m_num3; l++)
											{
												if (l == h)
												{
													l++;
												}
												if (l > m_num3)
												{
													continue;
												}
												/*if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[k]))
												{
													if (GameObject* gameObject3 = dynamic_cast<GameObject*>(&m_pStair[l]))
													{
														if (m_pObjects[k].IsCollisionPoint(*gameObject3))
														{
															if (gameObject3->GetCMinBounds().y + 0.15 >= gameObject3->GetMaxBounds().y)
															{
																m_pStair[l].OBJPos();
															}
														}
													}

												}*/
												//m_pStair[l].OBJPos(); //スタック問題
											}

										}
										//m_pStair[h].OBJPos();
										for (int k = 0; k < m_num3; k++)
										{
											/*if (h == i || h == j)
											{
												h++;
											}

											for (int l = 0; l < m_num3; l++)
											{
												m_pStair[l].OBJPos();
											}*/
											if (h == k)
											{
												k++;
											}
											//m_pStair[k].OBJPos();

										}
									}
								}

							}
						}
					}
				}
				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pStair[b]))  //三角形、憑依
				{
					//憑依のため・ブロックとプレイヤーが当たった場合
					if (m_pPlayer->IsCollisionPoint(*gameObject))
					{

						if (IsKeyPress('Q'))//(imanagerO.getKey(0) & 0b011)
						{
							// effectこうしん
							float X = m_pPlayer->GetPosX() / EFFECT_PLAYER_MAX_POS_X * EFFECT_MAX_POS_X;

							float Y = m_pPlayer->GetPosY() - 0.4f;

							float Z = (m_pPlayer->GetPosZ() - EFFECT_PLAYER_MIN_POS_Z) /
								(EFFECT_PLAYER_MAX_POS_Z - EFFECT_PLAYER_MIN_POS_Z) *
								EFFECT_MAX_POS_Z + EFFECT_MIN_POS_Z;
							m_EffectHandle = LibEffekseer::GetManager()->Play(m_Effect, X, Y, Z);
							//移動させる時
							Effekseer::Matrix43 EffecMat = LibEffekseer::GetManager()->GetBaseMatrix(m_EffectHandle);
							EffecMat.Translation(0.0f, -1.0f, 0.0f);
							LibEffekseer::GetManager()->SetBaseMatrix(m_EffectHandle, EffecMat);
							LibEffekseer::GetManager()->SetScale(m_EffectHandle, 0.15f, 0.15f, 0.15f);

							m_pPlayer->SetOk();
							m_pPlayer->HPlayerPos();
							m_pStair[b].Set();
							m_pStair[b].Set1();
							m_pStair[b].SetColgravityfalse();
							//m_pObjects[i].Modelchg();
						}
					}
					//憑依解除
					/*if (!m_pPlayer->HIsCollidingWith(*gameObject))
					{*/
					if (IsKeyPress('E'))//(imanagerO.getKey(1) & 0b011)
					{
						if (m_pStair[b].SetR() == true)
						{
							m_pPlayer->SetNOk();
							m_pPlayer->PlayerPos();
							for (int j = 0; j < m_num3; j++)
							{
								m_pStair[j].SetF();
								/*if (m_pObjects[i].SetR() == true)
								{
									m_pObjects[i].SetF();
								}*/
								m_pStair[j].SetColgravity();
							}
							m_pStair[b].SetF1();
							//m_pStair[b].SetColgravity();
							//m_pObjects[i].Update(); //重力を消してからUpdateを経ずにfalseに入っている
							//m_pObjects[i].Modelchg2();
						}
					}

					//}
				}
				if (m_pStair[b].SetR() == true /*|| m_pObjects[i].SetR() == false*/)  //三角形のmoveがtrue 三角形と四角形
				{
					for (int j = 0; j < m_num; j++)
					{
						/*if (j == i)
						{
							j++;
						}*/
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pStair[b]))
						{
							if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[j]))
							{
								//ブロックの衝突
								if (m_pObjects[j].IsCollisionPoint(*gameObject))
								{
									if (m_pStair[b].IsGravity())
									{
										if (gameObject->GetCMinBounds().y + 0.05 >= gameObject2->GetMaxBounds().y)
										{

											m_pStair[b].SetSlopeY(gameObject2->GetMaxBounds().y + 0.01f);
											if (m_pObjects[j].IsMove() == true)
											{
												m_pStair[b].Set1();
											}
											else
											{
												m_pStair[b].SetF1() ;
												m_pStair[b].framepls();
											}
										}
										else if (gameObject->GetMaxBounds().y >= gameObject2->GetCMinBounds().y)
										{
											/*m_pObjects[j].SetObjectTop();
											m_pObjects[j].Set();*/
											m_pStair[b].SetSlopeY(gameObject2->GetMaxBounds().y);
											for (int h = 0; h < m_num; h++)
											{
												m_pObjects[h].Set1();
												m_pStair[h].Set1();
											}
											if (m_pObjects[j].IsMove() == true)
											{
												m_pStair[b].Set1();
											}
											else
											{
												m_pStair[b].SetF1();
												m_pStair[b].framepls();
											}
										}
										else if (m_pStair[b].IsXZ())
										{
											m_pStair[b].OBJPos();
										}
									}
									else if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.1)
									{
										for (int h = 0; h < m_num; h++)
										{
											m_pObjects[h].Set1();
											m_pStair[h].Set1();
										}
										//m_pObjects[j].SetObjectTop();
										m_pObjects[j].Set();
										m_pObjects[j].OBJPosy();

									}
									//else if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.5)
									//{
									//	//for (int h = 0; h < m_num; h++)
									//	//{
									//	//	m_pObjects[h].Set1();
									//	//	m_pStair[h].Set1();
									//	//}
									//	////m_pObjects[j].SetObjectTop();
									//	//m_pObjects[j].Set();
									//	//m_pObjects[j].OBJPosy();
									//	m_pStair[b].OBJPos();
									//}
									else if (m_pObjects[j].IsXZ() && m_pObjects[j].IsAutoMove() == true)
									{
										m_pObjects[j].OBJPos();
										m_pStair[b].OBJPos();
										if (m_pObjects[j].MVX() == true)
										{
											m_pObjects[j].MoveXfalse();
										}
										else if (m_pObjects[j].MVX() == false)
										{
											m_pObjects[j].MoveXtrue();
										}
									}
									else if (m_pStair[b].IsXZ())
									{
										for (int l = 0; l < m_num3; l++)
										{
											m_pStair[l].OBJPos();
										}
										for (int l = 0; l < m_num; l++)
										{
											m_pObjects[l].OBJPos();
										}
										//m_pObjects[j].OBJPos();
										/*for (int k = 0; k < m_num3; k++)
										{
											if (b == k)
											{
												k++;
											}
											if (GameObject* gameObject3 = dynamic_cast<GameObject*>(&m_pStair[k]))
											{
												if (m_pObjects[j].IsCollisionPoint(*gameObject2))
												{
													if (gameObject2->GetMaxBounds().y <= gameObject3->GetCMinBounds().y + 0.1)
													{
														m_pStair[k].OBJPos();
														m_pObjects[j].OBJPos();
													}
												}


											}
										}*/
										//m_pObjects[j].SetF();
									}
								}
								//else
								//{
								//	if (m_pStair[b].IsStairTop() == true)
								//	{
								//		//m_pObjects[j].OBJPos();
								//		m_pStair[b].SetStairTop();
								//		//m_pObjects[j].SetF();
								//		m_pObjects[j].OBJPos();
								//	}
								//}


							}
						}


					}
					for (int j = 0; j < m_num3; j++)//三角形のmoveがtrue 三角形と三角形
					{
						if (j == b)
						{
							j++;
						}
						if (j >= m_num3)
						{
							continue;
						}
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pStair[b]))
						{
							if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pStair[j]))
							{
								//ブロックの衝突
								if (m_pStair[b].IsCollisionPoint(*gameObject2))
								{
									if (m_pStair[b].IsGravity())
									{
										if (gameObject->GetCMinBounds().y + 0.05 >= gameObject2->GetMaxBounds().y)
										{

											//m_pStair[b].OBJPosy();
											m_pStair[b].SetSlopeY(gameObject2->GetMaxBounds().y + 0.05f);
											if (m_pStair[b].IsMove() == true)
											{
												m_pStair[i].Set1();
											}
											else
											{
												m_pStair[b].SetF1();
												m_pStair[b].framepls();
											}
										}
										//else if (gameObject->GetMaxBounds().y >= gameObject2->GetCMinBounds().y)
										//{
										//	/*m_pObjects[j].SetObjectTop();
										//	m_pObjects[j].Set();*/
										//	m_pStair[b].OBJPosy();
										//	for (int h = 0; h < m_num; h++)
										//	{
										//		m_pObjects[h].Set1();
										//		m_pStair[h].Set1();
										//	}
										//}
										//else if (gameObject->GetMaxBounds().y  <= gameObject2->GetCMinBounds().y )
										//{
										//	/*m_pObjects[j].SetObjectTop();
										//	m_pObjects[j].Set();*/
										//	m_pStair[b].OBJPosy();
										//	m_pStair[j].OBJPosy();
										//	for (int h = 0; h < m_num; h++)
										//	{
										//		m_pObjects[h].Set1();
										//		//m_pStair[h].Set1();
										//	}
										//}
										else if (m_pStair[b].GetPos().y < gameObject2->GetMaxBounds().y && m_pStair[b].GetPos().y > gameObject2->GetCMinBounds().y)
										{
											if (m_pStair[b].IsXZ())
											{
												m_pStair[b].OBJPos();
											}
										}
										else if (m_pStair[b].GetPos().y > m_pStair[j].GetPos().y)
										{
											if (m_pStair[j].IsReverse() == false)
											{
												m_pStair[b].SetSlope();
											}
											else if (m_pStair[j].IsReverse() == true)
											{
												m_pStair[b].SetReverseSlope();
											}

										}
										/*else
										{
											m_pStair[b].SetSlope();


										}*/
									}
									else if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.1)
									{
										//for (int h = 0; h < m_num3; h++)
										//{
										//	//m_pObjects[h].Set1();
										//	m_pStair[h].Set1();
										//}
										////m_pObjects[j].SetObjectTop();
										//m_pStair[j].Set();
										m_pStair[j].OBJPosy();
										m_pStair[b].OBJPosy();
										//m_pStair[b].SetSlopeY(gameObject->GetMaxBounds().y + 0.01);
									}

									else if (m_pStair[b].IsXZ())
									{
										for (int l = 0; l < m_num3; l++)
										{
											m_pStair[l].OBJPos();
										}
										for (int l = 0; l < m_num; l++)
										{
											m_pObjects[l].OBJPos();
										}
										//m_pObjects[j].OBJPos();
										/*for (int k = 0; k < m_num3; k++)
										{
											if (b == k)
											{
												k++;
											}
											if (GameObject* gameObject3 = dynamic_cast<GameObject*>(&m_pStair[k]))
											{
												if (m_pObjects[j].IsCollisionPoint(*gameObject2))
												{
													if (gameObject2->GetMaxBounds().y <= gameObject3->GetCMinBounds().y + 0.1)
													{
														m_pStair[k].OBJPos();
														m_pObjects[j].OBJPos();
													}
												}


											}
										}*/
										//m_pObjects[j].SetF();
									}
									else
									{
										if (m_pStair[j].IsReverse() == false)
										{
											m_pStair[b].SetSlope();
										}
										else if (m_pStair[j].IsReverse() == true)
										{
											m_pStair[b].SetReverseSlope();
										}

									}
								}
								//else
								//{
								//	if (m_pStair[b].IsStairTop() == true)
								//	{
								//		//m_pObjects[j].OBJPos();
								//		m_pStair[b].SetStairTop();
								//		//m_pObjects[j].SetF();
								//		m_pObjects[j].OBJPos();
								//	}
								//}


							}
						}


					}
				}

				if (m_pStair[b].SetR() == false) //三角形のmoveがfalse 三角形と四角形
				{
					for (int j = 0; j < m_num; j++)
					{
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pStair[b]))
						{
							if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[j]))
							{

								// ブロックiとブロックjの当たり判定
								if (m_pObjects[j].IsCollisionPoint(*gameObject)) /*&& m_pObjects[j].col(*gameObject)*/
								{
									if (gameObject->GetCMinBounds().y + 0.05 >= gameObject2->GetMaxBounds().y)
									{

										m_pStair[b].SetSlopeY(gameObject2->GetMaxBounds().y + 0.05f);

									}
									if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y /*+ 0.05f*/)
									{
										m_pObjects[j].OBJPosy();
									}
									if (m_pObjects[j].IsXZ())
									{
										for (int h = 0; h < m_num3; h++)  //コメント
										{
											//m_pStair[h].OBJPos();
										}

										for (int h = 0; h < m_num; h++)
										{
											//if (h == j)
											//{
											//	h++;
											//}
											//if (GameObject* gameObject3 = dynamic_cast<GameObject*>(&m_pObjects[h]))
											//{
											//	if (gameObject2->GetMaxBounds().y <= gameObject->GetCMinBounds().y + 0.1)
											//	{
											//		//m_pStair[i].OBJPos();
											//		m_pStair[b].OBJPos();
											//		m_pObjects[j].OBJPos();
											//		m_pObjects[j].SetObjectTop();
											//		m_pObjects[j].SetF1();
											//	}
											//}
										}
									}
									if (m_pObjects[j].IsAutoMove() == true)
									{
										m_pObjects[j].OBJPos();

										if (m_pObjects[j].MVX() == true)
										{
											m_pObjects[j].MoveXfalse();
										}
										else if (m_pObjects[j].MVX() == false)
										{
											m_pObjects[j].MoveXtrue();
										}

									}
								}
							}
						}
					}
					for (int h = 0; h < m_num; h++)
					{
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[h]))
						{
							if (GameObject* gameObject1 = dynamic_cast<GameObject*>(&m_pStair[b]))
							{
								//float liftposY = lift[a].GetMaxBounds().y;
								//m_pStair[h].Update();
								//ブロックの衝突
								if (m_pObjects[h].IsCollisionPoint(*gameObject1))
								{
									if (gameObject1->GetCMinBounds().y + 0.15 >= gameObject->GetMaxBounds().y)
									{
										m_pStair[b].OBJPosy();
										//m_pStair[b].SetSlopeY(gameObject->GetMaxBounds().y + 0.01);
										if (m_pObjects[h].IsMove())
										{
											m_pStair[b].Set();
											m_pStair[b].Set1();

										}
									}
									if (gameObject->GetCMinBounds().y + 0.15 >= gameObject1->GetMaxBounds().y)
									{
										m_pObjects[h].OBJPosy();
										//m_pStair[h].SetSlopeY(gameObject1->GetMaxBounds().y + 0.01);
										if (m_pStair[b].IsMove())
										{
											m_pObjects[h].Set();
											for (int k = 0; k < m_num; k++)
											{
												m_pObjects[k].Set1();
											}
										}
									}
								}

							}
						}
					}
					for (int h = 0; h < m_num3; h++) //三角形のmoveがfalse 三角形と四角形
					{
						if (h == b)
						{
							h++;
						}
						if (h >= m_num3)
						{
							break;
						}
						else if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pStair[b]))
						{
							if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pStair[h]))
							{
								//ブロックの衝突
								if (m_pStair[b].IsCollisionPoint(*gameObject2))
								{
									if (gameObject->GetCMinBounds().y + 0.05 >= gameObject2->GetMaxBounds().y)
									{

										//m_pStair[b].OBJPosy();
										m_pStair[b].SetSlopeY(gameObject2->GetMaxBounds().y + 0.05f);
										if (m_pStair[h].IsMove())
										{
											m_pStair[b].Set();
											m_pStair[b].Set1();
											m_pStair[h].Set1();
										}
									}
									else if (gameObject->GetCMinBounds().y + 0.07 >= gameObject2->GetMaxBounds().y)
									{

										//m_pStair[b].OBJPosy();
										m_pStair[b].SetSlopeY(gameObject2->GetMaxBounds().y + 0.07f);
									/*	if (m_pStair[h].IsMove())
										{
											m_pStair[b].Set();
											m_pStair[b].Set1();
											m_pStair[h].Set1();
										}*/
									}
									else if (gameObject->GetMaxBounds().y >= gameObject2->GetCMinBounds().y)
									{
										m_pStair[b].OBJPosy();
										//m_pStair[h].SetSlopeY(gameObject->GetMaxBounds().y + 0.01);
										/*if (m_pStair[h].IsMove())
										{
											m_pStair[b].Set();
											m_pStair[b].Set1();
										}*/
									}

									else if (m_pStair[h].IsXZ())
									{
										for (int k = 0; k < m_num3; k++)
										{
											if (h == k)
											{
												continue;
											}
											m_pStair[k].OBJPos();
										}
									}
								}
							}
						}
					}

				}
			}
		}
	}
#endif
	m_pPlayer->Update(tick);

	//for (int t = 0; t < m_num6; t++)
	//{// 動くブロック
	//	m_pObjectsAuto->Update();
	//}

	for (int t = 0; t < m_num5; t++)
	{// 
		m_pTomegu[t].Update();
	}


	for (int y = 0; y < m_num4; y++)
	{// 床
		m_pYuka[y].Update();
	}

	for (int b = 0; b < m_num3; b++)
	{// ゴール
		m_pStair[b].Update();
	}
	for (int i = 0; i < m_num; i++)
	{
		m_pObjects[i].Update();
	}





	for (int i = 0; i < m_num; i++)
	{

		for (int b = 0; b < m_num3; b++)
		{
			for (int a = 0; a < m_num1; a++)
			{
				//m_pLift_obj[a].Update();
				//if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
				//{
				//	if (Lift_Obj* lift = dynamic_cast<Lift_Obj*>(&m_pLift_obj[a]))
				//	{

				//		//ブロックの衝突
				//		if (m_pObjects[i].col(*lift))
				//		{
				//			m_pObjects[i].OBJPos();
				//		}
				//	}
				//}
				//for (int l = 0; l < m_num2; l++)
				//{

				//	m_pLever[l].Update();//レバーとプレイヤー
				//	if (Lever* lever = dynamic_cast<Lever*>(&m_pLever[l]))
				//	{
				//		if (m_pPlayer->IsCollidingWith(*lever))
				//		{
				//			if (IsKeyPress('3'))
				//			{
				//				m_pLift_obj[a].SetFLever();
				//			}
				//			if (IsKeyPress('4'))
				//			{
				//				m_pLift_obj[a].SetLever();
				//			}
				//		}
				//	}
				//}

				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
				{
					if (Lift_Obj* lift = dynamic_cast<Lift_Obj*>(&m_pLift_obj[a]))
					{
						float liftposY = lift[a].GetMaxBounds().y;
						m_pLift_obj[a].Update();
						//ブロックの衝突
						if (m_pObjects[i].col(*lift))
						{
							if (gameObject->GetMaxBounds().y <= lift->GetMaxBounds().y - 0.05f)
							{
								m_pObjects[i].OBJPos();
							}
							else if (gameObject->GetCMinBounds().y + 0.05 >= lift->GetMaxBounds().y)
							{
								m_pObjects[i].MoveObject(lift->GetMaxBounds().y);
								m_pObjects[i].SetF1();
								m_pObjects[i].framepls();
							}
							/*	else if (gameObject->GetCMinBounds().y + 0.03 <= lift->GetMaxBounds().y)
								{
									m_pObjects[i].MoveObject(lift->GetMaxBounds().y);
									m_pObjects[i].SetF1();
									m_pObjects[i].framepls();

								}*/
							else if (m_pObjects[i].IsXZ())
							{
								m_pObjects[i].OBJPosy();
							}
							if (m_pLift_obj[a].IsMove())
							{
								if (gameObject->GetCMinBounds().y > lift->GetMaxBounds().y)
								{
									m_pObjects[i].MoveObject(liftposY);
								}
							}
							if (gameObject->GetMaxBounds().y <= lift->GetMaxBounds().y - 0.08f)
							{
								if (m_pLift_obj[a].IsMove() == true)
								{
									if (m_pObjects[i].IsMove() == false)
									{
										m_pLift_obj[a].MoveLift(gameObject->GetMaxBounds().y + 0.1f);
									}
									//m_pObjects[i].OBJPosy();

								}
							}
						}
					}
				}
				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pStair[b]))
				{
					if (Lift_Obj* lift = dynamic_cast<Lift_Obj*>(&m_pLift_obj[a]))
					{
						float liftposY = lift[a].GetMaxBounds().y;
						m_pLift_obj[a].Update();
						//ブロックの衝突
						if (m_pStair[b].col(*lift))
						{
							//m_pObjects[i].OBJPosy();
							if (gameObject->GetMaxBounds().y <= lift->GetMaxBounds().y - 0.05f)
							{
								m_pStair[b].OBJPos();
							}
							else if (gameObject->GetCMinBounds().y + 0.5 >= lift->GetMaxBounds().y)
							{
								m_pStair[b].SetSlopeY(lift->GetMaxBounds().y);
								m_pStair[b].SetF1();
								m_pStair[b].framepls();
							}
							/*else if (gameObject->GetCMinBounds().y + 0.03 <= lift->GetMaxBounds().y)
							{
								m_pStair[b].SetSlopeY(lift->GetMaxBounds().y);
								m_pStair[b].SetF1();
								m_pStair[b].framepls();
							}*/
							else if (m_pObjects[i].IsXZ())
							{
								m_pStair[b].SetSlopeY(lift->GetMaxBounds().y);
							}
							if (m_pLift_obj[a].IsMove())
							{
								m_pStair[b].MoveStair(liftposY);
							}
							if (gameObject->GetMaxBounds().y <= lift->GetMaxBounds().y - 0.08f)
							{
								if (m_pLift_obj[a].IsMove() == true)
								{
									if (m_pStair[b].IsMove() == false)
									{
										m_pLift_obj[a].MoveLift(gameObject->GetMaxBounds().y + 0.1f);
									}
									//m_pObjects[i].OBJPosy();

								}
							}
						}
					}
				}
				for (int l = 0; l < m_num2; l++)
				{
					for (int mg = 0; mg < m_num6; mg++)
					{// ゴール
						m_pMagnet[mg].Update();


						m_pLever[l].Update();//レバーとプレイヤー
						if (Lever* lever = dynamic_cast<Lever*>(&m_pLever[l]))
						{
							if (m_pPlayer->IsCollidingWith(*lever))
							{
								if (imanagerO.getKey(2) & 0b011)
								{
									m_pLever[l].SetMoveTrue();
									m_pMagnet[mg].Set();
									m_pLift_obj[a].SetMoveTrue();
									m_pPlayer->SetOk();
									m_pPlayer->HPlayerPos();
								}
							}
							if (imanagerO.getKey(3) & 0b011)
							{
								m_pLever[l].SetMoveFalse();
								m_pMagnet[mg].SetF();
								m_pLift_obj[a].SetMoveFalse();
								m_pPlayer->SetNOk();
								m_pPlayer->PlayerPos();
							}
						}
					}
				}

				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
				{
					//////ブロックとプレイヤー衝突
					//if (m_pPlayer->IsCollidingWith(*gameObject)) {
					//	// 衝突時の処理
					//	m_pPlayer->PlayerPos();
					//}
				}
				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
				{
					//憑依のため・ブロックとプレイヤーが当たった場合
					if ((m_pPlayer->HIsCollidingWith(*gameObject)) && m_pObjects[i].possessionok() == true)
					{

						if (imanagerO.getKey(2) & 0b011)//(imanagerO.getKey(0) & 0b011)
						{
							//// effectこうしん
							// effectこうしん
							float X = m_pPlayer->GetPosX() / EFFECT_PLAYER_MAX_POS_X * EFFECT_MAX_POS_X;

							float Y = m_pPlayer->GetPosY() - 0.9f;

							float Z = (m_pPlayer->GetPosZ() - EFFECT_PLAYER_MIN_POS_Z) /
								(EFFECT_PLAYER_MAX_POS_Z - EFFECT_PLAYER_MIN_POS_Z) *
								EFFECT_MAX_POS_Z + EFFECT_MIN_POS_Z;
							m_EffectHandle = LibEffekseer::GetManager()->Play(m_Effect, X, Y, Z);

							//移動させる時
							Effekseer::Matrix43 EffecMat = LibEffekseer::GetManager()->GetBaseMatrix(m_EffectHandle);
							EffecMat.Translation(0.0f, -1.0f, 0.0f);
							LibEffekseer::GetManager()->SetBaseMatrix(m_EffectHandle, EffecMat);
							LibEffekseer::GetManager()->SetScale(m_EffectHandle, 0.2f, 0.2f, 0.2f);

							m_pPlayer->SetOk();
							m_pPlayer->HPlayerPos();
							m_pObjects[i].Set();
							m_pObjects[i].Set1();
							m_pObjects[i].SetColgravityfalse();
							//m_pObjects[i].Modelchg();
						}
					}
					//憑依解除
					/*if (!m_pPlayer->HIsCollidingWith(*gameObject))
					{*/
					if (imanagerO.getKey(3) & 0b011)//(imanagerO.getKey(1) & 0b011)
					{
						if (m_pObjects[i].SetR() == true)
						{
							m_pPlayer->SetNOk();
							m_pPlayer->PlayerPos();
							for (int j = 0; j < m_num; j++)
							{
								m_pObjects[j].SetF();
								/*if (m_pObjects[i].SetR() == true)
								{
									m_pObjects[i].SetF();
								}*/
								m_pObjects[j].SetColgravity();
								m_pObjects[j].OnBoxfalse();
								//m_pObjects[i].Setgravityfalse();
							}
							//gravityをfalseにする

							//m_pObjects[i].SetF1();

							//m_pObjects[i].Update(); //重力を消してからUpdateを経ずにfalseに入っている
							//m_pObjects[i].Modelchg2();
						}
					}
				}



				if (m_pObjects[i].SetR() == true /*|| m_pObjects[i].SetR() == false*/)  //四角形同士
				{
					for (int j = 0; j < m_num; j++)
					{
						if (j == i)
						{
							j++;
						}
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
						{
							if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[j]))
							{

								// ブロックiとブロックjの当たり判定
								if (m_pObjects[i].col(*gameObject2) /*&& m_pObjects[j].col(*gameObject)*/)
								{
									//m_pObjects[j].OBJPos();
									//MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
									if (m_pObjects[i].IsGravity())  //ジャンプ中の重力がある場合
									{
										if (gameObject->GetCMinBounds().y + 0.05f >= gameObject2->GetMaxBounds().y)  //上辺の当たり判定
										{
											//m_pObjects[i].OBJPosy();  //y以外過去座標へ
											m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y + 0.01f);
											m_pObjects[i].OnBoxtrue();
											if (m_pObjects[j].IsMove() == true)
											{
												m_pObjects[i].Set1();

											}
											else
											{
												m_pObjects[i].SetF1();
												m_pObjects[i].framepls();
											}
											if (m_pObjects[j].IsOnBox())
											{
												m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y + 0.05f);
											}
										}
										//else if (gameObject->GetMaxBounds().y >= gameObject2->GetCMinBounds().y)  //すり抜け防止
										//{
										//	/*m_pObjects[j].SetObjectTop();
										//	m_pObjects[j].Set();*/
										//	m_pObjects[j].OBJPosy();
										//	//m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y + 0.02);
										//}
										else if (gameObject->GetCMinBounds().y + 0.07 >= gameObject2->GetMaxBounds().y)  //すり抜け防止
										{
											/*m_pObjects[j].SetObjectTop();
											m_pObjects[j].Set();*/
											m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y + 0.1f);
											//m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y + 0.02);
										}
										//else if (m_pObjects[i].IsXZ())  //x,z軸から当たった場合
										//{
										//	m_pObjects[i].OBJPos();  //すべて過去座標へ
										//}
									}
									else if (gameObject2->GetCMinBounds().y + 0.1f >= gameObject->GetMaxBounds().y/*gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.1*/)
									{
										for (int h = 0; h < m_num; h++)  //ジャンプ中の重力がない時の連動
										{
											m_pObjects[h].Set1();
										}
										m_pObjects[j].Set();
										m_pObjects[j].OBJPosy();
									}
									else if (gameObject2->GetCMinBounds().y >= gameObject->GetMaxBounds().y/*gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.1*/)
									{
										for (int h = 0; h < m_num; h++)  //すり抜け防止
										{
											m_pObjects[h].Set1();
										}
										//m_pObjects[j].Set();
										m_pObjects[j].OBJPosy();
									}
									//else if (gameObject->GetMaxBounds().y >= gameObject2->GetCMinBounds().y)
									//{
									//	/*m_pObjects[j].SetObjectTop();
									//	m_pObjects[j].Set();*/
									//	//m_pObjects[i].OBJPos();
									//}
									else if (m_pObjects[i].IsXZ())  //x,z軸から当たった場合
									{
										/*m_pObjects[i].OBJPos();
										m_pObjects[j].OBJPos();*/
										for (int h = 0; h < m_num; h++)
										{
											m_pObjects[h].OBJPos();
										}
										//m_pObjects[i].SetF();
									}
									else if (m_pObjects[j].IsXZ() && m_pObjects[j].IsAutoMove() == true)  //x,z軸から当たった場合 j automove
									{
										/*m_pObjects[i].OBJPos();
										m_pObjects[j].OBJPos();*/
										/*for (int h = 0; h < m_num; h++)
										{
											m_pObjects[h].OBJPos();
										}*/
										//m_pObjects[i].SetF();
										m_pObjects[i].OBJPos();
										m_pObjects[j].OBJPos();
										if (m_pObjects[j].MVX() == true)
										{
											m_pObjects[j].MoveXfalse();
										}
										else if (m_pObjects[j].MVX() == false)
										{
											m_pObjects[j].MoveXtrue();
										}
									}
									else if (gameObject->GetMaxBounds().y >= gameObject2->GetCMinBounds().y)
									{
										/*m_pObjects[j].SetObjectTop();
										m_pObjects[j].Set();*/
										m_pObjects[i].OBJPos();
									}
									/*else if (m_pObjects[j].IsObjectTop() == true)
									{
										if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.5)
										{
											m_pObjects[j].SetObjectTop();
											m_pObjects[i].SetF1();
										}
									}*/
								}
								//else
								//{
								//	if (m_pObjects[i].IsObjectTop() == true)
								//	{
								//		//m_pObjects[i].OBJPos();
								//		m_pObjects[i].SetObjectTop();
								//		m_pObjects[i].SetF();
								//		m_pObjects[j].OBJPos();
								//	}
								//}
							}
							for (int h = 0; h < m_num3; h++)  //四角形と三角形
							{
								if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
								{
									if (GameObject* gameObject1 = dynamic_cast<GameObject*>(&m_pStair[h]))
									{
										//float liftposY = lift[a].GetMaxBounds().y;
										//m_pStair[h].Update();
										//ブロックの衝突
										if (m_pObjects[i].IsCollisionPoint(*gameObject1))
										{
											/*if (gameObject->GetCMinBounds().y >= gameObject1->GetMaxBounds().y)
											{
												m_pObjects[i].OBJPosy();
											}*/
											if (m_pObjects[i].IsGravity())
											{
												if (gameObject->GetCMinBounds().y + 0.05 >= gameObject1->GetMaxBounds().y)
												{
													//m_pStair[h].OBJPosy();
												/*	m_pStair[h].SetStairTop();
													m_pObjects[i].Set1();
													m_pStair[h].Set1();
													m_pStair[h].Set();*/
													m_pObjects[i].MoveObject(gameObject1->GetMaxBounds().y + 0.01f);
													if (m_pStair[h].IsMove() == true)
													{
														m_pObjects[i].Set1();
													}
													else
													{
														m_pObjects[i].SetF1();
														m_pObjects[i].framepls();
													}
												}
												//else if (gameObject->GetMaxBounds().y >= gameObject1->GetCMinBounds().y)
												//{
												//	/*m_pObjects[j].SetObjectTop();
												//	m_pObjects[j].Set();*/
												//	//m_pObjects[i].MoveObject(gameObject1->GetMaxBounds().y);
												//	m_pStair[h].OBJPosy();
												//}  //仮で消す
												else if (m_pObjects[i].IsXZ())
												{
													m_pObjects[i].OBJPos();
												}
												else
												{
													if (m_pStair[h].IsReverse() == true)
													{
														m_pObjects[i].SetReverseSlope();  //滑り落ちるようにする
													}
													else if (m_pStair[h].IsReverse() == false)
													{
														m_pObjects[i].SetSlope();
													}
												}
											}
											else if (gameObject1->GetCMinBounds().y + 0.2 >= gameObject->GetMaxBounds().y/*gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.1*/)
											{
												for (int h = 0; h < m_num3; h++)
												{
													m_pStair[h].Set1();
												}
												m_pStair[h].Set();
												m_pStair[h].OBJPosy();
												m_pObjects[i].Set1();
											}
											else if (m_pObjects[i].IsXZ())  //連動時にブロック同士が衝突した場合の乗っているブロックの処理
											{
												m_pObjects[i].OBJPos();
												for (int k = 0; k < m_num; k++)
												{
													if (i == k)
													{
														k++;
													}
													if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[k]))
													{
														if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.1)
														{
															m_pObjects[k].OBJPos();
														}
													}
												}
												for (int l = 0; l < m_num3; l++)
												{
													m_pStair[l].OBJPos();
												}

											}
											//else if (gameObject->GetMaxBounds().y >= gameObject1->GetCMinBounds().y)  //追加
											//{
											//	/*m_pObjects[j].SetObjectTop();
											//	m_pObjects[j].Set();*/
											//	m_pObjects[i].OBJPosy();
											//}
											else
											{
												if (m_pStair[h].IsReverse() == true)
												{
													m_pObjects[i].SetReverseSlope();  //滑り落ちるようにする
												}
												else if (m_pStair[h].IsReverse() == false)
												{
													m_pObjects[i].SetSlope();
												}

											}
										}
									}
								}
							}
						}
					}
				}
				if (m_pObjects[i].SetR() == false)  //四角形のmoveがfalseの場合 四角形同士
				{
					for (int j = 0; j < m_num; j++)
					{
						if (j == i)
						{
							j++;
						}
						if (j > m_num)
						{
							continue;
						}
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
						{
							if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[j]))
							{

								// ブロックiとブロックjの当たり判定
								if (m_pObjects[i].col(*gameObject2) /*&& m_pObjects[j].col(*gameObject)*/)
								{
									if (gameObject->GetCMinBounds().y + 0.05 >= gameObject2->GetMaxBounds().y)
									{
										//m_pObjects[i].OBJPosy();
										m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y);
										if (m_pObjects[j].IsMove())
										{
											m_pObjects[i].Set1();
											m_pObjects[j].Set1();
											m_pObjects[i].Set();  //下のブロックのmoveがtrueだった場合連動
										}
									}
									else if (gameObject->GetCMinBounds().y + 0.07 >= gameObject2->GetMaxBounds().y)
									{
										//m_pObjects[i].OBJPosy();
										//m_pObjects[i].OBJPosy();
										m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y);
										if (m_pObjects[j].IsMove())
										{
											m_pObjects[i].Set1();
											m_pObjects[j].Set1();
											m_pObjects[i].Set();  //下のブロックのmoveがtrueだった場合連動
										}
									}
									else if (m_pObjects[j].IsXZ())  //連動時にブロック同士が衝突した場合の乗っているブロックの処理
									{
										if (gameObject2->GetCMinBounds().y + 0.05 >= gameObject->GetMaxBounds().y)
										{
											//m_pObjects[i].OBJPosy();
											//m_pObjects[i].MoveObject(gameObject2->GetMaxBounds().y + 0.05);
											//if (m_pObjects[j].IsMove())
											//{
											//	m_pObjects[i].Set();  //下のブロックのmoveがtrueだった場合連動
											//}
											//m_pObjects[j].OBJPos();
											for (int h = 0; h < m_num; h++)
											{
												if (h == i)
												{
													h++;
												}
												if (h == j)
												{
													h++;
												}
												if (h > m_num)
												{
													continue;
												}
												if (GameObject* gameObject3 = dynamic_cast<GameObject*>(&m_pObjects[h]))
												{
													if (m_pObjects[j].col(*gameObject3))
													{
														if (m_pObjects[j].IsXZ())
														{
															if (m_pObjects[h].GetPos().y > gameObject2->GetCMinBounds().y && m_pObjects[h].GetPos().y < gameObject2->GetMaxBounds().y)
															{
																m_pObjects[j].OBJPos();
															}
															else if (gameObject3->GetMaxBounds().y - 0.05f > gameObject2->GetCMinBounds().y)
															{
																m_pObjects[j].OBJPos();
															}
														}
													}
												}
											}

											//m_pObjects[j].OBJPos();
											for (int l = 0; l < m_num3; l++)
											{
												//m_pStair[l].OBJPos();  //バグる可能性
												if (GameObject* gameObject3 = dynamic_cast<GameObject*>(&m_pObjects[j]))
												{
													if (GameObject* gameObject1 = dynamic_cast<GameObject*>(&m_pStair[l]))
													{
														//float liftposY = lift[a].GetMaxBounds().y;
														//m_pStair[h].Update();
														//ブロックの衝突
														if (m_pObjects[i].IsCollisionPoint(*gameObject1))
														{


															if (gameObject1->GetCMinBounds().y + 0.1 >= gameObject3->GetMaxBounds().y)
															{

																m_pStair[l].OBJPos();
															}
														}
													}
												}
											}

										}
										if (m_pObjects[j].IsAutoMove() == true)
										{
											m_pObjects[j].OBJPos();

											if (m_pObjects[j].MVX() == true)
											{
												m_pObjects[j].MoveXfalse();
											}
											else if (m_pObjects[j].MVX() == false)
											{
												m_pObjects[j].MoveXtrue();
											}

										}
									}

								}
							}

						}
					}
					for (int h = 0; h < m_num3; h++) //四角形と三角形
					{
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
						{
							if (GameObject* gameObject1 = dynamic_cast<GameObject*>(&m_pStair[h]))
							{
								//float liftposY = lift[a].GetMaxBounds().y;
								//m_pStair[h].Update();
								//ブロックの衝突
								if (m_pObjects[i].IsCollisionPoint(*gameObject1))
								{
									if (gameObject1->GetCMinBounds().y + 0.1 >= gameObject->GetMaxBounds().y)
									{
										m_pStair[h].OBJPosy();
										if (m_pObjects[i].IsMove())  //下のブロックのmoveがtrueだった場合連動
										{
											m_pStair[h].Set();
											m_pStair[h].Set1();
										}
									}
									if (gameObject->GetCMinBounds().y + 0.1 >= gameObject1->GetMaxBounds().y)
									{
										m_pObjects[i].MoveObject(gameObject->GetMaxBounds().y + 0.05f);
										if (m_pStair[h].IsMove())  //下のブロックのmoveがtrueだった場合連動
										{
											m_pObjects[i].Set();
											for (int k = 0; k < m_num; k++)
											{
												m_pObjects[k].Set1();
											}
										}
									}
									if (gameObject1->GetCMinBounds().y >= gameObject->GetMaxBounds().y)
									{
										m_pStair[h].OBJPosy();  //すり抜け防止
									/*	if (m_pObjects[i].IsMove())
										{
											m_pStair[h].Set();
											m_pStair[h].Set1();
										}*/
									}
									else if (m_pObjects[i].IsXZ())
									{ //連動時にブロック同士が衝突した場合の乗っているブロックの処理
										for (int k = 0; k < m_num; k++)
										{
											if (k == i)
											{
												k++;
											}
											//m_pObjects[k].OBJPos();  //スタック問題
											for (int l = 0; l < m_num3; l++)
											{
												if (l == h)
												{
													l++;
												}
												if (l > m_num3)
												{
													continue;
												}
												/*if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[k]))
												{
													if (GameObject* gameObject3 = dynamic_cast<GameObject*>(&m_pStair[l]))
													{
														if (m_pObjects[k].IsCollisionPoint(*gameObject3))
														{
															if (gameObject3->GetCMinBounds().y + 0.15 >= gameObject3->GetMaxBounds().y)
															{
																m_pStair[l].OBJPos();
															}
														}
													}

												}*/
												//m_pStair[l].OBJPos(); //スタック問題
											}

										}
										//m_pStair[h].OBJPos();
										for (int k = 0; k < m_num3; k++)
										{
											/*if (h == i || h == j)
											{
												h++;
											}

											for (int l = 0; l < m_num3; l++)
											{
												m_pStair[l].OBJPos();
											}*/
											if (h == k)
											{
												k++;
											}
											//m_pStair[k].OBJPos();

										}
									}
								}

							}
						}
					}
				}
				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pStair[b]))  //三角形、憑依
				{
					//憑依のため・ブロックとプレイヤーが当たった場合
					if (m_pPlayer->IsCollisionPoint(*gameObject))
					{

						if (imanagerO.getKey(2) & 0b011)//(imanagerO.getKey(0) & 0b011)
						{
							// effectこうしん
							float X = m_pPlayer->GetPosX() / EFFECT_PLAYER_MAX_POS_X * EFFECT_MAX_POS_X;

							float Y = m_pPlayer->GetPosY() - 0.4f;

							float Z = (m_pPlayer->GetPosZ() - EFFECT_PLAYER_MIN_POS_Z) /
								(EFFECT_PLAYER_MAX_POS_Z - EFFECT_PLAYER_MIN_POS_Z) *
								EFFECT_MAX_POS_Z + EFFECT_MIN_POS_Z;
							m_EffectHandle = LibEffekseer::GetManager()->Play(m_Effect, X, Y, Z);
							//移動させる時
							Effekseer::Matrix43 EffecMat = LibEffekseer::GetManager()->GetBaseMatrix(m_EffectHandle);
							EffecMat.Translation(0.0f, -1.0f, 0.0f);
							LibEffekseer::GetManager()->SetBaseMatrix(m_EffectHandle, EffecMat);
							LibEffekseer::GetManager()->SetScale(m_EffectHandle, 0.15f, 0.15f, 0.15f);

							m_pPlayer->SetOk();
							m_pPlayer->HPlayerPos();
							m_pStair[b].Set();
							m_pStair[b].Set1();
							m_pStair[b].SetColgravityfalse();
							//m_pObjects[i].Modelchg();
						}
					}
					//憑依解除
					/*if (!m_pPlayer->HIsCollidingWith(*gameObject))
					{*/
					if (imanagerO.getKey(3) & 0b011)//(imanagerO.getKey(1) & 0b011)
					{
						if (m_pStair[b].SetR() == true)
						{
							m_pPlayer->SetNOk();
							m_pPlayer->PlayerPos();
							for (int j = 0; j < m_num3; j++)
							{
								m_pStair[j].SetF();
								/*if (m_pObjects[i].SetR() == true)
								{
									m_pObjects[i].SetF();
								}*/
								m_pStair[j].SetColgravity();
							}
							m_pStair[b].SetF1();
							//m_pStair[b].SetColgravity();
							//m_pObjects[i].Update(); //重力を消してからUpdateを経ずにfalseに入っている
							//m_pObjects[i].Modelchg2();
						}
					}

					//}
				}
				if (m_pStair[b].SetR() == true /*|| m_pObjects[i].SetR() == false*/)  //三角形のmoveがtrue 三角形と四角形
				{
					for (int j = 0; j < m_num; j++)
					{
						/*if (j == i)
						{
							j++;
						}*/
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pStair[b]))
						{
							if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[j]))
							{
								//ブロックの衝突
								if (m_pObjects[j].IsCollisionPoint(*gameObject))
								{
									if (m_pStair[b].IsGravity())
									{
										if (gameObject->GetCMinBounds().y + 0.1 >= gameObject2->GetMaxBounds().y)
										{

											m_pStair[b].SetSlopeY(gameObject2->GetMaxBounds().y + 0.01f);
											if (m_pObjects[j].IsMove() == true)
											{
												m_pStair[b].Set1();
											}
											else
											{
												m_pStair[b].SetF1();
												m_pStair[b].framepls();
											}
										}
										//else if (gameObject->GetMaxBounds().y >= gameObject2->GetCMinBounds().y)
										//{
										//	/*m_pObjects[j].SetObjectTop();
										//	m_pObjects[j].Set();*/
										//	m_pStair[b].SetSlopeY(gameObject2->GetMaxBounds().y);
										//	for (int h = 0; h < m_num; h++)
										//	{
										//		m_pObjects[h].Set1();
										//		m_pStair[h].Set1();
										//	}
										//	if (m_pObjects[j].IsMove() == true)
										//	{
										//		m_pStair[b].Set1();
										//	}
										//	else
										//	{
										//		m_pStair[b].SetF1();
										//		m_pStair[b].framepls();
										//	}
										//}
										else if (m_pStair[b].IsXZ())
										{
											m_pStair[b].OBJPos();
										}
									}
									else if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.1)
									{
										for (int h = 0; h < m_num; h++)
										{
											m_pObjects[h].Set1();
											m_pStair[h].Set1();
										}
										//m_pObjects[j].SetObjectTop();
										m_pObjects[j].Set();
										m_pObjects[j].OBJPosy();

									}
									//else if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.5)
									//{
									//	//for (int h = 0; h < m_num; h++)
									//	//{
									//	//	m_pObjects[h].Set1();
									//	//	m_pStair[h].Set1();
									//	//}
									//	////m_pObjects[j].SetObjectTop();
									//	//m_pObjects[j].Set();
									//	//m_pObjects[j].OBJPosy();
									//	m_pStair[b].OBJPos();
									//}
									else if (m_pObjects[j].IsXZ() && m_pObjects[j].IsAutoMove() == true)
									{
										m_pObjects[j].OBJPos();
										m_pStair[b].OBJPos();
										if (m_pObjects[j].MVX() == true)
										{
											m_pObjects[j].MoveXfalse();
										}
										else if (m_pObjects[j].MVX() == false)
										{
											m_pObjects[j].MoveXtrue();
										}
									}
									else if (m_pStair[b].IsXZ())
									{
										for (int l = 0; l < m_num3; l++)
										{
											m_pStair[l].OBJPos();
										}
										for (int l = 0; l < m_num; l++)
										{
											m_pObjects[l].OBJPos();
										}
										//m_pObjects[j].OBJPos();
										/*for (int k = 0; k < m_num3; k++)
										{
											if (b == k)
											{
												k++;
											}
											if (GameObject* gameObject3 = dynamic_cast<GameObject*>(&m_pStair[k]))
											{
												if (m_pObjects[j].IsCollisionPoint(*gameObject2))
												{
													if (gameObject2->GetMaxBounds().y <= gameObject3->GetCMinBounds().y + 0.1)
													{
														m_pStair[k].OBJPos();
														m_pObjects[j].OBJPos();
													}
												}


											}
										}*/
										//m_pObjects[j].SetF();
									}
								}
								//else
								//{
								//	if (m_pStair[b].IsStairTop() == true)
								//	{
								//		//m_pObjects[j].OBJPos();
								//		m_pStair[b].SetStairTop();
								//		//m_pObjects[j].SetF();
								//		m_pObjects[j].OBJPos();
								//	}
								//}


							}
						}


					}
					for (int j = 0; j < m_num3; j++)//三角形のmoveがtrue 三角形と三角形
					{
						if (j == b)
						{
							j++;
						}
						if (j >= m_num3)
						{
							continue;
						}
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pStair[b]))
						{
							if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pStair[j]))
							{
								//ブロックの衝突
								if (m_pStair[b].IsCollisionPoint(*gameObject2))
								{
									if (m_pStair[b].IsGravity())
									{
										if (gameObject->GetCMinBounds().y + 0.05 >= gameObject2->GetMaxBounds().y)
										{

											//m_pStair[b].OBJPosy();
											m_pStair[b].SetSlopeY(gameObject2->GetMaxBounds().y + 0.05f);
											if (m_pStair[b].IsMove() == true)
											{
												m_pStair[i].Set1();
											}
											else
											{
												m_pStair[b].SetF1();
												m_pStair[b].framepls();
											}
										}
										//else if (gameObject->GetMaxBounds().y >= gameObject2->GetCMinBounds().y)
										//{
										//	/*m_pObjects[j].SetObjectTop();
										//	m_pObjects[j].Set();*/
										//	m_pStair[b].OBJPosy();
										//	for (int h = 0; h < m_num; h++)
										//	{
										//		m_pObjects[h].Set1();
										//		m_pStair[h].Set1();
										//	}
										//}
										//else if (gameObject->GetMaxBounds().y  <= gameObject2->GetCMinBounds().y )
										//{
										//	/*m_pObjects[j].SetObjectTop();
										//	m_pObjects[j].Set();*/
										//	m_pStair[b].OBJPosy();
										//	m_pStair[j].OBJPosy();
										//	for (int h = 0; h < m_num; h++)
										//	{
										//		m_pObjects[h].Set1();
										//		//m_pStair[h].Set1();
										//	}
										//}
										else if (m_pStair[b].GetPos().y < gameObject2->GetMaxBounds().y && m_pStair[b].GetPos().y > gameObject2->GetCMinBounds().y)
										{
											if (m_pStair[b].IsXZ())
											{
												m_pStair[b].OBJPos();
											}
										}
										else if (m_pStair[b].GetPos().y > m_pStair[j].GetPos().y)
										{
											if (m_pStair[j].IsReverse() == false)
											{
												m_pStair[b].SetSlope();
											}
											else if (m_pStair[j].IsReverse() == true)
											{
												m_pStair[b].SetReverseSlope();
											}

										}
										/*else
										{
											m_pStair[b].SetSlope();


										}*/
									}
									else if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.1)
									{
										//for (int h = 0; h < m_num3; h++)
										//{
										//	//m_pObjects[h].Set1();
										//	m_pStair[h].Set1();
										//}
										////m_pObjects[j].SetObjectTop();
										//m_pStair[j].Set();
										m_pStair[j].OBJPosy();
										m_pStair[b].OBJPosy();
										//m_pStair[b].SetSlopeY(gameObject->GetMaxBounds().y + 0.01);
									}

									else if (m_pStair[b].IsXZ())
									{
										for (int l = 0; l < m_num3; l++)
										{
											m_pStair[l].OBJPos();
										}
										for (int l = 0; l < m_num; l++)
										{
											m_pObjects[l].OBJPos();
										}
										//m_pObjects[j].OBJPos();
										/*for (int k = 0; k < m_num3; k++)
										{
											if (b == k)
											{
												k++;
											}
											if (GameObject* gameObject3 = dynamic_cast<GameObject*>(&m_pStair[k]))
											{
												if (m_pObjects[j].IsCollisionPoint(*gameObject2))
												{
													if (gameObject2->GetMaxBounds().y <= gameObject3->GetCMinBounds().y + 0.1)
													{
														m_pStair[k].OBJPos();
														m_pObjects[j].OBJPos();
													}
												}


											}
										}*/
										//m_pObjects[j].SetF();
									}
									else
									{
										if (m_pStair[j].IsReverse() == false)
										{
											m_pStair[b].SetSlope();
										}
										else if (m_pStair[j].IsReverse() == true)
										{
											m_pStair[b].SetReverseSlope();
										}

									}
								}
								//else
								//{
								//	if (m_pStair[b].IsStairTop() == true)
								//	{
								//		//m_pObjects[j].OBJPos();
								//		m_pStair[b].SetStairTop();
								//		//m_pObjects[j].SetF();
								//		m_pObjects[j].OBJPos();
								//	}
								//}


							}
						}


					}
				}

				if (m_pStair[b].SetR() == false) //三角形のmoveがfalse 三角形と四角形
				{
					for (int j = 0; j < m_num; j++)
					{
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pStair[b]))
						{
							if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[j]))
							{

								// ブロックiとブロックjの当たり判定
								if (m_pObjects[j].IsCollisionPoint(*gameObject)) /*&& m_pObjects[j].col(*gameObject)*/
								{
									if (gameObject->GetCMinBounds().y + 0.05 >= gameObject2->GetMaxBounds().y)
									{

										m_pStair[b].SetSlopeY(gameObject2->GetMaxBounds().y + 0.05f);

									}
									if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y /*+ 0.05f*/)
									{
										m_pObjects[j].OBJPosy();
									}
									if (m_pObjects[j].IsXZ())
									{
										for (int h = 0; h < m_num3; h++)  //コメント
										{
											//m_pStair[h].OBJPos();
										}

										for (int h = 0; h < m_num; h++)
										{
											//if (h == j)
											//{
											//	h++;
											//}
											//if (GameObject* gameObject3 = dynamic_cast<GameObject*>(&m_pObjects[h]))
											//{
											//	if (gameObject2->GetMaxBounds().y <= gameObject->GetCMinBounds().y + 0.1)
											//	{
											//		//m_pStair[i].OBJPos();
											//		m_pStair[b].OBJPos();
											//		m_pObjects[j].OBJPos();
											//		m_pObjects[j].SetObjectTop();
											//		m_pObjects[j].SetF1();
											//	}
											//}
										}
									}
									if (m_pObjects[j].IsAutoMove() == true)
									{
										m_pObjects[j].OBJPos();

										if (m_pObjects[j].MVX() == true)
										{
											m_pObjects[j].MoveXfalse();
										}
										else if (m_pObjects[j].MVX() == false)
										{
											m_pObjects[j].MoveXtrue();
										}

									}
								}
							}
						}
					}
					for (int h = 0; h < m_num; h++)
					{
						if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[h]))
						{
							if (GameObject* gameObject1 = dynamic_cast<GameObject*>(&m_pStair[b]))
							{
								//float liftposY = lift[a].GetMaxBounds().y;
								//m_pStair[h].Update();
								//ブロックの衝突
								if (m_pObjects[h].IsCollisionPoint(*gameObject1))
								{
									if (gameObject1->GetCMinBounds().y + 0.15 >= gameObject->GetMaxBounds().y)
									{
										m_pStair[b].OBJPosy();
										//m_pStair[b].SetSlopeY(gameObject->GetMaxBounds().y + 0.01);
										if (m_pObjects[h].IsMove())
										{
											m_pStair[b].Set();
											m_pStair[b].Set1();

										}
									}
									if (gameObject->GetCMinBounds().y + 0.15 >= gameObject1->GetMaxBounds().y)
									{
										m_pObjects[h].OBJPosy();
										//m_pStair[h].SetSlopeY(gameObject1->GetMaxBounds().y + 0.01);
										if (m_pStair[b].IsMove())
										{
											m_pObjects[h].Set();
											for (int k = 0; k < m_num; k++)
											{
												m_pObjects[k].Set1();
											}
										}
									}
								}

							}
						}
					}
					for (int h = 0; h < m_num3; h++) //三角形のmoveがfalse 三角形と四角形
					{
						if (h == b)
						{
							h++;
						}
						if (h >= m_num3)
						{
							break;
						}
						else if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pStair[b]))
						{
							if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pStair[h]))
							{
								//ブロックの衝突
								if (m_pStair[b].IsCollisionPoint(*gameObject2))
								{
									if (gameObject->GetCMinBounds().y + 0.05 >= gameObject2->GetMaxBounds().y)
									{

										//m_pStair[b].OBJPosy();
										m_pStair[b].SetSlopeY(gameObject2->GetMaxBounds().y + 0.05f);
										if (m_pStair[h].IsMove())
										{
											m_pStair[b].Set();
											m_pStair[b].Set1();
											m_pStair[h].Set1();
										}
									}
									else if (gameObject->GetCMinBounds().y + 0.07 >= gameObject2->GetMaxBounds().y)
									{

										//m_pStair[b].OBJPosy();
										m_pStair[b].SetSlopeY(gameObject2->GetMaxBounds().y + 0.07f);
										/*	if (m_pStair[h].IsMove())
											{
												m_pStair[b].Set();
												m_pStair[b].Set1();
												m_pStair[h].Set1();
											}*/
									}
									else if (gameObject->GetMaxBounds().y >= gameObject2->GetCMinBounds().y)
									{
										m_pStair[b].OBJPosy();
										//m_pStair[h].SetSlopeY(gameObject->GetMaxBounds().y + 0.01);
										/*if (m_pStair[h].IsMove())
										{
											m_pStair[b].Set();
											m_pStair[b].Set1();
										}*/
									}

									else if (m_pStair[h].IsXZ())
									{
										for (int k = 0; k < m_num3; k++)
										{
											if (h == k)
											{
												continue;
											}
											m_pStair[k].OBJPos();
										}
									}
								}
							}
						}
					}

				}
			}
		}
	}

}
void ObjectMng::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix, bool draw)
{
	//行列の計算
	DirectX::XMFLOAT4X4 effectmat[2];

	effectmat[0] = m_pObjectCamera->GetViewMatrix();
	effectmat[1] = m_pObjectCamera->GetProjectionMatrix();

	//effeksserに送る前に転置する前の状態に戻す
	DirectX::XMMATRIX effekMat = XMLoadFloat4x4(&effectmat[0]);
	effekMat = XMMatrixTranspose(effekMat);
	XMStoreFloat4x4(&effectmat[0], effekMat);

	DirectX::XMMATRIX effekMat2 = XMLoadFloat4x4(&effectmat[1]);
	effekMat2 = XMMatrixTranspose(effekMat2);
	XMStoreFloat4x4(&effectmat[1], effekMat2);

	LibEffekseer::GetManager()->SetScale(m_EffectHandle, 0.1f, 0.1f, 0.1f);
	LibEffekseer::SetViewPosition(m_pObjectCamera->GetPos());
	LibEffekseer::SetCameraMatrix(effectmat[0], effectmat[1]);

	// ブロック描画
	for (int i = 0; i < m_num; i++)
	{
		m_pObjects[i].Draw(viewMatrix, projectionMatrix);

	}
	// リフト描画
	for (int i = 0; i < m_num1; i++)
	{
		m_pLift_obj[i].Draw(viewMatrix, projectionMatrix);
	}
	if (draw == true)
	{
		for (int i = 0; i < m_num2; i++)
		{
			m_pLever[i].Draw(viewMatrix, projectionMatrix);
		}
	}
	// ゴール描画
	for (int i = 0; i < m_num3; i++)
	{
		m_pStair[i].Draw(viewMatrix, projectionMatrix);
	}
	// 床描画
	for (int i = 0; i < m_num4; i++)
	{
		m_pYuka[i].Draw(viewMatrix, projectionMatrix);
	}

//留め具
		for (int i = 0; i < m_num5; i++)
		{
			m_pTomegu[i].Draw(viewMatrix, projectionMatrix);
		}

//マグネット

		if (draw == true)
		{
			for (int i = 0; i < m_num6; i++)
			{
				m_pMagnet[i].Draw(viewMatrix, projectionMatrix);
			}
		}

	//// 動くブロック描画
	//for (int i = 0; i < m_num6; i++)
	//{
	//	m_pObjectsAuto[i].Draw(viewMatrix, projectionMatrix);
	//}

	DirectX::XMFLOAT4X4 mat[3];

	mat[1] = m_pObjectCamera->GetViewMatrix();
	mat[2] = m_pObjectCamera->GetProjectionMatrix();

	if (draw == true)
	{
		m_pPlayer->Draw(viewMatrix, projectionMatrix);
	}


}





