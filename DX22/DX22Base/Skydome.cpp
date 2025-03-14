#include "Skydome.h"

Skydome::Skydome()
	:m_pCamera(nullptr) // カメラの初期化
{
	// モデル読み込み
	m_pModel = new Model();
	if (!m_pModel->Load("Assets/Model/sky.obj", 1.0f, Model::XFlip))
	{
		MessageBox(nullptr, "sky.fbx", "Error", MB_OK); // モデルロード失敗時のエラーメッセージ
	}

	// 頂点シェーダのロード
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBoxA(nullptr, "VS_Model.cso", "Error", MB_OK); // シェーダロード失敗時のエラーメッセージ
	}
	m_pModel->SetVertexShader(m_pVS); // モデルにシェーダを設定
}

Skydome::~Skydome() 
{
	// リソース解放
	delete m_pModel;
	delete m_pVS;
}

//描画処理
void Skydome::Draw(void)
{
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(0.0f);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(100.0f, 100.0f, 100.0f);
	DirectX::XMMATRIX move = DirectX::XMMatrixTranslation(0.0f, 70.0f, 0.0f);

	DirectX::XMMATRIX world = scale * rotY * move;

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();

	m_pVS->WriteBuffer(0, mat);
	m_pVS->Bind();

	m_pModel->Draw();
}

void Skydome::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}