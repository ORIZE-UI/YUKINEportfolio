#include "2DObjectManager.h"
#include <iostream>
#include <vector>
#include "Input.h"



//InputManager imanagerSP = InputManager();

#define _shadow_ (0)


#if _shadow_ >= 1
const int width = 200;
const int height = 50;
#else
const int width = 1;
const int height = 1;
#endif

Object2D::Object2D()
	: m_pSObjects(nullptr)
	, m_pPlayer(nullptr)
	, m_num(0)
	, m_IsUse(false)
{
	m_pObjectCamera = new CameraDebug();
	aabb = new AABB();
	//m_pPlayer = new Player();
	m_pShadowP = new ShadowP();
	//struct Setting
	//{
	//	float x, y, z, scaleX, scaleY, scaleZ;
	//};
	//Setting data[] = {
	//	{ 2.0f, 0.0f, 0.0f, 10.0f, 30.0f, 10.0f},
	//	{-2.0f, 0.0f, 0.0f, 10.0f, 30.0f, 10.0f},
	//};
	//配列の要素の数から必要なブロック数を計算
	//必要な数だけブロックを確保
	//m_num = width * height;
	//確保したブロックに初期データを設定
	/*for (int i = 0; i < m_num; i++)
	{
		m_pObjects[i].Create(
			data[i].x, data[i].y, data[i].z,
			data[i].scaleX, data[i].scaleY, data[i].scaleZ
		);
	}*/


	//const int width = 308;
	//const int height = 100;
	//std::vector<std::vector<int>> data(height, std::vector<int>(width, 0));
	//for (int i = 0; i < height; i++)
	//{
	//	for (int j = 0; j < width; j++)
	//	{
	//		data[i][0] = 1;
	//		data[width][j] = 1;
	//		if (j > width - 5)
	//		{
	//			data[j][i] = 1;
	//		}
	//	}
	//}
	//data[0][0] = 1;
	//data[1][1] = 1;
	m_num = height * width;
	m_pSObjects = new SObject[m_num];
	int nCnt = 2;
	int data[height][width];
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			// 1024と600の倍数を2次元配列に配置
			data[i][j] = 0;
		}
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i + 1 > height - 1)
			{
				data[i][j] = 1;
			}
			data[i][0] = 2;
			if (j == width - 1)
			{
				data[i][j] = 2;
			}
		}
	}
	for (int i = 10; i < height - 1; i++)
	{
		for (int j = 199; j > width - nCnt; j--)
		{
			data[i][j] = 1;

		}
		nCnt++;
	}


	//ステージデータの読み取り
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			//セルのデータに基づいてブロック生成
			switch (data[j][i])
			{
			default:
			case 0:
				//ブロックの生成を行わない
				break;
			case 1:
				//m_pSObjects = new SObject[j * width + i];
				/*m_pSObjects[j * width + i].Create(
					-5.0 + i * 0.00975, -j * 0.008 + 0.5f, 10.0f,
					0.0098f, 0.008f, 0.005f
				);*/
				m_pSObjects[j * width + i].Create(-5.0f + i * 0.00975f, -j * 0.008f + 0.5f, 10.0f, 0.0098f, 0.008f, 0.05f, "Assets/Model/Block/blc.fbx", 0.05f, Model::Flip::XFlip);				//m_pSObjects[j * width + i].Create(-5.0 + i * 0.00975, -j * 0.008 + 0.5f, 10.0f, 0.0098f, 0.008f, 0.05f, "Assets/Model/Block/blc.fbx", 0.05f, Model::Flip::XFlip);
				//break;
				//break;
				break;
			case 2:
				m_pSObjects[j * width + i].CreateBounds(-5.0f + i * 0.00975f, -j * 0.008f + 0.5f, 10.0f, 0.0098f, 0.008f, 0.05f, "Assets/Model/Block/blc.fbx", 0.05f, Model::Flip::XFlip);				//m_pSObjects[j * width + i].Create(-5.0 + i * 0.00975, -j * 0.008 + 0.5f, 10.0f, 0.0098f, 0.008f, 0.05f, "Assets/Model/Block/blc.fbx", 0.05f, Model::Flip::XFlip);

				break;
			}
		}
	}

}

