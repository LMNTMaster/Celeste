#include "CollisionVoxel2D.h"
#include "GameObject.h"
#include "Collider.h"

LMNT_USING


CCollisionVoxel2D::CCollisionVoxel2D()
{
}


CCollisionVoxel2D::~CCollisionVoxel2D()
{
	Clear();
}

void CCollisionVoxel2D::SetKeyPosition(const Vector2 & vKeyPosition)
{
	m_vKeyPosition = vKeyPosition;	
}

Vector2 CCollisionVoxel2D::GetKeyPosition() const
{
	return m_vKeyPosition;
}

unordered_map<CCollider*, CCollider*>* CCollisionVoxel2D::GetContainedColliders()
{
	return &m_mapContainedCollider;
}

int CCollisionVoxel2D::GetContainedColliderCount() const
{
	return m_mapContainedCollider.size();
}

bool CCollisionVoxel2D::Contain(CCollider * pTarget) const
{
	unordered_map<CCollider*, CCollider*>::const_iterator iter = m_mapContainedCollider.find(pTarget);

	if (iter == m_mapContainedCollider.end())
	{
		return false;	
	}
	return true;
}

bool CCollisionVoxel2D::Contain(CGameObject * pTarget) const
{

	list<CComponent*>* Colliders = pTarget->GetComponentsFromType(CT_COLLIDER);

	list<CComponent*>::const_iterator iterComponent;
	list<CComponent*>::const_iterator iterComponentEnd = Colliders->end();

	bool bFind = false;

	for (iterComponent = Colliders->begin(); iterComponent != iterComponentEnd; ++iterComponent)
	{

		CCollider* pTargetAddress = (CCollider*)(*iterComponent);

		unordered_map<CCollider*, CCollider*>::const_iterator iter = m_mapContainedCollider.find(pTargetAddress);

		if (iter == m_mapContainedCollider.end())
		{
			continue;
		}
		else
		{
			bFind = true;
			break;
		}
	}

	Safe_Release_VecList(*Colliders);
		
	return bFind;
}

void CCollisionVoxel2D::AddGameObject(CGameObject * pTarget)
{
	//CCollider* pCollider = pTarget->GetComponentFromType<CCollider>(CT_COLLIDER);
	//
	//unordered_map<CCollider*, CCollider*>::iterator iter = m_mapContainedCollider.find(pCollider);
	//
	//if (iter == m_mapContainedCollider.end())
	//{
	//	m_mapContainedCollider.insert(make_pair(pCollider, pCollider));
	//}
	//
	//SAFE_RELEASE(pCollider);

	list<CCollider*>::const_iterator iter = pTarget->GetColliderList()->begin();
	list<CCollider*>::const_iterator iterEnd = pTarget->GetColliderList()->end();
	
	for (; iter != iterEnd; ++iter)
	{
		CCollider* pCollider = *iter;
		unordered_map<CCollider*, CCollider*>::iterator iterMap = m_mapContainedCollider.find(pCollider);
	
		if (iterMap == m_mapContainedCollider.end())
		{
			m_mapContainedCollider.insert(make_pair(pCollider, pCollider));
		}
	
		//SAFE_RELEASE(pCollider);
	}
}
void CCollisionVoxel2D::AddGameObject(CCollider * pTarget)
{
	unordered_map<CCollider*, CCollider*>::iterator iter = m_mapContainedCollider.find(pTarget);

	if (iter == m_mapContainedCollider.end())
	{
		m_mapContainedCollider.insert(make_pair(pTarget, pTarget));
	}
}

void CCollisionVoxel2D::Remove(CGameObject * pTarget)
{
	CCollider* pTargetCollider = pTarget->GetComponentFromType<CCollider>(CT_COLLIDER);

	unordered_map<CCollider*, CCollider*>::iterator iter = m_mapContainedCollider.find(pTargetCollider);

	if (iter == m_mapContainedCollider.end())
	{
		return;
	}

	SAFE_RELEASE(pTargetCollider);

	m_mapContainedCollider.erase(pTargetCollider);

}
void CCollisionVoxel2D::Remove(CCollider * pTarget)
{
	unordered_map<CCollider*, CCollider*>::iterator iter = m_mapContainedCollider.find(pTarget);

	if (iter == m_mapContainedCollider.end())
	{
		return;
	}
	m_mapContainedCollider.erase(pTarget);
}

void CCollisionVoxel2D::Clear()
{
	m_mapContainedCollider.clear();
}