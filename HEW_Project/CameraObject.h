#pragma once
//#pragma once
#ifndef ___CAMERA_PLAYER_H___
#define ___CAMERA_PLAYER_H___

#include"Objectcamera.h"

class CameraObject
	:public CameraBase
{
public:
	CameraObject(ObjectCamera* pCamera);
	~CameraObject();

	void Update();

	//DirectX::XMFLOAT3 GetPos();
	//DirectX::XMFLOAT3 GetLook();

private:
	ObjectCamera* m_pobjCamera;

	//カメラのパラメータ
	float m_radXZ;
	float m_radY;
	float m_radius;
};

#endif // !___CAMERA_PLAYER_H___

