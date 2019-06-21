#include "CollisionVoxel3D.h"
#include "GameObject.h"
#include "Collider.h"

LMNT_USING

CCollisionVoxel3D::CCollisionVoxel3D()
{
}


CCollisionVoxel3D::~CCollisionVoxel3D()
{
	Clear();
}

void CCollisionVoxel3D::SetKeyPosition(const Vector3 & vKeyPosition)
{
	m_vKeyPosition = vKeyPosition;
}

Vector3 CCollisionVoxel3D::GetKeyPosition() const
{
	return m_vKeyPosition;
}

unordered_map<class CCollider*, class CCollider*>* CCollisionVoxel3D::GetContainedColliders()
{
	return &m_mapContainedCollider;
}

int CCollisionVoxel3D::GetContainedColliderCount() const
{
	return m_mapContainedCollider.size();
}

bool CCollisionVoxel3D::Contain(CCollider * pTarget) const
{
	unordered_map<CCollider*, CCollider*>::const_iterator iter = m_mapContainedCollider.find(pTarget);

	if (iter == m_mapContainedCollider.end())
	{
		return false;
	}
	return true;
}

bool CCollisionVoxel3D::Contain(CGameObject * pTarget) const
{
	list<CComponent*>* Colliders = pTarget->GetComponentsFromType(CT_COLLIDER);

	list<CComponent*>::const_iterator iter;
	list<CComponent*>::const_iterator iterEnd = Colliders->end();

	bool bFind = false;

	for (iter = Colliders->begin(); iter != iterEnd; ++iter)
	{
		CCollider* pTargetAddress = (CCollider*)(*iter);

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
	return bFind;
}

void CCollisionVoxel3D::AddGameObject(CGameObject * pTarget)
{
	CCollider* pCollider = pTarget->GetComponentFromType<CCollider>(CT_COLLIDER);

	unordered_map<CCollider*, CCollider*>::iterator iter = m_mapContainedCollider.find(pCollider);

	if (iter == m_mapContainedCollider.end())
	{
		m_mapContainedCollider.insert(make_pair(pCollider, pCollider));
	}

	SAFE_RELEASE(pCollider);
}

void CCollisionVoxel3D::AddGameObject(CCollider * pTarget)
{
	unordered_map<CCollider*, CCollider*>::iterator iter = m_mapContainedCollider.find(pTarget);

	if (iter == m_mapContainedCollider.end())
	{
		m_mapContainedCollider.insert(make_pair(pTarget, pTarget));
	}
}

void CCollisionVoxel3D::Remove(CGameObject * pTarget)
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

void CCollisionVoxel3D::Remove(CCollider * pTarget)
{
	unordered_map<CCollider*, CCollider*>::iterator iter = m_mapContainedCollider.find(pTarget);

	if (iter == m_mapContainedCollider.end())
	{
		return;
	}
	m_mapContainedCollider.erase(pTarget);
}

void CCollisionVoxel3D::Clear()
{
	m_mapContainedCollider.clear();
}
