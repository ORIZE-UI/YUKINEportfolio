#include "Geometory.h"
#include <math.h>

#define PI	(3.141592f)


void Geometory::MakeBox()
{
	// 元データの作成
	Vertex vtx[] = {
		// -Z面
		{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},

		// +Z面
		{{ 0.5f,  0.5f, 0.5f}, {0.0f, 0.0f}},
		{{-0.5f,  0.5f, 0.5f}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}},
		{{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},

		// +Y面
		{{-0.5f,  0.5f, 0.5f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f, 0.5f}, {1.0f, 0.0f}},
		{{-0.5f,  0.5f,-0.5f}, {0.0f, 1.0f}},
		{{ 0.5f,  0.5f,-0.5f}, {1.0f, 1.0f}},

		// -Y面
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},

		// +X面
		{{ 0.5f,  0.5f,-0.5f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f, 0.5f}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f,-0.5f}, {0.0f, 1.0f}},
		{{ 0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},

		// -X面
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
		{{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
		{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
	};

	int idx[] = {
		// -Z面
		0, 1, 2,
		1, 3, 2,

		// +Z面
		4, 5, 6,
		5, 7, 6,

		// +Y面
		8,  9, 10,
		9, 11, 10,

		// -Y面
		12, 13, 14,
		13, 15, 14,

		// +X面
		16, 17, 18,
		17, 19, 18,

		// -X面
		20, 21, 22,
		21, 23, 22,
	};

	// バッファの作成
	MeshBuffer::Description desc = {};
	desc.pVtx = vtx;
	desc.vtxCount = sizeof(vtx) / sizeof(vtx[0]);
	desc.vtxSize = sizeof(Vertex);
	desc.pIdx = idx;
	desc.idxCount = sizeof(idx) / sizeof(idx[0]);
	desc.idxSize = sizeof(int);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pBox = new MeshBuffer(desc);
}

void Geometory::MakeCylinder()
{
#if 0
	//--- 頂点の作成
	const int	SPLIT_COUNT = 10;
	const float fRadius = 0.5f;
	const float fHeight = 1.0f;

	Vertex vtx[SPLIT_COUNT * 12];

	// 上面、下面の生成
	for (int nCntSplit = 0; nCntSplit < SPLIT_COUNT; nCntSplit++)
	{
		float fRad = (2.0f * PI) * ((float)nCntSplit / (float)SPLIT_COUNT);
		float fNextRad = (2.0f * PI) * ((float)(nCntSplit + 1) / (float)SPLIT_COUNT);

		// 上面
		vtx[nCntSplit * 12] = {
				{cosf(fRad) * fRadius, fHeight / 2.0f, sinf(fRad) * fRadius},
				{cosf(fRad) * 0.5f + 0.5f, sinf(fRad) * 0.5f + 0.5f}
		};

		vtx[nCntSplit * 12 + 1] = {
				{0.0f, fHeight / 2.0f, 0.0f},
				{0.5f, 0.5f}
		};

		vtx[nCntSplit * 12 + 2] = {
				{cosf(fNextRad) * fRadius, fHeight / 2.0f, sinf(fNextRad) * fRadius},
				{cosf(fNextRad) * 0.5f + 0.5f, sinf(fNextRad) * 0.5f + 0.5f}
		};

		// 下面
		vtx[nCntSplit * 12 + 3] = {
				{cosf(fNextRad) * fRadius, -fHeight / 2.0f, sinf(fNextRad) * fRadius},
				{cosf(fRad) * 0.5f + 0.5f, sinf(fRad) * 0.5f + 0.5f}
		};

		vtx[nCntSplit * 12 + 4] = {
				{0.0f, -fHeight / 2.0f, 0.0f},
				{0.5f, 0.5f}
		};

		vtx[nCntSplit * 12 + 5] = {
				{cosf(fRad) * fRadius, -fHeight / 2.0f, sinf(fRad) * fRadius},
				{cosf(fNextRad) * 0.5f + 0.5f, sinf(fNextRad) * 0.5f + 0.5f}
		};

		// 側面左を作成
		vtx[nCntSplit * 12 + 6] = { {cosf(fRad) * fRadius, fHeight / 2.0f, sinf(fRad) * fRadius},
									{(1.0f / (float)SPLIT_COUNT) * (float)nCntSplit, 0.0f} };
		vtx[nCntSplit * 12 + 7] = { {cosf(fNextRad) * fRadius, fHeight / 2.0f, sinf(fNextRad) * fRadius},
									{1.0f, 0.0f} };
		vtx[nCntSplit * 12 + 8] = { {cosf(fRad) * fRadius, -fHeight / 2.0f, sinf(fRad) * fRadius},			{0.0f, 1.0f} };

		// 側面右を作成
		vtx[nCntSplit * 12 + 8] = { {
									vtx[nCntSplit * 12 + 7].pos[0],
									vtx[nCntSplit * 12 + 7].pos[1],
									vtx[nCntSplit * 12 + 7].pos[2]},
									{0.0f, 1.0f} };
		vtx[nCntSplit * 12 + 8] = { {cosf(fRad) * fRadius, -fHeight / 2.0f, sinf(fRad) * fRadius},			{0.0f, 1.0f} };
		vtx[nCntSplit * 12 + 8] = { {cosf(fRad) * fRadius, -fHeight / 2.0f, sinf(fRad) * fRadius},			{0.0f, 1.0f} };

	}

	//--- インデックスの作成
	int idx[];
	int nIdxCnt = 0;

	// 上面
	for (int nCntSplit = 0; nCntSplit < SPLIT_COUNT; nCntSplit++)
	{
		idx[nIdxCnt] = SPLIT_COUNT * 2;	//中心点
		nIdxCnt++;

		// 2番目の頂点
		if (nCntSplit < SPLIT_COUNT - 1)
		{
			idx[nIdxCnt] = nCntSplit * 2 + 2;
		}
		else
		{
			idx[nIdxCnt] = 0;
		}
		nIdxCnt++;

		// 3番目の頂点
		idx[nIdxCnt] = nCntSplit * 2;
		nIdxCnt++;
	}


	// 下面
	for (int nCntSplit = 0; nCntSplit < SPLIT_COUNT; nCntSplit++)
	{
		// 1番目の頂点
		idx[nIdxCnt] = SPLIT_COUNT * 2 + 1;	//中心点
		nIdxCnt++;

		// 2番目の頂点
		idx[nIdxCnt] = nCntSplit * 2 + 1;
		nIdxCnt++;

		// 3番目の頂点
		if (nCntSplit < SPLIT_COUNT - 1)
		{
			idx[nIdxCnt] = nCntSplit * 2 + 3;
		}
		else
		{
			idx[nIdxCnt] = 1;
		}
		nIdxCnt++;

	}


	// 側面
	for (int nCntSplit = 0; nCntSplit < SPLIT_COUNT; nCntSplit++)
	{
		if (nCntSplit < SPLIT_COUNT - 1)
		{
			// 1頂点
			idx[nIdxCnt] = nCntSplit * 2;
			nIdxCnt++;

			// 2頂点
			idx[nIdxCnt] = nCntSplit * 2 + 2;
			nIdxCnt++;

			// 3頂点
			idx[nIdxCnt] = nCntSplit * 2 + 1;
			nIdxCnt++;

			// 4頂点
			idx[nIdxCnt] = nCntSplit * 2 + 2;
			nIdxCnt++;

			// 5頂点
			idx[nIdxCnt] = nCntSplit * 2 + 3;
			nIdxCnt++;

			// 6頂点
			idx[nIdxCnt] = nCntSplit * 2 + 1;
			nIdxCnt++;

		}
		else
		{
			// 1頂点
			idx[nIdxCnt] = nCntSplit * 2;
			nIdxCnt++;

			// 2頂点
			idx[nIdxCnt] = 0;
			nIdxCnt++;

			// 3頂点
			idx[nIdxCnt] = nCntSplit * 2 + 1;
			nIdxCnt++;

			// 4頂点
			idx[nIdxCnt] = 0;
			nIdxCnt++;

			// 5頂点
			idx[nIdxCnt] = 1;
			nIdxCnt++;

			// 6頂点
			idx[nIdxCnt] = nCntSplit * 2 + 1;
			nIdxCnt++;
		}
	}

	//--- バッファの作成
	MeshBuffer::Description desc = {};
	desc.pVtx = vtx;
	desc.vtxCount = SPLIT_COUNT * 2 + 2;
	desc.vtxSize = sizeof(Vertex);
	desc.pIdx = idx;
	desc.idxCount = (3 * 2 * SPLIT_COUNT) + (6 * SPLIT_COUNT);
	desc.idxSize = sizeof(int);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pCylinder = new MeshBuffer(desc);
#endif
}

void Geometory::MakeSphere()
{
	//--- 頂点の作成

	//--- インデックスの作成

	// バッファの作成
}