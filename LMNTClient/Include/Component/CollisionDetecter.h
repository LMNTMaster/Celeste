#pragma once
#include "Component.h"

LMNT_USING
class CCollisionDetecter :
	public CComponent
{
private:
	friend class CGameObject;
	CCollisionDetecter();
	~CCollisionDetecter();

private:
	class CGameObject* m_pTarget;

public:
	void SetTarget(class CGameObject* pTarget);

public:
	bool Initialize() override;
	int OnCollisionEnter(class CCollider* pThis, class CCollider* pTarget, float fTime) override;
	int OnCollisionStay(class CCollider* pThis, class CCollider* pTarget, float fTime) override;
	int OnCollisionExit(class CCollider* pThis, class CCollider* pTarget, float fTime) override;
};

