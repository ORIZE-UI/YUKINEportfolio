#ifndef __FADE_H__
#define __FADE_H__

#include "Texture.h"

class Fade
{
public:
	enum class FadeKind
	{
		None,
		FadeIn,
		FadeOut,
	};

	Fade(void);
	~Fade(void);

	void	Update(void);
	void	Draw(void);

	void	StartFade(FadeKind);
	FadeKind	GetKind(void);
	bool		IsFinish(void);

private:
	Texture*	m_pFadeTexture;
	float		m_fAlpha;
	FadeKind	m_fadeState;

	static const float	FADE_SPEED;
};

#endif