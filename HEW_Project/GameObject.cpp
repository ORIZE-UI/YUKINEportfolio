#include "GameObject.h"

GameObject::GameObject()
	
{
}

GameObject::~GameObject()
{
}

bool GameObject::IsCollidingWith(GameObject& other)
{
	//Collision“à‚Ì“–‚½‚è”»’è‚ÌŠÖ”‚ğ•Ô‚·
	return m_aabb.IsAABBIntersecting(minBound, maxBound, other.minBound, other.maxBound);
}

bool GameObject::HIsCollidingWith(GameObject& other)
{
	//Collision“à‚Ì“–‚½‚è”»’è‚ÌŠÖ”‚ğ•Ô‚·
	return m_haabb.HIsAABBIntersecting(hminBound, hmaxBound, other.hminBound, other.hmaxBound);
}

bool GameObject::col(GameObject & other)
{
	return m_caabb.col(cminBound, cmaxBound, other.cminBound, other.cmaxBound);
}

bool GameObject::IsCollisionBoxTriangle(GameObject & other)
{
	return m_aabb.IntersectAABBTriangle(minBound, maxBound, other.v0, other.v1, other.v2);
}

bool GameObject::IsCollisionPoint(GameObject & other)
{
	return m_aabb.IsPointInsideAABB(other.points, minBound, maxBound);
}

DirectX::XMFLOAT3 GameObject::GetCMinBounds()
{
	return cminBound;
}

DirectX::XMFLOAT3 GameObject::GetMaxBounds()
{
	return cmaxBound;
}




