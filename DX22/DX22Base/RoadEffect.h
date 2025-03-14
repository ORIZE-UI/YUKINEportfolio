#ifndef __ROAD_EFFECT_H__
#define __ROAD_EFFECT_H__

#include "Polyline.h"
#include <vector>
#include <DirectXMath.h>

class RoadEffect : public Polyline
{
public:
	// コントロールポイントの構造体定義
	typedef struct
	{
		DirectX::XMFLOAT3 pos;	//位置
		float bold;	//大きさ
	}tSectionInfo;

	RoadEffect(void);
	~RoadEffect(void);

private:
	void UpdateControlPoints(LineID id, ControlPoints& controlPoints) override;
	//void UpdateControlPoints(int id, const std::vector<ControlPoint>& controlPoints);

	std::vector<tSectionInfo> m_sectionInfo;// 道のセクション情報
	bool m_bIsSetSection;
};

#endif