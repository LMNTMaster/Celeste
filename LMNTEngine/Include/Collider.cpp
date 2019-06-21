#include "Collider.h"
#include "Shader.h"
#include "Mesh.h"
#include "Device.h"
#include "GameObject.h"
#include "Component\RectCollider2D.h"
#include "Component\CircleCollider2D.h"
#include "CollisionVoxel2D.h"
#include "CollisionVoxel3D.h"
#include "Scene.h"
#include "Layer.h"
#include "TImerManager.h"
#include "Timer.h"

LMNT_USING


CCollider::CCollider()
{
	m_eComponentType = CT_COLLIDER;

}

CCollider::CCollider(const CCollider & component) :
	CComponent(component)
{
	m_eComponentType = CT_COLLIDER;
	m_vPivot = component.m_vPivot;
#ifdef _DEBUG

	m_pShader = component.m_pShader;
	m_pMesh = component.m_pMesh;
	m_pLayout = component.m_pLayout;


	if (m_pShader)
		m_pShader->AddRef();

	if (m_pMesh)
		m_pMesh->AddRef();

#endif // _DEBUG

}

CCollider::~CCollider()
{
	{
		unordered_map<CCollider*, CCollider*>::iterator iter;
		unordered_map<CCollider*, CCollider*>::iterator iterEnd = m_mapCollision.end();
		for (iter = m_mapCollision.begin(); iter != iterEnd;++iter)
		{
			CGameObject* pTarget = iter->second->GetGameObject();
			pTarget->OnCollisionExit(this, NULL, 0);	
			(*iter).second->RemoveTargetFromCollisionList(this);
	
			SAFE_RELEASE(pTarget);
		}
	}

    m_pScene->RemoveCollider(this);
	{
		list<CCollisionVoxel2D*>::iterator iter;
		list<CCollisionVoxel2D*>::iterator iterEnd = m_CollisionVoxel2DList.end();

		for (iter = m_CollisionVoxel2DList.begin(); iter != iterEnd; ++iter)
		{
			(*iter)->Remove(this);
		}	
	}

	{
		list<CCollisionVoxel3D*>::iterator iter;
		list<CCollisionVoxel3D*>::iterator iterEnd = m_CollisionVoxel3DList.end();

		for (iter = m_CollisionVoxel3DList.begin(); iter != iterEnd; ++iter)
		{
			(*iter)->Remove(this);
		}
	}

#ifdef _DEBUG
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pMesh);
#endif // _DEBUG
}

COLLIDER_TYPE CCollider::GetColliderType() const
{
	return m_eColliderType;
}

Vector3 CCollider::GetPivot() const
{
	return m_vPivot;
}


Vector3 CCollider::GetScale() const
{
	return m_vScale;
}

