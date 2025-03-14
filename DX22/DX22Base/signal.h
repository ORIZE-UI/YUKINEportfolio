#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#include "Texture.h"
#include <DirectXMath.h>

class Signal
{
public:
	Signal();
	~Signal();

	void	Update(void);
	void	Draw(void);

	bool	IsSignalEnd(void);
	void	CountDownStart(void);

private:
	Texture*	m_pSignalTexture;
	int			m_nCntFrame;
	DirectX::XMFLOAT3	m_pos;
	bool		m_bIsActive;

	static const int	MAX_SIGNAL_FRAME;
};

#endif