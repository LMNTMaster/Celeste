#pragma once
#include "Component.h"
#include "AnimationController.h"
#include "AnimationState.h"
#include "AnimationClip2D.h"

LMNT_USING

enum BIRD_STATE
{
	BS_NORMAL,
	BS_LANDING,
	BS_FLY_A,
	BS_FLY_B
};

class CBird :
	public CComponent
{
private:
	friend class CGameObject;

	CBird();
	~CBird();

private:
	BIRD_STATE m_eState;
	class CAnimationController* m_pAnimationController;
	float m_fFlipTimer;
	float m_fFlyTimer;
	Vector3 m_vOriginPosition;

public:
	void SetAnimationController(class CAnimationController* pController);
	void Landing();
	void FlyAway();
	void Jump(float fFixedDeltaTime);

public:
	bool Initialize() override;
	int Update(float fTime) override;


};

