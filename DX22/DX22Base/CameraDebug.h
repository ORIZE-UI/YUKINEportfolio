#ifndef __CAMERA_DEBUG_H__
#define __CAMERA_DEBUG_H__

#include "cameraBase.h"

class CameraDebug : public CameraBase
{
public:
	CameraDebug(void);

	void	Update(void)	override;
private:
	float	m_fRotateXZ;
	float	m_fRotateY;
	float	m_fDistanceLookPos;

	static const float	MIN_DISTANCE;
	static const float	MAX_DISTANCE;
	static const float	MIN_ROTATE_Y;
	static const float	MAX_ROTATE_Y;
};

#endif