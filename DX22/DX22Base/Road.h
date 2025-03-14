#ifndef __ROAD_H__
#define __ROAD_H__

#include "cameraBase.h"
#include "player.h"
#include "RoadEffect.h"
#include "Collision.h"
#include "CPUCar.h"

class Road
{
public:
	typedef struct
	{
		Collision::Result	collisionResult;
		int					polyNo;
	}tCollisionResult;

	Road(void);
	~Road(void);

	void Update(void);
	void Draw(void);
	void SetCamera(CameraBase*);
	void SetControlPointsToCar(CPUCar*);
	tCollisionResult	CheckCollision(Collision::Ray);

private:
	CameraBase*	m_pCamera;
	RoadEffect*	m_pRoadEffect;
	Texture*	m_pRoadTexture;
	CPUCar*		m_pCPUCar;
};

#endif