Object2D::~Object2D()
{
	delete[] m_pSObjects;
	if (m_pObjectCamera)
	{
		delete m_pObjectCamera;
		m_pObjectCamera = nullptr;
	}
	if (aabb)
	{
		delete aabb;
		aabb = nullptr;
	}
	//if (m_pPlayer)
	//{
	//	delete m_pPlayer;
	//	m_pPlayer = nullptr;
	//}
	if (m_pShadowP)
	{
		delete m_pShadowP;
		m_pShadowP = nullptr;
	}
}

void Object2D::Update()
{
	//m_pPlayer->Update();
	m_pShadowP->Update();
	DirectX::XMFLOAT3 PPos = m_pShadowP->NowPos();
	for (int i = 0; i < m_num; i++)
	{
		DirectX::XMFLOAT3 PPos = m_pShadowP->NowPos();
		m_pSObjects[i].Update();
		DirectX::XMFLOAT3 OPos = m_pSObjects[i].Pos();
		if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pSObjects[i])) {
			if (m_pShadowP->IsCollidingWith(*gameObject)) {
				// 衝突時の処理
				//MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
				m_pShadowP->ShadowPPosY();
			}
		}
		if (PPos.y < OPos.y && PPos.x > -4.0f)
		{
			if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pSObjects[i])) {
				if (m_pShadowP->IsCollidingWith(*gameObject)) {
					// 衝突時の処理
					//m_pShadowP->Jump();
					m_pShadowP->SetPos(PPos.x, OPos.y + 0.05, PPos.z);
					break;
					//MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
					//m_pShadowP->ShadowPPosY();
				}
			}
		}
		
		if (m_pSObjects[i].IBound() == true)
		{
			if (m_pSObjects[i].IBound() == true && m_pShadowP->isUse() == false)
			{
				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pSObjects[i])) {
					if (m_pShadowP->IsCollidingWith(*gameObject)) {
						// 衝突時の処理
						//MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
						/*m_pShadowP->NotUse();*/
						m_pShadowP->oldX();
						m_pShadowP->ShadowPPosY();
						m_pShadowP->Use();
						break;
						//m_pShadowP->ShadowPPos();						
						//m_pShadowP->Use();
					}
				}
			}

			else if (m_pSObjects[i].IBound() == true && m_pShadowP->isUse())
			{
				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pSObjects[i])) {
					if (m_pShadowP->IsCollidingWith(*gameObject)) {
						// 衝突時の処理
						//MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
						//m_pShadowP->Use();
						m_pShadowP->oldX();
						m_pShadowP->ShadowPPosY();
						//m_pShadowP->ShadowPPos();
						m_pShadowP->NotUse();
						break;
						//m_pShadowP->Use();
					}
				}
			}
		}
		

	}



	/*imanagerSP.addKeycode(0, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_Y);

	imanagerSP.addKeycode(1, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_X);

	imanagerSP.inspect();
*/
	/*float AB = static_cast<float>(imanagerSP.getKey(0));

	float AA = static_cast<float>(imanagerSP.getKey(1));
*/
	if (IsKeyPress('O'))//imanagerSP.getKey(0) & 0b011)
	{
		if (!m_IsUse)
		{
			m_IsUse = true;
		}
		
	}

	if (IsKeyPress('P'))//imanagerSP.getKey(1) & 0b011)
		if (m_IsUse)
	{
		m_IsUse = false;
	}
}





void Object2D::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	if (m_IsUse)
	{
		for (int i = 0; i < m_num; i++)
		{
			m_pSObjects[i].Draw(viewMatrix, projectionMatrix);
		}
	}
	DirectX::XMFLOAT4X4 mat[3];

	mat[1] = m_pObjectCamera->GetViewMatrix();
	mat[2] = m_pObjectCamera->GetProjectionMatrix();
	//DirectX::XMFLOAT4X4 viewMatrix = m_pObjectCamera->GetViewMatrix();
	//DirectX::XMFLOAT4X4 projectionMatrix = m_pObjectCamera->GetProjectionMatrix();

	//m_pPlayer->Draw(viewMatrix, projectionMatrix);
	m_pShadowP->Draw(viewMatrix, projectionMatrix);
}
