#include "CollisionDetecter.h"
#include "GameObject.h"


CCollisionDetecter::CCollisionDetecter()
{
}


CCollisionDetecter::~CCollisionDetecter()
{
}

void CCollisionDetecter::SetTarget(CGameObject * pTarget)
{
}

bool CCollisionDetecter::Initialize()
{
	return true;
}

int CCollisionDetecter::OnCollisionEnter(CCollider * pThis, CCollider * pTarget, float fTime)
{
	m_pGameObject->OnCollisionEnter(pThis, pTarget, fTime);
	return 0;
}

int CCollisionDetecter::OnCollisionStay(CCollider * pThis, CCollider * pTarget, float fTime)
{
	m_pGameObject->OnCollisionStay(pThis, pTarget, fTime);
	return 0;
}

int CCollisionDetecter::OnCollisionExit(CCollider * pThis, CCollider * pTarget, float fTime)
{
	m_pGameObject->OnCollisionExit(pThis, pTarget, fTime);
	return 0;
}
