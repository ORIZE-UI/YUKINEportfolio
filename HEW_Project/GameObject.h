#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Collision.h"
#include"HCollision.h"
#include"col.h"

class GameObject 
{
public:
	GameObject();
	virtual ~GameObject();
	bool IsCollidingWith(GameObject& other);
	bool HIsCollidingWith(GameObject& other);
	bool col(GameObject & other);
	bool IsCollisionBoxTriangle(GameObject & other);
	bool IsCollisionPoint(GameObject & other);

	DirectX::XMFLOAT3 GetCMinBounds();
	DirectX::XMFLOAT3 GetMaxBounds();

protected:
	
	DirectX::XMFLOAT3 minBound;
	DirectX::XMFLOAT3 maxBound;

	DirectX::XMFLOAT3 hminBound;
	DirectX::XMFLOAT3 hmaxBound;


	DirectX::XMFLOAT3 cminBound;
	DirectX::XMFLOAT3 cmaxBound;
	
	DirectX::XMFLOAT3 v0;
	DirectX::XMFLOAT3 v1;
	DirectX::XMFLOAT3 v2;

	DirectX::XMFLOAT3 point;
	std::vector<DirectX::XMFLOAT3> points;
	struct TriangleVertex
	{
		DirectX::XMFLOAT3 position;
		// ëºÇÃèÓïÒÅiñ@ê¸Ç»Ç«ÅjÇ‡ä‹ÇﬂÇÈÇ±Ç∆Ç™Ç≈Ç´Ç‹Ç∑
	};
private:
	AABB m_aabb;
	HAABB m_haabb;
	CAABB m_caabb;
};
