#include "Dummy.h"
#include "GameObject.h"
#include "Component/Renderer.h"


CDummy::CDummy(const CDummy & component)
{
}

CDummy::CDummy():
	fTimer(0),
	fTimerMax(0.36f)
{
}

CDummy::~CDummy()
{
}

void CDummy::SetRenderer(CRenderer * pRenderer)
{
	m_pRenderer = pRenderer;
}

bool CDummy::Initialize()
{
	return true;
}

int CDummy::Update(float fTime)
{
	fTimer += fTime;
	if (fTimer >= fTimerMax)
	{
		m_pGameObject->Destroy(true);
	}
	else
	{
		Vector4	vColor = m_pRenderer->GetDiffuse();
		vColor.w = 1.0f - fTimer / fTimerMax;
		m_pRenderer->SetDiffuse(vColor);
	}

	return 0;
}
