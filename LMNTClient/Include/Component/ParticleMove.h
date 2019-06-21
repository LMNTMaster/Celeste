#pragma once
#include "Component.h"
#include "ParticleManager.h"

LMNT_USING

class CParticleMove :
	public CComponent
{
private:
	friend class CGameObject;
public:
	CParticleMove();
	CParticleMove(const CParticleMove& component);
	~CParticleMove(); 
	CParticleMove* Clone() const	override;

private:	
	class CParticleManager* m_pManager;
	Vector3 m_vTargetPosition;
	Vector3 m_vSpawnPosition;
	Vector3 m_vPivotPosition;

	bool m_bUseSnow;

	float m_fTimer;
	
	float m_fFadeTimer;
	bool m_bFading;
	bool m_bUseFadeout;

	bool m_bUseSpeedReduce;


	float m_fMoveSpeed;
	float m_fEffectiveMoveSpeed;
	float m_fFrequency;
	float m_fAmplitude;

public:
	void UseSnow();
	void SetManager(class CParticleManager* pManager);
	void SetReduceSpeed(bool bUse);
	void SetFadeout(bool bUse);
	void SetTargetPosition(const Vector3& vPosition);
	void SetMoveSpeed(float fSpeed);
	void SetFrequency(float fSpeed);
	void SetAmplitude(float fDistance);


	bool Initialize() override;
	int Update(float fTime) override;

};



