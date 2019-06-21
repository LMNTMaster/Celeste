#pragma once
#include "Component.h"
#include "../Client.h"
#include "Component/Animator.h"
#include "Component/Renderer.h"

LMNT_USING


class CStrawberry :
	public CComponent
{

	enum STRAWBERRY_STATE
	{
		SS_IDLE,
		SS_FOLLOW,
		SS_RETURN,
		SS_GET
	};

private:
	friend class CGameObject;
private:
	CStrawberry();
	~CStrawberry();


private:
	bool m_bInitialized;

	Vector3 m_vOriginPosition;

	BERRY_TYPE m_eStrawberryType;
	STRAWBERRY_STATE m_eStrawberryState;
	bool m_bSafe;
	float m_fSafeTimer;
	float m_fTimer; 
	float m_fParticleTimer;
	CAnimator* m_pAnimator;

	class CGameObject* m_pPlayer;
	float m_fDistance;

public:
	void StartTimer();
	void SetStrawberryType(BERRY_TYPE eType);

public:
	bool Initialize() override;
	int Update(float fTime) override;
	int OnCollisionEnter(class CCollider* pThis, class CCollider* pTarget, float fTime) override;
};

