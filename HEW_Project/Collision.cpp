#include "Collision.h"
#include <algorithm>
using namespace std;
bool AABB::IsAABBIntersecting(const DirectX::XMFLOAT3& minA, const DirectX::XMFLOAT3& maxA, const DirectX::XMFLOAT3& minB, const DirectX::XMFLOAT3& maxB) {
	return (minA.x <= maxB.x && maxA.x >= minB.x) &&
		(minA.y <= maxB.y && maxA.y >= minB.y) &&
		(minA.z <= maxB.z && maxA.z >= minB.z);
}

bool AABB::IntersectAABBTriangle(const DirectX::XMFLOAT3& minA, const DirectX::XMFLOAT3& maxA,
	const DirectX::XMFLOAT3& v0, const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2) {
	// AABBと三角形の交差を判定

	// 三角形の辺との交差をテスト
	DirectX::XMFLOAT3 triangleEdges[3] = {
		DirectX::XMFLOAT3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z),
		DirectX::XMFLOAT3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z),
		DirectX::XMFLOAT3(v0.x - v2.x, v0.y - v2.y, v0.z - v2.z)
	};

	DirectX::XMFLOAT3 axis;
	for (int i = 0; i < 3; ++i) {
		// 三角形の辺との交差をテスト
		DirectX::XMFLOAT3 axisCrossInput(0, 0, 1);
		DirectX::XMVECTOR crossResult = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&triangleEdges[i]), DirectX::XMLoadFloat3(&axisCrossInput));
		DirectX::XMStoreFloat3(&axis, crossResult);
		if (!TestAxisAgainstAABB(axis, minA, maxA, v0, v1, v2)) {
			return false;
		}

		DirectX::XMFLOAT3 axisCrossInput1(0, 1, 0);
		DirectX::XMVECTOR crossResult1 = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&triangleEdges[i]), DirectX::XMLoadFloat3(&axisCrossInput1));
		DirectX::XMStoreFloat3(&axis, crossResult1);		
		if (!TestAxisAgainstAABB(axis, minA, maxA, v0, v1, v2)) {
			return false;
		}

		DirectX::XMFLOAT3 axisCrossInput2(1, 0, 0);
		DirectX::XMVECTOR crossResult2 = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&triangleEdges[i]), DirectX::XMLoadFloat3(&axisCrossInput2));
		DirectX::XMStoreFloat3(&axis, crossResult2);
		if (!TestAxisAgainstAABB(axis, minA, maxA, v0, v1, v2)) {
			return false;
		}
	}

	return true;
}

bool AABB::TestAxisAgainstAABB(const DirectX::XMFLOAT3& axis, const DirectX::XMFLOAT3& minA, const DirectX::XMFLOAT3& maxA,
	const DirectX::XMFLOAT3& v0, const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2) {
	// AABBの境界ボックスとの交差をテスト

	// 三角形を軸上に射影
	float triangleMin = std::min({ DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&v0), DirectX::XMLoadFloat3(&axis))),
								   DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&v1), DirectX::XMLoadFloat3(&axis))),
								   DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&v2), DirectX::XMLoadFloat3(&axis))) });

	float triangleMax = std::max({ DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&v0), DirectX::XMLoadFloat3(&axis))),
								   DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&v1), DirectX::XMLoadFloat3(&axis))),
								   DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&v2), DirectX::XMLoadFloat3(&axis))) });

	// 重なりをテスト
	triangleMin += 1.0f;
	triangleMax += 1.0f;
	return !(triangleMax < DirectX::XMVectorGetX(DirectX::XMLoadFloat3(&minA))) || (triangleMin > DirectX::XMVectorGetX(DirectX::XMLoadFloat3(&maxA)))
		&& !(triangleMax < DirectX::XMVectorGetY(DirectX::XMLoadFloat3(&minA))) || (triangleMin > DirectX::XMVectorGetY(DirectX::XMLoadFloat3(&maxA)))
		&& !(triangleMax < DirectX::XMVectorGetZ(DirectX::XMLoadFloat3(&minA))) || (triangleMin > DirectX::XMVectorGetZ(DirectX::XMLoadFloat3(&maxA)));
}
bool AABB::IsPointInsideAABB(const std::vector<DirectX::XMFLOAT3>& points, const DirectX::XMFLOAT3& minA, const DirectX::XMFLOAT3& maxA) {
	for (size_t i = 0; i < points.size(); ++i) {
		if (points[i].x >= minA.x && points[i].x <= maxA.x &&
			points[i].y >= minA.y && points[i].y <= maxA.y &&
			points[i].z >= minA.z && points[i].z <= maxA.z) {
			// Point i is inside the AABB
			return true;
		}
	}
	return false;
}