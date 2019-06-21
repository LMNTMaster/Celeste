#pragma once
#include "Component.h"
#include "../Client.h"
LMNT_USING

//FallBlock 은 단순히 중력 방향으로만 편방향 이동이고
//플레이어와의 접족으로만 작동한다.

enum FALL_BLOCK_STATE
{
	FBS_STANDBY,
	FBS_SHAKE,
	FBS_FALL,
	FBS_FALL_IDLE
};

class CFallBlock :
	public CComponent
{
private:
	friend class CGameObject;
	CFallBlock();	
	CFallBlock(const CFallBlock& component);
	~CFallBlock();

private:
	bool m_bFixedCollider;
	class CGameObject* m_pPlayer;
	Vector3 m_vOriginColliderLocalPosition;
	Vector3 m_vOriginPosition;
	FALL_BLOCK_STATE m_eState;
	bool m_bInteractionWithTerrain;
	float m_fShakeTime;
	float m_fShakeTimer;
	float m_fAmplitude;
	float m_fFrequency;

	float m_fFallTimer;
	float m_fFallSpeed;
	float m_fGravity;
	bool m_bUseCameraShake;

	bool m_bSoundPlayAtShake;
	bool m_bSoundPlayAtRelease;

private:
	float Gravity(float fElapseTime);

public:
	void SetCameraShake(bool bShake);
	void SetColliderFixed(bool bFixed);
	void InteractionWithTerrain(bool bInteraction);
	void SetShakeTime(float fTime);
	void SetFallBlockState(FALL_BLOCK_STATE eState);
	FALL_BLOCK_STATE GetFallBlockState() const;
	
public:
	void Reset();
public:
	bool Initialize() override;
	int Update(float fTime) override;
	int OnCollisionEnter(class CCollider* pThis, class CCollider* pTarget, float fTime) override;
	int OnCollisionExit(class CCollider* pThis, class CCollider* pTarget, float fTime) override;
};

