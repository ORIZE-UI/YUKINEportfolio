#ifndef __SCENE_RESULT_H__
#define __SCENE_RESULT_H__


#include "SceneBase.h"
#include "Texture.h"
#include "Fade.h"
//#include "number.h"

class SceneResult : public SceneBase
{
public:
	SceneResult(void);
	~SceneResult(void);

	void	Update(float tick)	override;
	void	Draw(void)	override;

	//static void	SetResultTime(int);

private:
	Texture*	m_pResultTexture;
	Fade*		m_pFade;
	//static int	ms_resultTime;
	//Number*		m_pResultTimeNumber;

};


#endif