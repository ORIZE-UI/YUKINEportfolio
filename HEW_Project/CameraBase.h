//#pragma once
#ifndef ___CAMERA_BASE_H___
#define ___CAMERA_BASE_H___

#include<DirectXMath.h>

class CameraBase
{
public:
	CameraBase();
	virtual ~CameraBase();

	virtual void Update() = 0;

	DirectX::XMFLOAT4X4 GetViewMatrix();		//転置済みビュー行列の取得
	DirectX::XMFLOAT4X4 GetProjectionMatrix();	//転置済みプロジェクション行列の取得

	DirectX::XMFLOAT4X4 GetShadowViewMatrix();			//影の転置済みビュー行列の取得
	DirectX::XMFLOAT4X4 GetShadowProjectionMatrix();	//影の転置済みプロジェクション行列の取得

	DirectX::XMFLOAT3 GetPos();
	DirectX::XMFLOAT3 GetLook();

protected:
	DirectX::XMFLOAT3 m_pos, m_look, m_up;	//ビュー行列の設定に必要な変数
	DirectX::XMFLOAT3 m_Spos, m_Slook, m_Sup;	//ビュー行列の設定に必要な変数
	float m_fovy, m_aspect, m_near, m_far;	//プロジェクション行列の設定に必要な変数

};


#endif // !___CAMERA_BASE_H___
