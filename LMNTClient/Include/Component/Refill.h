#pragma once
#include "Component.h"
#include "../Client.h"
#include "Component/Animator.h"
#include "Component/Renderer.h"

LMNT_USING

class CRefill :
	public CComponent
{
private:
	enum REFILL_STATE
	{
		RS_IDLE,
		RS_DISABLE,
		RS_REGENERATION
	};


private:
	friend class CGameObject;
	CRefill();
	~CRefill();

private:
	REFILL_STATE m_eState;
	bool m_bInitialized;
	float m_fTimer;
	float m_fParticleTimer;

	float m_fRegenerationTimer;
	bool m_bDisable;

	float m_fScalingTimer;	
	float m_fLocalScale;
	float m_fStartAmplitude;
	float m_fFrequency;

	Vector3 m_vOriginPosition;

	CRenderer* m_pBaseRenderer;
	CRenderer* m_pEffectRenderer;
	CAnimator* m_pBaseAnimator;
	CAnimator* m_pEffectAnimator;
	
private:
	void SetDisable();
	void Regeneration();

public:
	bool Initialize() override;
	int Update(float fTime) override;
	int OnCollisionEnter(class CCollider* pThis, class CCollider* pTarget, float fTime) override;



};

