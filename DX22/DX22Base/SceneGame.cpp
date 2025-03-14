#include "SceneGame.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include "cameraDebug.h"
#include "cameraPlayer.h"
#include "Input.h"
#include "Sprite.h"
#include "Main.h"
#include "SceneResult.h"

// レースの最大ラップ数
const int SceneGame::MAX_RAP = 3;

// シーンゲームのコンストラクタ
SceneGame::SceneGame()
	: m_bLastHitGoal(true)  // 最後にゴールにヒットしたかのフラグ
{
	// プレイヤーの生成とカメラの設定
	m_pPlayer = new Player();

	//CPU生成
	m_pCPUCar = new CPUCar();

	// メインカメラの種類をプレイヤーカメラに設定
	m_mainCamera = CAM_PLAYER;
	m_apCamera[CAM_PLAYER] = new CameraPlayer(m_pPlayer); // プレイヤー視点のカメラ
	m_apCamera[CAM_DEBUG] = new CameraDebug();            // デバッグ用のカメラ

	m_pPlayer->SetCamera(m_apCamera[CAM_PLAYER]);  // プレイヤーにプレイヤーカメラを設定
	
	m_pCPUCar->SetCamera(m_apCamera[CAM_PLAYER]);
	m_pCPUCar->AddControlPoint({ 22.0f,0.0f,22.0f });
	m_pCPUCar->AddControlPoint({ 0.0f,0.0f,26.0f });
	m_pCPUCar->AddControlPoint({ -22.0f,0.0f,22.0f });
	m_pCPUCar->AddControlPoint({ -26.0f, 0.0f, 0.0f });
	m_pCPUCar->AddControlPoint({ -22.0f, 0.0f, -22.0f });
	m_pCPUCar->AddControlPoint({ 0.0f, 0.0f, -26.0f });
	m_pCPUCar->AddControlPoint({ 22.0f, 0.0f, -22.0f });
	m_pCPUCar->AddControlPoint({ 26.0f, 0.0f, 0.0f });

	// ゲーム内の各オブジェクトの初期化
	m_pItemUI = new ItemUI();

	m_pField = new Field();
	m_pField->SetCamera(m_apCamera[CAM_PLAYER]);   // フィールドにプレイヤーカメラを設定

	m_pRoad = new Road();
	m_pRoad->SetCamera(m_apCamera[CAM_PLAYER]);    // 道にもプレイヤーカメラを設定

	m_pFade = new Fade();
	m_pFade->StartFade(Fade::FadeKind::FadeIn);    // フェードインを開始

	// ラップ数の表示用のオブジェクト
	m_pRapNumber = new Number();
	m_pRapNumber->SetSize({ 80.0f, 80.0f });
	m_pRapNumber->SetPos({ 25.0f, 50.0f, 0.0f });

	m_pMaxRapNumber = new Number();
	m_pMaxRapNumber->SetSize({ 80.0f, 80.0f });
	m_pMaxRapNumber->SetPos({ 125.0f, 50.0f, 0.0f });
	m_pMaxRapNumber->SetNumber(MAX_RAP);           // 最大ラップ数を設定

	// タイマー表示用のオブジェクト
	m_pTimerNumber = new Number();
	m_pTimerNumber->SetDigit(6);
	m_pTimerNumber->SetSize({ 150.0f, 50.0f });
	m_pTimerNumber->SetPos({ 25.0f, 110.0f, 0.0f });

	m_pSignal = new Signal();   // スタート信号オブジェクトの生成
	m_pMeter = new Meter(m_pPlayer);  // スピードメーターオブジェクトを生成

	// ゴール地点を示すポールの生成とカメラ設定
	m_pGoalPoll = new GoalPoll();
	m_pGoalPoll->SetCamera(m_apCamera[CAM_PLAYER]);

	// スカイドームの生成とカメラ設定
	m_pSkydome = new Skydome();
	m_pSkydome->SetCamera(m_apCamera[CAM_PLAYER]);

	// スピードアイテムの生成とカメラ設定
	m_pSpeedItem = new CSpeedItem();
	m_pSpeedItem->SetCamera(m_apCamera[CAM_PLAYER]);

	// スピードアイテムの生成位置（4つのグループで分けられている）
	m_pSpeedItem->AddSpawnGroup(
		{
			{0.0f,0.0f,26.0f},
			{0.0f,0.0f,26.0f + 3.0f},
			{0.0f,0.0f,26.0f - 3.0f}
		}
	);
	m_pSpeedItem->AddSpawnGroup(
		{
			{0.0f,0.0f,-26.0f},
			{0.0f,0.0f,-26.0f + 3.0f},
			{0.0f,0.0f,-26.0f - 3.0f},
		}
	);
	m_pSpeedItem->AddSpawnGroup(
		{
			{26.0f, 0.0f, 0.0f},
			{26.0f + 3.0f, 0.0f, 0.0f},
			{26.0f - 3.0f, 0.0f, 0.0f}
		}
	);
	m_pSpeedItem->AddSpawnGroup(
		{
			{-26.0f, 0.0f, 0.0f},
			{-26.0f + 3.0f, 0.0f, 0.0f},
			{-26.0f - 3.0f, 0.0f, 0.0f}
		}
	);
}

