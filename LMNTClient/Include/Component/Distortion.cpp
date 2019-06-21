#include "Distortion.h"
#include "Component/Renderer.h"
#include "GameObject.h"
#include "Transform.h"


CDistortion::CDistortion()
{
}


CDistortion::~CDistortion()
{
}

void CDistortion::SetTotalTime(float fTime)
{
	m_fTargetTime = fTime;
}

void CDistortion::SetTargetScale(float fScale)
{
	m_fTargetScale = fScale;	
}

bool CDistortion::Initialize()
{
	return true;
}

int CDistortion::Update(float fTime)
{
	m_fTimer += fTime;	
	
	float fProgress = m_fTimer / m_fTargetTime;

	if (m_fTimer >= m_fTargetTime)
	{
		m_fTimer = 0.0f;
	}

	CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
	if (pRenderer)
	{
		Vector4 vColor = pRenderer->GetDiffuse();
		vColor.w = cosf(PI * 0.5f * fProgress);
		pRenderer->SetDiffuse(vColor);
	}
	SAFE_RELEASE(pRenderer);
	
	m_fScale = fProgress * m_fTargetScale;
	m_pTransform->SetWorldScale(m_fScale, m_fScale, 1.0f);


	if (fProgress >= 1.0f)
	{
		m_pGameObject->Destroy(true);
	}


	return 0;
}
