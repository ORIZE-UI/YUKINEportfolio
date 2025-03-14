#include "HCollision.h"

//オブジェクト同士の衝突判定
bool HAABB::HIsAABBIntersecting(const DirectX::XMFLOAT3 & minA, const DirectX::XMFLOAT3 & maxA, const DirectX::XMFLOAT3 & minB, const DirectX::XMFLOAT3 & maxB)
{
	return (minA.x <= maxB.x && maxA.x >= minB.x) &&
		(minA.y <= maxB.y && maxA.y >= minB.y) &&
		(minA.z <= maxB.z && maxA.z >= minB.z);
}

