#include "Death.h"
#include "Transform.h"
#include "GameObject.h"


CDeath::CDeath():
	m_fTimer(0)
{
}


CDeath::~CDeath()
{
}

void CDeath::AddBlob(CGameObject * pTarget)
{
	m_vecBlob.push_back(pTarget);		
}

bool CDeath::Initialize()
{
	return true;
}

int CDeath::Update(float fTime)
{
	m_fTimer += fTime;

	float fProgress = m_fTimer / 1.0f;
		
	m_pTransform->SetWorldRotation(0.0f, 0.0f, PI / 4 * fProgress);
	Vector3 position = m_pTransform->GetWorldPosition();

	for (int i = 0; i < m_vecBlob.size(); ++i)
	{
		float fBlobAngle = PI / 4.0f * i + PI / 4 * fProgress;
		float fDistance = 100.0f * sinf(fProgress * PI / 2.0f);
		CGameObject* pCurrent = m_vecBlob[i];
		CTransform* pTransform = pCurrent->GetTransform();
		pTransform->SetWorldScale(sinf(PI * fProgress) * 50.0f , sinf(PI * fProgress) * 50.0f,1.0f);
		pTransform->SetWorldPosition(position.x + cosf(fBlobAngle) * fDistance, position.y + sinf(fBlobAngle) * fDistance, 0.0f);
		SAFE_RELEASE(pTransform);
	}

	if (m_fTimer >= 1.0f)
	{
		for (int i = 0; i < m_vecBlob.size(); ++i)
		{
			m_vecBlob[i]->Destroy(true);
		}
	
		m_pGameObject->Destroy(true);
	}

	return 0;
}
