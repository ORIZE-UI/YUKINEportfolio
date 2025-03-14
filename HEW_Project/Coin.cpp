//Coin.cpp
//2Dのコインについて

#include "Coin.h"
#include "Sprite.h"

//コンストラクタ
Coin::Coin()
	: m_pCFirstTexture(nullptr)
	, m_pCSecondTexture(nullptr)
	, m_pCThirdTexture(nullptr)
	, m_CoinPos(0.0f, 0.0f, 0.0f)

{
	//モデルの読み込み処理
	m_pCFirstTexture = new Texture();
	if (m_pCFirstTexture->Create("Assets/Texture/coin.png"))
	{
		MessageBox(NULL, "コインの読み込みエラー", "Error", MB_OK);
	}

	m_pCSecondTexture = new Texture();
	if (m_pCSecondTexture->Create("Assets/Texture/coin.png"))
	{
		MessageBox(NULL, "コインの読み込みエラー", "Error", MB_OK);
	}

	m_pCThirdTexture = new Texture();
	if (m_pCThirdTexture->Create("Assets/Texture/coin.png"))
	{
		MessageBox(NULL, "コインの読み込みエラー", "Error", MB_OK);
	}

	SetCollect(false);

	m_pPS = new PixelShader();
	if (m_pPS->Load("Assets/Shader/PS_Sprite.cso"))
	{
		MessageBox(NULL, "[Coin.cpp] Failed to load Pixcel Shader", "Error", MB_OK);
	}

	m_EffectCoin = LibEffekseer::Create("Assets/effect/coinGet.efkefc");

}

//デストラクタ
Coin::~Coin()
{
	if (m_pPS)
	{
		delete m_pPS;
		m_pPS = nullptr;
	}
	if (m_pCFirstTexture)
	{
		delete m_pCFirstTexture;
		m_pCFirstTexture = nullptr;
	}
	if (m_pCSecondTexture)
	{
		delete m_pCSecondTexture;
		m_pCSecondTexture = nullptr;
	}
	if (m_pCThirdTexture)
	{
		delete m_pCThirdTexture;
		m_pCThirdTexture = nullptr;
	}
}

//更新
void Coin::Update()
{
	if (IsCoinCollected == true)
	{
		m_EffectHandle = LibEffekseer::GetManager()->Play(m_EffectCoin, m_CoinPos.x,m_CoinPos.y, m_CoinPos.z);
	}

}

//描画
void Coin::Draw(float x, float y, float z, float sizeX, float sizeY, int num)
{

	SetPosition(x, y, z);

	GetSizeX(sizeX);
	GetSizeY(sizeY);


	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(x, y, z);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//プロジェクション行列には2Dとして表示するための行列を設定
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	//スプライトの設定
	Sprite::SetPixelShader(m_pPS);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	//Sprite::SetOffset({ 0.0f, 0.0f });
	Sprite::SetSize(DirectX::XMFLOAT2(sizeX, -sizeY));
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	switch (num)
	{
	case 1:
		Sprite::SetTexture(m_pCFirstTexture);
		break;

	case 2:
		Sprite::SetTexture(m_pCSecondTexture);
		break;

	case 3:
		Sprite::SetTexture(m_pCThirdTexture);
		break;
	}

	Sprite::Draw();
}


void Coin::SetPosition(float x, float y, float z)
{
	m_CoinPos.x = x;
	m_CoinPos.y = y;
	m_CoinPos.z = z;
}

DirectX::XMFLOAT3 Coin::GetPosition() const
{
	return m_CoinPos;
}

float Coin::GetSizeX(float x)
{
	m_CoinSize.x = x;
	return m_CoinSize.x;
}

float Coin::GetSizeY(float y)
{
	m_CoinSize.y = y;
	return m_CoinSize.y;
}

DirectX::XMFLOAT3 Coin::GetSize() const
{
	return m_CoinSize;
}

void Coin::SetCollect(bool CollectFirst)
{
	IsCoinCollected = CollectFirst;
}

bool Coin::GetCollect()
{
	return IsCoinCollected;
}
