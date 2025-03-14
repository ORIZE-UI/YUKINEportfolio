#ifndef __FIELD_H__
#define __FIELD_H__

#include "Collision.h"  // 衝突判定関連の処理を含むヘッダーファイル
#include <vector>       // 動的配列 std::vector を利用するためのヘッダーファイル
#include "Texture.h"    // テクスチャ関連の処理を含むヘッダーファイル
#include "Model.h"      // モデルデータ関連の処理を含むヘッダーファイル
#include "Shader.h"     // シェーダー関連の処理を含むヘッダーファイル
#include "cameraBase.h" // カメラ基盤クラスの処理を含むヘッダーファイル

// フィールド（レース場など）の表現を行うクラス
class Field
{
public:
	Field();
	~Field();

	void Draw();						 // フィールドを描画する関数
	Collision::Plane GetPlaneInfo(int);  // 指定されたインデックスの平面の情報を取得する関数
	int GetDataNum();					 // フィールド内のデータ数を取得する関数
	void SetCamera(CameraBase*);		 // カメラの設定を行う関数

private:
	// フィールド内のデータ構造を表現する構造体
	struct Data
	{
		DirectX::XMFLOAT3 pos;		// 天面の位置（3D座標）
		DirectX::XMFLOAT3 size;		// サイズ（幅、高さ、奥行きなど）
		DirectX::XMFLOAT3 normal;	// 法線ベクトル（平面の向きを表す）
	};

	std::vector<Data>	m_data;         // フィールド内の各データ（天面など）を保持する動的配列
	Model*				m_pGroundModel; // フィールドの地面モデルへのポインタ
	VertexShader*		m_pVS;          // 頂点シェーダーへのポインタ
	CameraBase*			m_pCamera;      // カメラの基本クラスのポインタ
};

#endif
