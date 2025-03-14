#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "SceneBase.h"

class SceneManager
{
public:
	enum class CSceneKind
	{
		None,
		Title,
		Game,
		Result,
	};

	SceneManager(void);
	~SceneManager(void);

	void	Update(float);
	void	Draw(void);

	void	SetScene(CSceneKind);
private:
	CSceneKind	m_nextScene;
	SceneBase*	m_pBaseScene;
};

#endif