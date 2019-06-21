#include "Animator.h"
#include "../AnimationController.h"
#include "../AnimationState.h"
#include "../AnimationClip2D.h"
#include "../GameObject.h"
#include "../Component.h"
#include "Renderer.h"
#include "../Material.h"
#include "../Transform.h"
LMNT_USING

CAnimator::CAnimator():
	m_pRenderer(NULL),
	m_bSyncScale(false),
	m_vScaleFactor(Vector3::One),
	m_vLocalScaleFactor(Vector3::One)
{
	m_eComponentType = CT_ANIMATOR;
	SetTag("Animator");
}

CAnimator::~CAnimator()
{
	SAFE_DELETE(m_pAnimationController);
	SAFE_RELEASE(m_pRenderer);
}
void CAnimator::SetTargetRenderer(CRenderer * pRenderer)
{
	m_pRenderer = pRenderer;
}
const Vector3 & CAnimator::GetLocalScaleFactor() const
{
	return m_vLocalScaleFactor;
}
const Vector3& CAnimator::GetFixedScaleFactor() const
{
	return m_vScaleFactor;
}
const Vector3& CAnimator::GetOriginScale() const
{
	return m_vOriginScale;
}
void CAnimator::SetFixedScaleFactor(const Vector3& vFactor)
{
	m_vScaleFactor = vFactor;
}
void CAnimator::SetLocalScaleFactor(const Vector3 & vFactor)
{
	m_vLocalScaleFactor = vFactor;
}
bool CAnimator::GetSyncScale() const
{
	return m_bSyncScale;
}
void CAnimator::SetSyncScale(bool bUseSyncScale)
{
	m_bSyncScale = bUseSyncScale;
}

void CAnimator::SetAnimationStateTransitioned()
{
	m_bAnimationStateTransitioned = true;
}
void CAnimator::SetAnimationController(CAnimationController * pController)
{
	m_pAnimationController = pController;
	pController->m_pAnimator = this;
}

void CAnimator::SetEntryState(const string & strStateName)
{
	if (m_pAnimationController)
	{
		m_pAnimationController->SetEntryState(strStateName);
	}
}

class CAnimationState* CAnimator::CreateAnimationStateInCurrentController(const string & strStateName)
{
	if (m_pAnimationController)
	{
		return m_pAnimationController->CreateAnimationState(strStateName);
	}	

	return NULL;
}

void CAnimator::CreateAnimationController()
{
	CAnimationController* pController = new CAnimationController;
	SetAnimationController(pController);
}

CAnimationController * CAnimator::GetAnimationController() const
{
	return m_pAnimationController;
}

bool CAnimator::Initialize()
{
	CTransform* pTransform = m_pGameObject->GetTransform();
	m_vOriginScale = pTransform->GetWorldScale();
	SAFE_RELEASE(pTransform);
	return true;
}

int CAnimator::Update(float fTime)
{
	m_pAnimationController->Update(fTime);

	if (!m_pRenderer)
	{
		m_pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
	}


	if (m_bAnimationStateTransitioned)
	{
		m_bAnimationStateTransitioned = false;
		CMaterial*	pMaterial = m_pRenderer->GetMaterial();
		pMaterial->SetDiffuseTexture(m_pAnimationController->m_pCurrentState->GetClip()->m_pTexture);
		pMaterial->SetDiffuseTextureRegister(m_pAnimationController->m_pCurrentState->GetClip()->m_iTextureRegister);
		SAFE_RELEASE(pMaterial);
	}

	PANIMATION2DCBUFFER	pBuffer = m_pAnimationController->m_pCurrentState->GetClip()->GetAnimation2DCBuffer();
	m_pRenderer->UpdateRendererConstantBuffer(ANIMATION2D_CBUFFER, pBuffer);


	return 0;
}

