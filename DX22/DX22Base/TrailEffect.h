#ifndef __TRAIL_EFFECT_H__
#define __TRAIL_EFFECT_H__

#include "Polyline.h"
#include <DirectXMath.h>

// 基底クラスとして車両を抽象化
class Vehicle
{
public:
	virtual ~Vehicle() {}
	virtual DirectX::XMFLOAT3 GetPos() const = 0; // 車両の現在位置を取得する
};

class TrailEffect : public Polyline
{
public:
	// コンストラクタで Vehicle ポインタを受け取る
	TrailEffect(Vehicle* vehicle);
	~TrailEffect(void);

protected:
	void UpdateControlPoints(LineID id, ControlPoints& controlPoints) override;

	Vehicle* m_pVehicle;            // 車両（Player または CPUCar）
	DirectX::XMFLOAT3 m_oldPos;     // 前回位置
};

#endif