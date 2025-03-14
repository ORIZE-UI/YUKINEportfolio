#ifndef __CAMERA_PLAYER_H__
#define __CAMERA_PLAYER_H__

#include "player.h"

class CameraPlayer : public CameraBase
{
public:
	CameraPlayer(Player*);
	~CameraPlayer();
	void	Update(void)	override;
private:
	Player*	m_pPlayer;
	float	m_fRotateXZ;
	float	m_fRotateY;
	float	m_fDistanceLookPos;

	static const float	MIN_DISTANCE;
	static const float	MAX_DISTANCE;
	static const float	MIN_ROTATE_Y;
	static const float	MAX_ROTATE_Y;

};

#endif