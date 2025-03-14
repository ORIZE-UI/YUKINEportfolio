#ifndef __CPU_CAR_H__
#define __CPU_CAR_H__

#include "Model.h"
#include "Shader.h"
#include "cameraBase.h"
#include "Texture.h"
#include "TrailEffect.h"

// CPUCar クラス: 自動運転する車両を表す
class CPUCar : public Vehicle
{
public:
	CPUCar();
	~CPUCar();

	// 各フレームで呼ばれる処理
	void Update(); // 移動や挙動の更新
	void Draw();   // 描画処理

	// カメラを設定
	void SetCamera(CameraBase* pCamera);

	// 現在位置を取得
	DirectX::XMFLOAT3 GetPos() const override;

	// 制御ポイントを追加
	void AddControlPoint(const DirectX::XMFLOAT3& point);

	// 道の制御点を設定
	void SetControlPoints(const std::vector<DirectX::XMFLOAT3>& controlPoints);

	// タイマーの状態を設定
	void SetActiveTimer(bool isActive); 

private:
	Model* m_pModel;              // 車両モデル
	VertexShader* m_pVS;          // 頂点シェーダ
	CameraBase* m_pCamera;        // カメラ情報
	Texture* m_pShadowTexture;    // 影用テクスチャ
	TrailEffect* m_pTrailEffect;  // 軌跡エフェクト

	DirectX::XMFLOAT3 m_pos;      // 現在位置
	DirectX::XMFLOAT3 m_lookDir;  // 向きベクトル
	DirectX::XMFLOAT3 m_speed;    // 速度ベクトル

	// 制御ポイント関連
	std::vector<DirectX::XMFLOAT3> m_controlPoints; // 制御ポイントのリスト
	int m_currentControlPoint;                      // 現在の制御ポイントのインデックス

	// 移動関連パラメータ
	float m_fAccelLen;  // 加速力
	float m_fMaxSpeed;  // 最大速度
	float m_turnSpeed;  // 回転速度

	// 減速係数（静的定数として定義）
	static const float DECREASE_SPEED;

	// タイマーの状態（true: 動作中, false: 停止中）
	bool m_bIsActiveTimer; 
};

#endif