// シーンゲームのデストラクタ
SceneGame::~SceneGame()
{
	// 各オブジェクトの破棄処理
	if (m_pMeter) { delete m_pMeter; m_pMeter = nullptr; }
	if (m_pGoalPoll) { delete m_pGoalPoll; m_pGoalPoll = nullptr; }
	if (m_pSignal) { delete m_pSignal; m_pSignal = nullptr; }
	if (m_pSpeedItem) { delete m_pSpeedItem; m_pSpeedItem = nullptr; }
	if (m_pTimerNumber) { delete m_pTimerNumber; m_pTimerNumber = nullptr; }
	if (m_pMaxRapNumber) { delete m_pMaxRapNumber; m_pMaxRapNumber = nullptr; }
	if (m_pRapNumber) { delete m_pRapNumber; m_pRapNumber = nullptr; }
	if (m_pFade) { delete m_pFade; m_pFade = nullptr; }
	if (m_pRoad) { delete m_pRoad; m_pRoad = nullptr; }

	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		delete m_apCamera[nCnt];
		m_apCamera[nCnt] = nullptr;
	}

	if (m_pPlayer) { delete m_pPlayer; m_pPlayer = nullptr; }
	if (m_pCPUCar) { delete m_pCPUCar; m_pCPUCar = nullptr; }
	if (m_pItemUI) { delete m_pItemUI; m_pItemUI = nullptr; }
	if (m_pField) { delete m_pField; m_pField = nullptr; }
	if (m_pSkydome) { delete m_pSkydome; m_pSkydome = nullptr; }
}

