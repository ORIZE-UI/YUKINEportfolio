#include "SceneManager.hpp"
#include "TitleScene.h"
#include "SceneTutorial.h"
#include "SelectScene.h"
#include "SceneGame.h"
#include "SceneResult.h"
#include "SceneLoad.h"

SceneManager::SceneManager()
	: m_Nowscene(SCENE_MAX)
	, m_NextScene(SCENE_START)
	, m_pSceneTitle(nullptr)
	, m_pSceneTutorial(nullptr)
	, m_pSceneSelect(nullptr)
	, m_pSceneGame(nullptr)
	, m_pSceneResult(nullptr)
	, m_pSceneLoad(nullptr)
	, m_pFade(nullptr)
	, m_pCurtainUI(nullptr)
{
	m_pCurtainUI = new CurtainUI();
	m_pFade = new Fade(m_pCurtainUI);
}

SceneManager::~SceneManager()
{
	switch (m_Nowscene)
	{
	case SCENE_TITLE:	 delete m_pSceneGame;		break;
	case SCENE_TUTORIAL: delete m_pSceneTutorial;	break;
	case SCENE_SELECT:	 delete m_pSceneSelect;		break;
	case SCENE_GAME:	 delete m_pSceneGame;		break;
	case SCENE_RESULT:	 delete m_pSceneResult;		break;
	case SCENE_LOAD:	 delete m_pSceneLoad;		break;
	default: break;
	}
	delete m_pFade;
	delete m_pCurtainUI;
}

void SceneManager::Update(float tick)
{
	if (m_pSceneSelect)
	{
		m_SelectScene = m_pSceneSelect->GetSelectStage();
	}
	if (!m_pFade->IsPlay())
	{
		// シーンの切り替え判定
		if (m_Nowscene != m_NextScene)
		{
			// 現在のシーンを削除
			switch (m_Nowscene)
			{
			case SCENE_TITLE:	 delete m_pSceneTitle; m_pSceneTitle = nullptr;			break;
			case SCENE_TUTORIAL: delete m_pSceneTutorial; m_pSceneTutorial = nullptr;	break;				break;
			case SCENE_SELECT:	 delete m_pSceneSelect; m_pSceneSelect = nullptr;		break;
			case SCENE_GAME:	 delete m_pSceneGame; m_pSceneGame = nullptr;			break;
			case SCENE_RESULT:	 delete m_pSceneResult; m_pSceneResult = nullptr;		break;
			case SCENE_LOAD:	 delete m_pSceneLoad; m_pSceneLoad = nullptr;			break;
			default: break;
			}
			// 新しいシーンの読込
			switch (m_NextScene)
			{
			case SCENE_TITLE:	 m_pSceneTitle = new SceneTitle();			break;
			case SCENE_TUTORIAL: m_pSceneTutorial = new SceneTutorial();	break;
			case SCENE_SELECT:	 m_pSceneSelect = new SelectScene();		break;
			case SCENE_GAME:	 m_pSceneGame = new SceneGame(m_SelectScene);			break;
			case SCENE_RESULT:	 m_pSceneResult = new SceneResult();		break;
			case SCENE_LOAD:	 m_pSceneLoad = new SceneLoad();			break;
			default: break;
			}
			// 現在のシーンを新しいシーンへ上書き
			m_Nowscene = m_NextScene;
			m_pFade->Start(true, 1.0f);	// フェードイン
		}
		else
		{
			// フェードの明るさにあわせて、スピーカーのボリュームを調節
			//m_pSpeaker->SetVolume(1.0f - m_pFade->GetAlpha());
		}
	}
	switch (m_Nowscene)
	{
	case SCENE_TITLE:	 m_pSceneTitle->Update(this);		break;
	case SCENE_TUTORIAL: m_pSceneTutorial->Update(this);	break;
	case SCENE_SELECT:	 m_pSceneSelect->Update(this);		break;
	case SCENE_GAME:	 m_pSceneGame->Update(this, tick);	break;
	case SCENE_RESULT:   m_pSceneResult->Update(this);		break;
	case SCENE_LOAD:	 m_pSceneLoad->Update(this);		break;
	default: break;
	}
	// フェードの更新処理
	m_pFade->Update();
	m_pCurtainUI->Update();
}

void SceneManager::Draw()
{
	// =m_pSceneSelect->GetPosition();

	switch (m_Nowscene)
	{
	case SCENE_TITLE:	 m_pSceneTitle->Draw();		break;
	case SCENE_TUTORIAL: m_pSceneTutorial->Draw();	break;
	case SCENE_SELECT:	 m_pSceneSelect->Draw(), m_pSceneSelect->BackUIDraw(), m_pSceneSelect->MenuUIDraw(), m_pSceneSelect->StartUIDraw()
						,m_pSceneSelect->CursorUIDraw(m_pSceneSelect->GetPosition().x, m_pSceneSelect->GetPosition().y, m_pSceneSelect->GetPosition().z)
						, m_pSceneSelect->Stage1Draw(), m_pSceneSelect->Stage2Draw(), m_pSceneSelect->Stage3Draw(), m_pSceneSelect->Stage4Draw(), m_pSceneSelect->Stage5Draw();	break;
	case SCENE_GAME:	 m_pSceneGame->Draw();		break;
	case SCENE_RESULT:   m_pSceneResult->BGDraw(), m_pSceneResult->ClearDraw(), m_pSceneResult->NextDraw();	break;
	case SCENE_LOAD:	 m_pSceneLoad->Draw();		break;
	default: break;
	}
	// 一番最後に画面全体に表示する
	m_pFade->Draw();
	m_pCurtainUI->StageCurtainDraw();
}

void SceneManager::SetNextScene(SceneKind scene)
{
	if (m_Nowscene == SCENE_GAME)
	{
		m_pFade->Start(false, 6.0f);
	}
	else
	{
		m_pFade->Start(false, 1.0f);	// フェードアウト
	}
	// フェード中は次のシーンを予約しない
	if (m_pFade->IsPlay())
	{
		m_NextScene = scene;
		return;
	}
}
