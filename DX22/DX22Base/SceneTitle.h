#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

#include "SceneBase.h"
#include "Texture.h"
#include "Fade.h"

class SceneTitle : public SceneBase
{
public:
	SceneTitle(void);
	~SceneTitle(void);

	void	Update(float tick)	override;
	void	Draw(void)	override;
private:
	Texture*	m_pTitleTexture;
	Fade*		m_pFade;
};

#endif