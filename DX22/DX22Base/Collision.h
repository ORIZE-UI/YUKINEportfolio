#ifndef __COLLISION_H__
#define __COLLISION_H__

#include <DirectXMath.h>

class Collision
{
public:
	// 面
	struct Plane
	{
		DirectX::XMFLOAT3 normal;	//法線
		DirectX::XMFLOAT3 pos;		//座標
	};

	// レイ
	struct Ray
	{
		DirectX::XMFLOAT3 start;		//始点
		DirectX::XMFLOAT3 direction;	//方向
	};

	// 三角形
	struct Triangle
	{
		DirectX::XMFLOAT3 p[3];	//頂点座標
	};

	// 結果
	struct Result
	{
		bool				hit;	//衝突フラグ
		DirectX::XMFLOAT3	point;	//ヒット位置
	};

	static Result CheckRayPlane(Ray, Plane);	//レイと無限平面
	static Result CheckPointTriangle(DirectX::XMFLOAT3, Triangle);	//レイと三角形
};

#endif