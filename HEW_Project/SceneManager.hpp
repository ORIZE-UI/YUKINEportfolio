#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Fade.hpp"
#include "Curtain.h"

// 相互インクルードが発生した場合やむを得ない対応
class SceneTitle;
class SceneTutorial;
class SelectScene;
class SceneGame;
class SceneResult;
class SceneLoad;
class SelectScene;

enum SceneKind
{
	SCENE_TITLE = 0,
	SCENE_TUTORIAL,
	SCENE_SELECT,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_LOAD,
	SCENE_MAX,

	SCENE_START = SCENE_TITLE,
};

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Update(float tick);
	void Draw();

	void SetNextScene(SceneKind scene);
private:
	int		m_Nowscene; // 現在実行中のシーン
	int		m_NextScene; // 次のシーン
	int		m_SelectScene; // 選択されたシーン

	Fade*		m_pFade;
	CurtainUI*	m_pCurtainUI;

	SceneTitle*		m_pSceneTitle;
	SceneTutorial*	m_pSceneTutorial;
	SelectScene*	m_pSceneSelect;
	SceneGame*		m_pSceneGame;
	SceneResult*	m_pSceneResult;
	SceneLoad*		m_pSceneLoad;
};

#endif // !__SCENE_MANAGER_H__
