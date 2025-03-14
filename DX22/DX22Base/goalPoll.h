#ifndef __GOAL_POLL_H__
#define __GOAL_POLL_H__

#include "Model.h"
#include "cameraBase.h"
#include "Shader.h"

class GoalPoll
{
public:
	GoalPoll();
	~GoalPoll();

	void	Draw(void);

	void	SetCamera(CameraBase*);
private:
	CameraBase*		m_pCamera;
	Model*			m_pModel;
	VertexShader*	m_pVS;

};

#endif