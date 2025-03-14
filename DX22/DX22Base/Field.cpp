#include "Field.h"
#include "Geometory.h"
#include <math.h>
#include "Sprite.h"

// コンストラクタ: フィールドの初期化
Field::Field()
{
	// 初期データとして地面の位置、サイズ、法線を設定
	Data initData[] = {
		{{0.0f, 0.0f, 0.0f}, {20.0f, 3.0f, 20.0f}, {0.0f, 1.0f, 0.0f}},
	};

	// 初期データを m_data ベクトルに追加
	for (int i = 0; i < _countof(initData); ++i)
	{
		m_data.push_back(initData[i]);
	}

	// 地面のモデルをロード
	m_pGroundModel = new Model();
	if (!m_pGroundModel->Load("Assets/Model/glassGround.fbx"))
	{
		MessageBox(NULL, "ModelRead", "Error", MB_OK); // モデルの読み込みエラーチェック
	}

	// 頂点シェーダーを読み込み
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBoxA(nullptr, "VS_Model.cso", "Error", MB_OK); // シェーダーの読み込みエラーチェック
	}
	m_pGroundModel->SetVertexShader(m_pVS); // モデルに頂点シェーダーを設定
}

// デストラクタ: メモリの解放
Field::~Field()
{
	delete m_pVS;
	delete m_pGroundModel;
}

// フィールドの描画処理
void Field::Draw()
{
	for (int i = 0; i < m_data.size(); ++i)
	{
		// 法線をもとにした回転軸と回転角度の計算
		DirectX::XMVECTOR vStart = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // 初期の上方向
		DirectX::XMVECTOR vTarget = DirectX::XMLoadFloat3(&m_data[i].normal);    // 法線ベクトルを読み込む
		vStart = DirectX::XMVector3Normalize(vStart);
		vTarget = DirectX::XMVector3Normalize(vTarget);

		// 回転軸を求める（法線ベクトルと上方向ベクトルの外積）
		DirectX::XMVECTOR vCross = DirectX::XMVector3Cross(vStart, vTarget);
		vCross = DirectX::XMVector3Normalize(vCross);

		// 回転角度を求める（法線ベクトルと上方向ベクトルの内積から）
		float angle;
		DirectX::XMVECTOR vDot = DirectX::XMVector3Dot(vStart, vTarget);
		DirectX::XMStoreFloat(&angle, vDot);
		angle = acosf(angle);

		// 地形の変換行列を設定
		DirectX::XMMATRIX mSize = DirectX::XMMatrixScaling(m_data[i].size.x, m_data[i].size.y, m_data[i].size.z); // スケーリング行列
		DirectX::XMMATRIX mOffset = DirectX::XMMatrixTranslation(0.0f, -m_data[i].size.y * 0.5f, 0.0f);           // オフセット行列
		DirectX::XMMATRIX mRot = angle > 0.0f ? DirectX::XMMatrixRotationAxis(vCross, angle) : DirectX::XMMatrixIdentity(); // 回転行列

		// ワールド行列の作成（スケール、オフセット、回転を適用）
		DirectX::XMMATRIX mWorld = mSize * mOffset * mRot;
		mWorld = DirectX::XMMatrixTranspose(mWorld); // 行列を転置

		// ワールド行列を `Geometory` クラスに設定
		DirectX::XMFLOAT4X4 world;
		DirectX::XMStoreFloat4x4(&world, mWorld);
		Geometory::SetWorld(world);

		// モデルの描画行列を設定
		DirectX::XMFLOAT4X4 mat[3];
		mat[0] = world;                                   // ワールド行列
		mat[1] = m_pCamera->GetViewMatrix();              // ビュー行列
		mat[2] = m_pCamera->GetProjectionMatrix();        // プロジェクション行列

		m_pVS->WriteBuffer(0, mat); // 頂点シェーダにバッファを転送
		m_pVS->Bind();              // シェーダーをバインド
		m_pGroundModel->Draw();      // モデルを描画
	}
}

// 指定されたインデックスの平面情報を返す
Collision::Plane Field::GetPlaneInfo(int index)
{
	Collision::Plane plane;
	plane.normal = m_data[index].normal; // 法線ベクトル
	plane.pos = m_data[index].pos;       // 位置ベクトル

	return plane;
}

// フィールド内のデータ数を返す
int Field::GetDataNum()
{
	return m_data.size();
}

// カメラを設定する
void Field::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}