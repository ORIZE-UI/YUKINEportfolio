#pragma once
#ifndef __GOAL_H__
#define __GOAL_H__

#include "Texture.h"
#include "Collision.h"
#include "Shader.h"

#include "LibEffekseer.h"
#include "Effekseer/Effekseer.h"

//class SceneManager;


class Goal
{
public:
	Goal();
	~Goal();

	void Update();
	void Draw(float x, float y, float z, float sizeX, float sizeY);

	bool IsGoal = false;									// ゴールしたかどうかを返す

	float SetPosition(float x, float y, float z);			// ゴール位置を設定
	DirectX::XMFLOAT3 GetPosition() const;

	float GetSizeX(float x);								// ゴールの横幅を取得
	float GetSizeY(float y);								// ゴールの縦幅を取得
	DirectX::XMFLOAT3 GetSize() const;						// ゴールのサイズを取得

	void SetGoal(bool Goal);
	bool GetGoal();

private:
	Texture* m_pGoalTexture;
	
	DirectX::XMFLOAT3 m_GoalPos;							// ゴールの現在の位置
	DirectX::XMFLOAT3 m_GoalSize;							//ゴールの大きさ

//	SceneManager* m_pSceneManager;

	PixelShader* m_pPS;

	Effekseer::Handle m_EffectHandle; // エフェクトハンドル
	Effekseer::EffectRef m_EffectGoal; // エフェクト Goal

};

#endif
