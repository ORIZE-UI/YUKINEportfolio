#include "ItemUI.h"
#include "Sprite.h"
#include <DirectXMath.h>

ItemUI::ItemUI()
{
	/*m_pTexture = new Texture;
	if (FAILED(m_pTexture->Create("Assets/TexTure/siro.png")))
	{
		MessageBox(NULL, "ItemUI siro.png", "ERROR", MB_OK);
	}*/
}

ItemUI::~ItemUI()
{
	//if (m_pTexture)
	//{
	//	delete m_pTexture;
	//	m_pTexture = nullptr;
	//}
}

void ItemUI::Update()
{
}

void ItemUI::Draw()
{
	//DirectX::XMFLOAT4X4 mat[3];

	////ワールド行列はXとYのみ考慮
	//DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(1080.0f, 520.0f, 0.0f);
	//DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	////ビュー行列は2Dだとカメラの位置があまり関係ないので、単位行列を設定する
	//DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	////プロジェクション行列には2Dとして表示するための行列を設定する
	////コン行列で2Dのスクリーンの大きさが決まる
	//DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH
	//						(0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	//DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	////スプライトの設定
	//Sprite::SetWorld(mat[0]);
	//Sprite::SetView(mat[1]);
	//Sprite::SetProjection(mat[2]);
	//Sprite::SetSize(DirectX::XMFLOAT2(300.0f, -300.0f));
	//Sprite::SetTexture(m_pTexture);
	//Sprite::Draw();
}
