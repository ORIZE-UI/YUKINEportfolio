#pragma once
#include "Texture.h"

class ItemUI
{
public:
	ItemUI();
	~ItemUI();

	void Update();
	void Draw();

private:
	Texture* m_pTexture;
};

