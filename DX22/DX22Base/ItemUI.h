#ifndef __ITEM_UI_H__
#define __ITEM_UI_H__

#include "Texture.h"
#include <DirectXMath.h>

class ItemUI
{
public:
	ItemUI(void);
	~ItemUI(void);

	void	Draw(void);
private:
	Texture*			m_pStarTexture;
	DirectX::XMFLOAT3	m_starPos;
	DirectX::XMFLOAT3	m_starScale;
	DirectX::XMFLOAT3	m_starRotate;
};

#endif