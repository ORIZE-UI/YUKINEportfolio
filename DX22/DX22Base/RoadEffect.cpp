#include "RoadEffect.h"

// コンストラクタ
RoadEffect::RoadEffect(void)
	: m_bIsSetSection(false)
{
	const float fRadius = 6.0f;
	const float fBold = 6.0f;
	const int	nSplit = 3;
	const float	fPadding = 40.0f;

	// 配列のリサイズ
	m_sectionInfo.resize((1 + nSplit) * 4 + 2);

	// コースの座標を設定
	for (int nCntArc = 0; nCntArc < 4; nCntArc++)
	{
		// 角を配置	
		for (int nCntPoint = 0; nCntPoint < nSplit; nCntPoint++)
		{
			float fAngle = (3.141592f / (nSplit * 2)) * (nSplit * nCntArc + nCntPoint);

			// 円上の座標
			float fPosX = cosf(fAngle) * fRadius;
			float fPosZ = sinf(fAngle) * fRadius;

			// コース上の座標
			if (nCntArc == 0 || nCntArc == 3)
			{
				fPosX += fPadding / 2.0f;
			}
			else
			{
				fPosX -= fPadding / 2.0f;
			}

			if (nCntArc < 2)
			{
				fPosZ += fPadding / 2.0f;
			}
			else
			{
				fPosZ -= fPadding / 2.0f;
			}

			// 配列に格納
			int index = (nSplit + 1) * nCntArc + 1 + nCntPoint;
			m_sectionInfo[index] = { {fPosX, 0.0f, fPosZ}, fBold };
		}
	}

	// 辺を配置
	m_sectionInfo[0] =
	{ { fPadding / 2.0f + fRadius, 0.0f, -fPadding / 2.0f}, fBold };
	m_sectionInfo[nSplit + 1] =
	{ { fPadding / 2.0f, 0.0f, fPadding / 2.0f + fRadius}, fBold };
	m_sectionInfo[(nSplit + 1) * 2] =
	{ {-fPadding / 2.0f - fRadius, 0.0f, fPadding / 2.0f}, fBold };
	m_sectionInfo[(nSplit + 1) * 3] =
	{ {-fPadding / 2.0f, 0.0f,  -fPadding / 2.0f - fRadius}, fBold };
	m_sectionInfo[m_sectionInfo.size() - 2] = m_sectionInfo[0];
	m_sectionInfo[m_sectionInfo.size() - 1] = m_sectionInfo[1];
	m_sectionInfo[m_sectionInfo.size() - 1].pos.y = -0.1f;

}

RoadEffect::~RoadEffect(void)
{

}

void RoadEffect::UpdateControlPoints(LineID id, ControlPoints& controlPoints)
{
	// 道の更新
	if (m_bIsSetSection == false)
	{
		// 道の頂点を設定
		for (int nCntSection = 0; nCntSection < controlPoints.size(); nCntSection++)
		{
			controlPoints[nCntSection].pos = m_sectionInfo[nCntSection].pos;
			controlPoints[nCntSection].bold = m_sectionInfo[nCntSection].bold;
		}

		// フラグを更新
		m_bIsSetSection = true;
	}
}