// 毎フレーム呼び出されるアップデート関数
void SceneGame::Update(float tick)
{
	// カメラ切り替え操作
	if (IsKeyPress('K'))
	{
		if (IsKeyPress('1')) { m_mainCamera = CAM_DEBUG; }
		if (IsKeyPress('2')) { m_mainCamera = CAM_PLAYER; }

		// カメラの変更を反映
		m_pSkydome->SetCamera(m_apCamera[m_mainCamera]);
		m_pPlayer->SetCamera(m_apCamera[m_mainCamera]);
		m_pRoad->SetCamera(m_apCamera[m_mainCamera]);
		m_pSpeedItem->SetCamera(m_apCamera[m_mainCamera]);
		m_pField->SetCamera(m_apCamera[m_mainCamera]);
		m_pGoalPoll->SetCamera(m_apCamera[m_mainCamera]);
	}

	// 各オブジェクトの更新処理
	if (m_mainCamera == CAM_PLAYER)
	{
		m_pPlayer->Update();
		m_pCPUCar->Update();
		m_pRoad->Update();
		m_pSpeedItem->Update();
		m_pMeter->Update();
	}

	m_apCamera[m_mainCamera]->Update();  // カメラの更新

	{
		// 地面と道の当たり判定処理
		Collision::Ray ray;
		DirectX::XMFLOAT3 playerPos = m_pPlayer->GetPos();
		ray.start = { playerPos.x, playerPos.y + 1.0f, playerPos.z };
		ray.direction = { 0.0f, -2.0f, 0.0f };

		for (int i = 0; i < m_pField->GetDataNum(); ++i)
		{
			Collision::Result collision = Collision::CheckRayPlane(ray, m_pField->GetPlaneInfo(i));
			if (collision.hit)
			{
				m_pPlayer->SetPos(collision.point);
			}
		}

		// 道の当たり判定
		Road::tCollisionResult roadCollision = m_pRoad->CheckCollision(ray);
		if (roadCollision.collisionResult.hit)
		{
			OutputDebugString(("polyNo" + std::to_string(roadCollision.polyNo) + "\n").c_str());

			// 一周判定
			DirectX::XMVECTOR vGoalPos = DirectX::XMVectorSet(23.0f, 0.0f, 0.0f, 0.0f);
			DirectX::XMVECTOR vPlayerPos = DirectX::XMVectorSet(
				roadCollision.collisionResult.point.x,
				roadCollision.collisionResult.point.y,
				roadCollision.collisionResult.point.z,
				0.0f
			);
			DirectX::XMVECTOR vRelative = DirectX::XMVector3Length(
				DirectX::XMVectorSubtract(vGoalPos, vPlayerPos)
			);
			float fRelative;
			DirectX::XMStoreFloat(&fRelative, vRelative);
			if (fRelative < 6.0f)
			{
				if (m_bLastHitGoal == false)
				{
					m_pPlayer->AddRap();  // 一周追加
				}

				m_bLastHitGoal = true;
			}
			else
			{
				m_bLastHitGoal = false;
			}

			m_pPlayer->SetLastHitRoadPoly(roadCollision.polyNo);

			if (m_pPlayer->GetAccelKind() != Player::AccelKind::SpeedUpItem)
			{
				m_pPlayer->SetAccelKind(Player::AccelKind::Road);
			}
		}
		else
		{
			m_pPlayer->SetAccelKind(Player::AccelKind::OutSide);
		}

		// スピードアップアイテムの当たり判定
		Collision::Result speedUpResult = m_pSpeedItem->CheckCollision(m_pPlayer->GetPos());
		if (speedUpResult.hit)
		{
			m_pPlayer->SetAccelKind(Player::AccelKind::SpeedUpItem);
		}
	}

	// 表示するラップ数とタイマーの設定
	m_pRapNumber->SetNumber(m_pPlayer->GetRap());
	m_pTimerNumber->SetNumber(m_pPlayer->GetTimerFrame());

	// 各オブジェクトのフェードおよびシグナルの更新
	m_pFade->Update();
	m_pSignal->Update();

	// フェードイン完了後のカウントダウン
	if (m_pFade->GetKind() == Fade::FadeKind::FadeIn &&
		m_pFade->IsFinish() &&
		m_pPlayer->IsTimerActive() == false)
	{
		if (m_pSignal->IsSignalEnd())
		{
			m_pPlayer->TimerStart();  // タイマー開始
		}
		else
		{
			m_pSignal->CountDownStart();  // カウントダウン開始
		}
	}

	// 最大ラップ数に達した場合の処理
	if (m_pPlayer->GetRap() > MAX_RAP - 1 && m_pFade->GetKind() != Fade::FadeKind::FadeOut)
	{
		m_pFade->StartFade(Fade::FadeKind::FadeOut);  // フェードアウト開始
		m_pPlayer->TimerStop();
		//SceneResult::SetResultTime(m_pPlayer->GetTimerFrame());
	}

	// フェードアウト完了後のシーン遷移
	if (m_pFade->GetKind() == Fade::FadeKind::FadeOut && m_pFade->IsFinish())
	{
		GetSceneMng()->SetScene(SceneManager::CSceneKind::Result);  // 結果シーンに遷移
	}
}

// シーンゲームの描画処理
void SceneGame::Draw()
{
	// ビュー行列とプロジェクション行列の設定
	DirectX::XMFLOAT4X4 mat[3];
	mat[1] = m_apCamera[m_mainCamera]->GetViewMatrix();
	mat[2] = m_apCamera[m_mainCamera]->GetProjectionMatrix();

	Geometory::SetView(mat[1]);
	Geometory::SetProjection(mat[2]);

	// 各オブジェクトの描画
	m_pSkydome->Draw();
	m_pPlayer->Draw();
	m_pCPUCar->Draw();
	m_pField->Draw();
	m_pRoad->Draw();
	m_pGoalPoll->Draw();
	m_pSpeedItem->Draw();
	m_pItemUI->Draw();
	m_pMeter->Draw();
	m_pRapNumber->Draw();
	m_pMaxRapNumber->Draw();
	m_pTimerNumber->Draw();
	m_pSignal->Draw();
	m_pFade->Draw();
}