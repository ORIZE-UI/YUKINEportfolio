#ifndef __NUMBER_H__
#define __NUMBER_H__

#include <DirectXMath.h>
#include "Texture.h"

class Number
{
public:
	Number(void);
	~Number(void);

	void	SetPos(DirectX::XMFLOAT3);
	void	SetSize(DirectX::XMFLOAT2);
	void	SetNumber(int);
	void	SetDigit(int);

	void	Draw(void);
private:
	int					m_nMaxDigit;
	DirectX::XMFLOAT3	m_pos;
	DirectX::XMFLOAT2	m_size;
	int					m_nNumber;
	Texture*			m_pNumberTexture;
};

#endif