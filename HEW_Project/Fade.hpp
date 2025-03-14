#pragma once

#include "Texture.h"
#include "Curtain.h"
#include "Shader.h"

class Fade
{
public:
	Fade(CurtainUI* curtain);
	~Fade();

	void Update();
	void Draw();

	void Start(bool isIn, float time);
	bool IsPlay();
	float GetAlpha();
private:
	Texture* m_pTex;

	float m_alpha;

	bool m_isIn;
	float m_time;
	float m_totalTime;

	CurtainUI* m_pCurtain;

	PixelShader* m_pPS;
};

