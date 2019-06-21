#pragma once
#include "Component.h"
#include "../Client.h"

LMNT_USING

enum CRUMBLE_STATE
{
	CS_STANDBY,
	CS_SHAKE,
	CS_FALL,
	CS_FALL_IDLE,
	CS_GENERATION
};

class CCrumble :
	public CComponent
{
private:


private:
	friend class CGameObject;
	CCrumble();
	~CCrumble();

private:
	bool m_bGeneration;
	CRUMBLE_STATE m_eState;
	vector<Vector3> m_vecOriginLocalPosition;
	vector<class CGameObject*> m_vecChildren;
	vector<float> m_vecGravitationalWeight;

	float m_fTimer;
	float m_fAmplitude;
	int m_iGenerationCounter;

private:
	float Gravity(float fElapse);

public:
	bool GetGeneration() const;
	void SetActive();

public:
	bool Initialize() override;
	int Update(float fTime) override;
	int OnCollisionEnter(class CCollider* pThis, class CCollider* pTarget, float fTime) override;
};

