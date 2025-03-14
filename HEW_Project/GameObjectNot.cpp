// 憑依できないブロックの当たり判定を作成するプログラム
#include "GameObjectNot.h"

GameObjectNot::GameObjectNot()

{
}

GameObjectNot::~GameObjectNot()
{
}

bool GameObjectNot::IsNotCollidingWith(GameObjectNot& other)
{
	//Collision内の当たり判定の関数を返す
	return m_aabb.IsAABBIntersecting(minBound, maxBound, other.minBound, other.maxBound);
}

bool GameObjectNot::HIsNotCollidingWith(GameObjectNot& other)
{
	//Collision内の当たり判定の関数を返す
	return m_haabb.HIsAABBIntersecting(hminBound, hmaxBound, other.hminBound, other.hmaxBound);
}

bool GameObjectNot::colNot(GameObjectNot& other)
{
	return m_caabb.col(hminBound, hmaxBound, other.hminBound, other.hmaxBound);
}