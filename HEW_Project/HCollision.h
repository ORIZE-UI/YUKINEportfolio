#pragma once
#pragma once
#include <DirectXMath.h>
class HAABB
{
public:
	bool HIsAABBIntersecting(const DirectX::XMFLOAT3& minA, const DirectX::XMFLOAT3& maxA, const DirectX::XMFLOAT3& minB, const DirectX::XMFLOAT3& maxB);

private:
	
};
