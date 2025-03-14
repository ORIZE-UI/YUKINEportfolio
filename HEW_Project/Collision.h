#pragma once
#include <DirectXMath.h>
#include <vector>
class AABB
{
public:
	bool IsAABBIntersecting(const DirectX::XMFLOAT3& minA, const DirectX::XMFLOAT3& maxA, const DirectX::XMFLOAT3& minB, const DirectX::XMFLOAT3& maxB);
	bool IntersectAABBTriangle(const DirectX::XMFLOAT3& minA, const DirectX::XMFLOAT3& maxA,
		const DirectX::XMFLOAT3& v0, const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);
	bool TestAxisAgainstAABB(const DirectX::XMFLOAT3& axis, const DirectX::XMFLOAT3& minA, const DirectX::XMFLOAT3& maxA,
		const DirectX::XMFLOAT3& v0, const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);
	bool IsPointInsideAABB(const std::vector<DirectX::XMFLOAT3>& points, const DirectX::XMFLOAT3& minA, const DirectX::XMFLOAT3& maxA);
private:
};
