#include "Collision.h"

Collision::Result Collision::CheckRayPlane(Ray ray, Plane plane)
{
	Result result = { false };

	// レイの初期値
	float rayLength;
	DirectX::XMVECTOR vRayStart = DirectX::XMLoadFloat3(&ray.start);
	DirectX::XMVECTOR vRayN = DirectX::XMLoadFloat3(&ray.direction);
	DirectX::XMVECTOR vRayEnd = DirectX::XMVectorAdd(vRayStart, vRayN);
	DirectX::XMStoreFloat(&rayLength, DirectX::XMVector3Length(vRayN));
	vRayN = DirectX::XMVector3Normalize(vRayN);

	// 面の初期値
	DirectX::XMVECTOR vPlaneN = DirectX::XMLoadFloat3(&plane.normal);
	DirectX::XMVECTOR vPlanePos = DirectX::XMLoadFloat3(&plane.pos);
	vPlaneN = DirectX::XMVector3Normalize(vPlaneN);

	// 平面の座標->レイの始点
	float P1;
	DirectX::XMVECTOR vToStart = DirectX::XMVectorSubtract(vRayStart, vPlanePos);
	DirectX::XMStoreFloat(&P1, DirectX::XMVector3Dot(vPlaneN, vToStart));

	// 平面の座標->レイの終点
	float P2;
	DirectX::XMVECTOR vToEnd = DirectX::XMVectorSubtract(vRayEnd, vPlanePos);
	DirectX::XMStoreFloat(&P2, DirectX::XMVector3Dot(vPlaneN, vToEnd));

	// レイが面を貫通しているか
	if (P1 >= 0.0f && P2 <= 0.0f)
	{
		result.hit = true;

		float rate = P1 / (P1 + fabsf(P2));
		rayLength = rayLength * rate;
		DirectX::XMStoreFloat3(&result.point, DirectX::XMVectorAdd(
			vRayStart, DirectX::XMVectorScale(vRayN, rayLength)
		));
	}

	return result;

}

Collision::Result Collision::CheckPointTriangle(DirectX::XMFLOAT3 pos, Triangle triangle)
{
	Result result = { false, pos };

	// 辺のベクトルと座標へのベクトルから外積を求める
	DirectX::XMVECTOR vCross[3];	//外積の優先結果
	DirectX::XMVECTOR vPoint = DirectX::XMLoadFloat3(&pos);
	DirectX::XMVECTOR vTriStart = DirectX::XMLoadFloat3(&triangle.p[0]);

	for (int i = 0; i < 3; i++)
	{
		// 各ベクトルを計算
		DirectX::XMVECTOR vTriEnd = i == 2 ?
			DirectX::XMLoadFloat3(&triangle.p[0]) :
			DirectX::XMLoadFloat3(&triangle.p[i + 1]);
		DirectX::XMVECTOR vTriEdge = DirectX::XMVectorSubtract(vTriEnd, vTriStart);
		DirectX::XMVECTOR vToPoint = DirectX::XMVectorSubtract(vPoint, vTriStart);

		// 各外積を計算
		vCross[i] = DirectX::XMVector3Cross(vTriEdge, vToPoint);
		vCross[i] = DirectX::XMVector3Normalize(vCross[i]);

		// 始点を更新
		vTriStart = vTriEnd;
	}

	// 外積のベクトル同士から内積を計算
	DirectX::XMVECTOR vDot[3] = {
		DirectX::XMVector3Dot(vCross[0], vCross[1]),
		DirectX::XMVector3Dot(vCross[1], vCross[2]),
		DirectX::XMVector3Dot(vCross[2], vCross[0])
	};

	float match[3];
	for (int nCntMatch = 0; nCntMatch < 3; nCntMatch++)
	{
		DirectX::XMStoreFloat(&match[nCntMatch], vDot[nCntMatch]);
	}

	if (match[0] >= 0.0f && match[1] >= 0.0f && match[2] >= 0.0f)
	{
		result.hit = true;
		result.point = pos;
	}

	return result;
}