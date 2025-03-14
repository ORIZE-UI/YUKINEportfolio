#pragma once
#ifndef __SKYDOME_H__
#define __SKYDOME_H__

#include "Model.h"
#include "Shader.h"
#include "cameraBase.h"

class Skydome 
{
public:
	Skydome();                     // コンストラクタ
	~Skydome();                    // デストラクタ

	//void Update();                 // 更新処理（必要なら）
	void Draw(void);                   // 描画処理
	void SetCamera(CameraBase*);

private:
	Model*			  m_pModel;    // スカイドーム用のモデル
	VertexShader*	  m_pVS;       // 頂点シェーダー
	CameraBase*		  m_pCamera;
};

#endif