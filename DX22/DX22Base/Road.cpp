#include "Road.h"

// コンストラクタ: 道の初期化処理
Road::Road(void)
	: m_pCamera(nullptr)
{
	// 道のテクスチャをロード
	m_pRoadTexture = new Texture();
	m_pRoadTexture->Create("Assets/Texture/road1.png");

	// 道のエフェクトを生成し、ラインを追加
	m_pRoadEffect = new RoadEffect();
	m_pRoadEffect->AddLine(18); // 道を構成するラインのポイント数

	// 道のUVスケールを設定
	Polyline::Line& line = m_pRoadEffect->GetLine(0);
	line.uvScale = { 10.0f, 1.0f }; // X方向を10倍、Y方向を1倍にスケーリング
}

// デストラクタ: メモリの解放
Road::~Road(void)
{
	delete m_pRoadEffect;
	m_pRoadEffect = nullptr;
	delete m_pRoadTexture;
	m_pRoadTexture = nullptr;
}

// 毎フレーム呼ばれる更新処理
void Road::Update(void)
{
	m_pRoadEffect->Update(); // 道のエフェクトを更新
}

// 道の描画処理
void Road::Draw(void)
{
	// カメラのビューおよびプロジェクション行列を設定
	m_pRoadEffect->SetView(m_pCamera->GetViewMatrix());
	m_pRoadEffect->SetProjection(m_pCamera->GetProjectionMatrix());

	// テクスチャとエフェクトを設定し、描画
	m_pRoadEffect->SetTexture(m_pRoadTexture);
	m_pRoadEffect->Draw();
}

// カメラを設定する
void Road::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

void Road::SetControlPointsToCar(CPUCar* car)
{
	// 道の制御点をCPUCarに設定
	std::vector<DirectX::XMFLOAT3> controlPoints;
	for (const auto& point : m_pRoadEffect->GetLine(0).controlPoints)
	{
		controlPoints.push_back(point.pos);
	}
	car->SetControlPoints(controlPoints);
}

// 道との衝突判定処理
Road::tCollisionResult Road::CheckCollision(Collision::Ray ray)
{
	// 初期化: 結果に衝突していないことを設定
	tCollisionResult result;
	result.collisionResult.hit = false;

	// 道の制御点（コントロールポイント）の数を取得
	int nMaxControl = m_pRoadEffect->GetLine(0).controlPoints.size();

	// 道のセグメントの始点を格納する変数
	DirectX::XMFLOAT3 lastRoadPos[2] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f}
	};

	// コントロールポイントの参照を取得
	auto& controlPoints = m_pRoadEffect->GetLine(0).controlPoints;

	// 各コントロールポイントに基づいて道のセグメントごとに処理
	for (int nCntControl = 0; nCntControl < nMaxControl; nCntControl++)
	{
		// 道のセグメント位置を計算
		//DirectX::XMFLOAT3 polyPos[4];
		DirectX::XMFLOAT3 linePos = controlPoints[nCntControl].pos;
		int nPrevIndex = nCntControl == 0 ? 0 : nCntControl - 1;

		// 各位置ベクトルの計算
		DirectX::XMVECTOR vPrev = DirectX::XMLoadFloat3(&controlPoints[nPrevIndex].pos);
		DirectX::XMVECTOR vNext = DirectX::XMLoadFloat3(&linePos);
		DirectX::XMVECTOR vNormal = DirectX::XMLoadFloat3(&controlPoints[nCntControl].normal);
		DirectX::XMVECTOR vDir = DirectX::XMVectorSubtract(vNext, vPrev);
		vNormal = DirectX::XMVector3Normalize(vNormal);
		vDir = DirectX::XMVector3Normalize(vDir);
		vDir = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vNormal, vDir));
		vDir = DirectX::XMVectorScale(vDir, controlPoints[nCntControl].bold);
		DirectX::XMFLOAT3 dir;
		DirectX::XMStoreFloat3(&dir, vDir);

		// 現在のセグメントの道の左右の位置を格納
		DirectX::XMFLOAT3 curRoadPos[2] =
		{
			{ linePos.x + dir.x, linePos.y + dir.y, linePos.z + dir.z },
			{ linePos.x - dir.x, linePos.y - dir.y, linePos.z - dir.z }
		};

		// 初回は判定せず、現在のセグメント位置を次回のセグメントの始点に設定
		if (nCntControl == 0)
		{
			lastRoadPos[0] = curRoadPos[0];
			lastRoadPos[1] = curRoadPos[1];
			continue;
		}

		// 衝突判定用の平面を作成
		Collision::Plane plane;
		plane.pos = curRoadPos[0];
		plane.normal = controlPoints[nCntControl].normal;

		// レイと平面の衝突判定を行う
		Collision::Result planeResult = Collision::CheckRayPlane(ray, plane);
		if (planeResult.hit == true)
		{
			// 平面に当たった場合、三角形ポリゴンを作成して点との判定
			Collision::Triangle triangles[2];
			triangles[0].p[0] = lastRoadPos[0];
			triangles[0].p[1] = lastRoadPos[1];
			triangles[0].p[2] = curRoadPos[0];

			triangles[1].p[0] = curRoadPos[1];
			triangles[1].p[1] = curRoadPos[0];
			triangles[1].p[2] = lastRoadPos[1];

			// 各三角形と当たり判定
			Collision::Result triangleResult = Collision::CheckPointTriangle(planeResult.point, triangles[0]);
			if (triangleResult.hit)
			{
				// 当たった場合、結果を返す
				result.collisionResult = triangleResult;
				result.polyNo = nCntControl;
				return result;
			}

			triangleResult = Collision::CheckPointTriangle(planeResult.point, triangles[1]);
			if (triangleResult.hit)
			{
				// 当たった場合、結果を返す
				result.collisionResult = triangleResult;
				result.polyNo = nCntControl;
				return result;
			}
		}

		// 現在のセグメント位置を次のセグメントの始点に退避
		lastRoadPos[0] = curRoadPos[0];
		lastRoadPos[1] = curRoadPos[1];
	}

	return result; // 当たらなかった場合、初期の結果を返す
}