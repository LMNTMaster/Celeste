#include "Bird.h"
#include "Timer.h"
#include "TimerManager.h"
#include "Transform.h"
#include "../Client.h"

CBird::CBird():
	m_eState(BS_NORMAL)
{
	m_eComponentType = CT_BIRD;
	SetTag("Bird");
}


CBird::~CBird()
{
}

void CBird::SetAnimationController(CAnimationController * pController)
{
	m_pAnimationController = pController;
}

void CBird::Landing()
{
}

void CBird::FlyAway()
{
	m_eState = BS_FLY_A;
	m_fFlyTimer = 0.0f; 
	m_vOriginPosition = m_pTransform->GetWorldPosition();
}

void CBird::Jump(float fFixedDeltaTime)
{
	m_fFlyTimer += fFixedDeltaTime;

	float fDeltaX = sinf(PI / 1.0f * m_fFlyTimer) * 100.0f;
	float fDeltaY = sinf(PI / 2.0f* m_fFlyTimer) * 200.0f;

	m_pTransform->SetWorldPosition(m_vOriginPosition + Vector3(fDeltaX, -fDeltaY, 0.0f));

	if (m_fFlyTimer >= 0.5f)
	{
		m_eState = BS_FLY_B;
	}
}

bool CBird::Initialize()
{
	return true;
}

int CBird::Update(float fTime)
{
	float fFixedDeltaTime = GET_SINGLE(CTimerManager)->FindTimer("MainTimer")->GetFixedDeltaTime();

	m_pTransform->SetPivot(0.5f, 1.0f, 0.0f);

	if (m_eState == BS_NORMAL)
	{
		float fFrame = m_pAnimationController->GetCurrentState()->GetClip()->GetCurrentFrameCount();
		bool bEnd;		
		bEnd = fFrame + 1 >= m_pAnimationController->GetCurrentState()->GetClip()->GetCustomFrameCount();	

		if (bEnd)
		{
			int iRand = rand() % 10;

			if (iRand < 8 )
			{
				m_pAnimationController->TransitionState("Bird Idle");
			}
			if ( iRand >= 8)
			{
				m_pAnimationController->TransitionState("Bird Peck");
			}
		}
	}
	if (m_eState == BS_LANDING)
	{
	}

	if (m_eState == BS_FLY_A)
	{
		//Jump(fFixedDeltaTime);
	}

	if (m_eState == BS_FLY_B)
	{
	}

	return 0;
}
