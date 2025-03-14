#include "sceneManager.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "SceneResult.h"

// コンストラクタ
SceneManager::SceneManager(void)
	: m_nextScene(CSceneKind::None)
	, m_pBaseScene(nullptr)
{

}

// デストラクタ
SceneManager::~SceneManager(void)
{
	if (m_pBaseScene)
	{
		delete m_pBaseScene;
		m_pBaseScene = nullptr;
	}
}

// 更新
void SceneManager::Update(float tick)
{
	// シーン変更
	if (m_nextScene != CSceneKind::None)
	{
		// 前のシーンを終了
		if (m_pBaseScene)
		{
			delete m_pBaseScene;
			m_pBaseScene = nullptr;
		}

		// 新シーンをロード
		switch (m_nextScene)
		{
		case SceneManager::CSceneKind::Title:
			m_pBaseScene = new SceneTitle();
			break;
		case SceneManager::CSceneKind::Game:
			m_pBaseScene = new SceneGame();
			break;
		case SceneManager::CSceneKind::Result:
			m_pBaseScene = new SceneResult();
			break;
		default:
			break;
		}

		// シーン読込フラグを戻す
		m_nextScene = CSceneKind::None;
	}

	// 更新
	if (m_pBaseScene)
	{
		m_pBaseScene->Update(tick);
	}
}

void SceneManager::Draw(void)
{
	// 描画
	if (m_pBaseScene)
	{
		m_pBaseScene->Draw();
	}
}

// シーンを設定
void SceneManager::SetScene(CSceneKind kind)
{
	m_nextScene = kind;
}