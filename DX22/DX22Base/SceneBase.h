#ifndef __SCENE_BASE_H__
#define __SCENE_BASE_H__

class SceneBase
{
public:
	virtual ~SceneBase(void) {}

	virtual void	Update(float tick) = 0;
	virtual void	Draw(void) = 0;
};

#endif