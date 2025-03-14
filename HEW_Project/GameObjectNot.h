#pragma once
#include <DirectXMath.h>
#include "Collision.h"
#include"HCollision.h"
#include"col.h"

class GameObjectNot
{
public:
	GameObjectNot();
	virtual ~GameObjectNot();
	// úﬂàÀÇ≈Ç´Ç»Ç¢ÉuÉçÉbÉN
	bool IsNotCollidingWith(GameObjectNot& other);
	bool HIsNotCollidingWith(GameObjectNot& other);
	bool colNot(GameObjectNot& other);

protected:

	DirectX::XMFLOAT3 minBound;
	DirectX::XMFLOAT3 maxBound;

	DirectX::XMFLOAT3 hminBound;
	DirectX::XMFLOAT3 hmaxBound;


	DirectX::XMFLOAT3 cminBound;
	DirectX::XMFLOAT3 cmaxBound;

private:
	AABB m_aabb;
	HAABB m_haabb;
	CAABB m_caabb;
};
