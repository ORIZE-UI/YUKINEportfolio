#include "TrailEffect.h"
#include "player.h"
#include "CPUCar.h"

TrailEffect::TrailEffect(Vehicle* vehicle)
	: m_pVehicle(vehicle)
{
	if (vehicle)
	{
		m_oldPos = vehicle->GetPos();
	}
}

TrailEffect::~TrailEffect(void)
{
	m_pVehicle = nullptr; // 車両の参照を解除
}

void TrailEffect::UpdateControlPoints(LineID id, ControlPoints& controlPoints)
{
	// ポリラインの幅を少しずつ小さくする
	auto polyLineIter = controlPoints.begin();
	while (polyLineIter != controlPoints.end())
	{
		// 透明にする処理
		auto& color = polyLineIter->color;
		color.w *= 0.95f;
		++polyLineIter;
	}

	// プレイヤーの移動を取得
	//float distance;	// 距離
	//DirectX::XMFLOAT3 pos = m_pPlayer->GetPos();
	//DirectX::XMVECTOR vOld = DirectX::XMLoadFloat3(&m_oldPos);
	//DirectX::XMVECTOR vNow = DirectX::XMLoadFloat3(&pos);
	//DirectX::XMVECTOR vDir = DirectX::XMVectorSubtract(vNow, vOld);
	//DirectX::XMStoreFloat(&distance, DirectX::XMVector3Length(vDir));

	// 現在位置を取得
	DirectX::XMFLOAT3 currentPos = m_pVehicle->GetPos();

	// 前回位置との差を確認してポリラインを更新
	DirectX::XMVECTOR vOldPos = DirectX::XMLoadFloat3(&m_oldPos);
	DirectX::XMVECTOR vCurrentPos = DirectX::XMLoadFloat3(&currentPos);
	DirectX::XMVECTOR vDiff = DirectX::XMVectorSubtract(vCurrentPos, vOldPos);
	float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(vDiff));

	if (distance >= 0.2f)
	{
		// 1つずつずらす
		for (int i = controlPoints.size() - 1; i > 0; i--)
		{
			controlPoints[i] = controlPoints[i - 1];
		}

		DirectX::XMFLOAT3 tempPos = currentPos;

		tempPos.y += 0.01f;
		tempPos.z += (id * 2.0f);

		controlPoints[0].pos = tempPos;
		controlPoints[0].bold = 0.5f;
		controlPoints[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };

		if (id == GetLineCount() - 1)
		{
			// プレイヤーの前の位置を更新
			m_oldPos = currentPos;
		}
	}
}