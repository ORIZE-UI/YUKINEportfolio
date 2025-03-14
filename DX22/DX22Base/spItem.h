#ifndef __SPEED_ITEM_H__
#define __SPEED_ITEM_H__

#include <DirectXMath.h>
#include "Model.h"
#include <vector>
#include "Collision.h"
#include "cameraBase.h"
#include "Shader.h"
#include <random>

class CSpeedItem
{
public:
	CSpeedItem(void);
	~CSpeedItem(void);

	void	Update(void);
	void	Draw(void);

	void	AddSpawnGroup(const std::vector<DirectX::XMFLOAT3>&);

	Collision::Result	CheckCollision(DirectX::XMFLOAT3);
	void	SetCamera(CameraBase*);

private:
	typedef struct
	{
		DirectX::XMFLOAT3	pos;
		bool				isUse;
		int					nGroup;
	}tInstance;

	typedef struct
	{
		std::vector<DirectX::XMFLOAT3>	positions;
		int								nEmptyFrame;
		int								nCntInstance;
		int								nGroupNo;
	}tSpawnGroup;

	void	Spawn(tSpawnGroup&);
	void	Destroy(tInstance&);

	static const int	MAX_ITEM = 20;
	static const int	EMPTY_NOSPAWNN_FRAME = 120;
	static const float	ITEM_RADIUS;

	Model*		m_pItemModel;
	tInstance	m_aInstances[MAX_ITEM];
	std::vector<tSpawnGroup>	m_spawnPosGroup;
	CameraBase*		m_pCamera;
	VertexShader*	m_pModelVS;
	std::random_device	m_rnd;
	float			m_fRotY;
};

#endif