void CCollider::Straighten()
{
	float fDeltaTime = GET_SINGLE(CTimerManager)->FindTimer("MainTimer")->GetDeltaTime();
	
	unordered_map<CCollider*, CCollider*>::iterator iter;
	unordered_map<CCollider*, CCollider*>::iterator iterEnd = m_mapCollision.end();
	for (iter = m_mapCollision.begin(); iter != iterEnd; )
	{
		if (!CollisionCheck((iter)->second))
		{			
			CGameObject* pTargetObject = iter->second->GetGameObject();
			pTargetObject->OnCollisionExit(iter->second, this, fDeltaTime);
			SAFE_RELEASE(pTargetObject);

			m_pGameObject->OnCollisionExit(this, iter->second, fDeltaTime);
			iter->second->RemoveTargetFromCollisionList(this);
			iter = m_mapCollision.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CCollider::SetPivot(const Vector3 & vPivot)
{
	m_vPivot = vPivot;
}

void CCollider::SetPivot(float x, float y, float z)
{
	m_vPivot.x = x;
	m_vPivot.y = y;
	m_vPivot.z = z;
}

void CCollider::SetScale(const Vector3 & vScale)
{
	m_vScale = vScale;
}

void CCollider::SetScale(float x, float y, float z)
{
	m_vScale.x = x;
	m_vScale.y = y;
	m_vScale.z = z;
}

bool CCollider::Initialize()
{
	m_pScene->AddCollider(this);
	return true;
}

int CCollider::Update(float fTime)
{
	return 0;
}

int CCollider::LateUpdate(float fTime)
{
	return 0;
}

int CCollider::OnCollisionEnter(CCollider* pThis, CCollider * pTarget, float fTime)
{
	return 0;
}

int CCollider::OnCollisionStay(CCollider* pThis, CCollider * pTarget, float fTime)
{
	return 0;
}

int CCollider::OnCollisionExit(CCollider* pThis, CCollider * pTarget, float fTime)
{
	return 0;
}

void CCollider::Render(float fTime)
{
#ifdef _DEBUG
	DEVICE_CONTEXT->IASetInputLayout(m_pLayout);
	m_pShader->SetShader();
	m_pMesh->Render();
#endif // _DEBUG
}



void CCollider::ClearCollisionVoxel()
{
	m_CollisionVoxel2DList.clear();
	m_CollisionVoxel3DList.clear();
}

size_t CCollider::GetCollisionCount() const
{
	return m_mapCollision.size();
}

void CCollider::AddCollisionList(CCollider * pTarget)
{
	unordered_map<CCollider*, CCollider*>::iterator iter = m_mapCollision.find(pTarget);

	if (iter == m_mapCollision.end())
	{
		m_mapCollision.insert(make_pair(pTarget, pTarget));
	}
}

void CCollider::RemoveTargetFromCollisionList(CCollider * pTarget)
{
	unordered_map<CCollider*, CCollider*>::iterator iter = m_mapCollision.find(pTarget);

	if (iter != m_mapCollision.end())
	{
		m_mapCollision.erase(iter);
	}
}

void CCollider::ClearCollisionList()
{
	m_mapCollision.clear();
}

bool CCollider::ContainCollision(CCollider * pTarget) const
{
	unordered_map<CCollider*, CCollider*>::const_iterator iter = m_mapCollision.find(pTarget);

	if (iter == m_mapCollision.end())
	{
		return false;
	}

	return true;
}


void CCollider::AddCollisionVoxel(CCollisionVoxel2D * pVoxel)
{
}

void CCollider::AddCollisionVoxel(CCollisionVoxel3D * pVoxel)
{
}

void CCollider::AddCollisionVoxel(const Vector2 & vVoxelKey)
{
}

void CCollider::AddCollisionVoxel(const Vector3 & vVoxelKey)
{
}

void CCollider::CalculateCollision2D(float fTime)
{
}

void CCollider::CalculateCollision3D(float fTime)
{
}

bool CCollider::CollisionCheck(CCollider* pTarget) const
{
	COLLIDER_TYPE eTargetColliderType = pTarget->GetColliderType();

	switch (m_eColliderType)
	{
	case LMNT::CT_RECT:
	{
		switch (eTargetColliderType)
		{
		case LMNT::CT_RECT:
		{
			return ((CRectCollider2D*)this)->CollisionCheckWithRectCollider(pTarget, true);
		}
			break;
		case LMNT::CT_CIRCLE:
		{
			return ((CRectCollider2D*)this)->CollisionCheckWithCircleCollider(pTarget);
		}
			break;
		default:
			break;
		}
	}
		break;
	case LMNT::CT_CIRCLE:
	{
		switch (eTargetColliderType)
		{
		case LMNT::CT_RECT:
		{
			return ((CCircleCollider2D*)this)->CollisionCheckWithRectCollider(pTarget);
		}
		break;
		case LMNT::CT_CIRCLE:
		{
			return ((CCircleCollider2D*)this)->CollisionCheckWithCircleCollider(pTarget);
		}
		break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
}

CCollider* CCollider::GetColliderInBegin() const
{
	return m_mapCollision.begin()->second;
}
