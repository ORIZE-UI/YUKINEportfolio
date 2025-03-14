#include "spItem.h"
#include "Geometory.h"

const float CSpeedItem::ITEM_RADIUS = 0.75f;

CSpeedItem::CSpeedItem(void)
	: m_pCamera(nullptr)
	, m_fRotY(0.0f)
{
	// インスタンス情報の初期化
	for (auto& rInstance : m_aInstances)
	{
		rInstance.pos = { 0.0f, 0.0f, 0.0f };
		rInstance.isUse = false;
		rInstance.nGroup = -1;
	}

	// モデル情報の読込
	m_pItemModel = new Model();
	if (!m_pItemModel->Load("Assets/Model/lowPolyCar.fbx", 0.07f, Model::XFlip))
	{
		MessageBox(NULL, "ItemModelRead", "Error", MB_OK);
	}

	// シェーダの読込
	m_pModelVS = new VertexShader();
	if (FAILED(m_pModelVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(NULL, "ItemVSRead", "Error", MB_OK);
	}
	m_pItemModel->SetVertexShader(m_pModelVS);

}

CSpeedItem::~CSpeedItem(void)
{
	if (m_pModelVS)
	{
		delete m_pModelVS;
		m_pModelVS = nullptr;
	}

	if (m_pItemModel)
	{
		delete m_pItemModel;
		m_pItemModel = nullptr;
	}
}

void CSpeedItem::Update(void)
{
	// スポーン処理
	for (auto& rGroup : m_spawnPosGroup)
	{
		// グループ内にインスタンスがないならカウントを加算
		if (rGroup.nCntInstance < 1)
		{
			rGroup.nEmptyFrame++;
		}

		// 空白で指定フレームたったらアイテム生成
		if (rGroup.nEmptyFrame > EMPTY_NOSPAWNN_FRAME)
		{
			Spawn(rGroup);
			rGroup.nEmptyFrame = 0;
		}
	}

	// 回転
	m_fRotY += 0.05f;
}

void CSpeedItem::Draw(void)
{
	for (auto& rInstance : m_aInstances)
	{
		// 未使用かチェック
		if (rInstance.isUse == false)
		{
			continue;
		}

		// 描画
		DirectX::XMFLOAT4X4 mat[3];

		// ワールド行列
		DirectX::XMMATRIX move = DirectX::XMMatrixTranslation(
			rInstance.pos.x,
			rInstance.pos.y + 0.7f,
			rInstance.pos.z
		);

		DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(m_fRotY);

		DirectX::XMMATRIX world = rotY * move;

		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

		mat[1] = m_pCamera->GetViewMatrix();
		mat[2] = m_pCamera->GetProjectionMatrix();

		// モデル表示
		//m_pModelVS->WriteBuffer(0, mat);
		//m_pItemModel->Draw();

		Geometory::SetWorld(mat[0]);
		Geometory::SetView(mat[1]);
		Geometory::SetProjection(mat[2]);
		Geometory::DrawBox();
	}
}
void CSpeedItem::AddSpawnGroup(const std::vector<DirectX::XMFLOAT3>& c_rPosGroup)
{
	// グループを追加
	tSpawnGroup newGroup;
	newGroup.positions = c_rPosGroup;
	newGroup.nCntInstance = 0;
	newGroup.nEmptyFrame = 0;
	newGroup.nGroupNo = m_spawnPosGroup.size();

	m_spawnPosGroup.push_back(newGroup);
}

Collision::Result CSpeedItem::CheckCollision(DirectX::XMFLOAT3 targetPos)
{
	// 当たり判定
	Collision::Result result = { false, };

	for (auto& rInstance : m_aInstances)
	{
		// 未使用かチェック
		if (rInstance.isUse == false)
		{
			continue;
		}

		// 衝突判定
		DirectX::XMVECTOR vItemPos = DirectX::XMVectorSet(
			rInstance.pos.x,
			rInstance.pos.y,
			rInstance.pos.z,
			0.0f
		);
		DirectX::XMVECTOR vTgtPos = DirectX::XMVectorSet(
			targetPos.x,
			targetPos.y,
			targetPos.z,
			0.0f
		);
		DirectX::XMVECTOR vRelative = DirectX::XMVector3Length(DirectX::XMVectorSubtract(vTgtPos, vItemPos));
		float fRelative;
		DirectX::XMStoreFloat(&fRelative, vRelative);

		if (fRelative < ITEM_RADIUS)
		{
			// 衝突

			// アイテムの消去
			Destroy(rInstance);

			// 結果を格納
			result.hit = true;
			result.point = targetPos;

			return result;
		}
	}

	return result;
}

void CSpeedItem::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

// 生成
void CSpeedItem::Spawn(tSpawnGroup& rGroup)
{
	for (tInstance& rInstance : m_aInstances)
	{
		// 使用済かチェック
		if (rInstance.isUse == true)
		{
			continue;
		}

		// 生成座標を確定
		int nPosNo = m_rnd() % rGroup.positions.size();

		// 生成
		rInstance.isUse = true;
		rInstance.nGroup = rGroup.nGroupNo;
		rInstance.pos = rGroup.positions[nPosNo];

		// グループのインスタンス数を増やす
		rGroup.nCntInstance++;

		break;
	}
}

void CSpeedItem::Destroy(tInstance& rInstance)
{
	// オブジェクト削除
	rInstance.isUse = false;

	// グループから削除
	m_spawnPosGroup[rInstance.nGroup].nCntInstance--;
}