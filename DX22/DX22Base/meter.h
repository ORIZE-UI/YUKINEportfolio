#ifndef __METER_H__
#define __METER_H__

#include "Texture.h"
#include "player.h"

class Meter
{
public:
	Meter(Player*);
	~Meter(void);
	void	Update(void);
	void	Draw(void);
private:
	Texture*	m_pBackImg;
	Texture*	m_pFrontImg;
	Player*		m_pPlayer;
	float		m_fRotZ;
};

#endif