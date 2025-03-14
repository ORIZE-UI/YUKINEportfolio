
#pragma once
#include <DirectXMath.h>
class CAABB
{
public:
	bool col(const DirectX::XMFLOAT3& minA, const DirectX::XMFLOAT3& maxA, const DirectX::XMFLOAT3& minB, const DirectX::XMFLOAT3& maxB);

private:

};
