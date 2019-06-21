#include "Crumble.h"
#include "Transform.h"
#include "GameObject.h"
#include "Collider.h"
#include "Component/RectCollider2D.h"
#include "Component/Renderer.h"



CCrumble::CCrumble():
	m_eState(CS_STANDBY),
	m_fAmplitude(3.0f),
	m_fTimer(0.0f),
	m_bGeneration(true)
{
	m_eComponentType = CT_CRUMBLE;
	SetTag("FallBlock");
}


CCrumble::~CCrumble()
{
}

float CCrumble::Gravity(float fElapse)
{
	return (448.0f * (-2.0f * fElapse)) / 0.4225f;
}

bool CCrumble::GetGeneration() const
{
	return m_bGeneration;
}

void CCrumble::SetActive()
{
	m_eState = CS_SHAKE;
	m_fTimer = 0.0f;
}

bool CCrumble::Initialize()
{
	vector<CTransform*>::const_iterator iter;
	vector<CTransform*>::const_iterator iterEnd = m_pTransform->GetChildren()->end();

	for (iter = m_pTransform->GetChildren()->begin(); iter != iterEnd; ++iter)
	{
		//No Ref Count
		CGameObject* pGameObject = (*iter)->GetGameObject();
		m_vecChildren.push_back(pGameObject);
		m_vecOriginLocalPosition.push_back((*iter)->GetLocalPosition());
	}


	return true;
}

int CCrumble::Update(float fTime)
{
	if (m_eState == CS_STANDBY)
	{
		//DO NOTHING
	}

	if (m_eState == CS_SHAKE)
	{
		size_t iSize = m_vecChildren.size();

		m_fTimer += fTime;
		float fProgress = m_fTimer / 1.0f;

		for (size_t i = 0; i < iSize; ++i)
		{
			Vector2 vRandomDirection = Vector2::RandomDirection();
			Vector2 vShakePosition = vRandomDirection * m_fAmplitude * (1.0f - fProgress);

			CTransform* pTransform	= m_vecChildren[i]->GetTransform();
			pTransform->SetLocalPosition(m_vecOriginLocalPosition[i] + Vector3(vShakePosition.x, vShakePosition.y, 0.0f));
			SAFE_RELEASE(pTransform);
		}

		if (fProgress >= 1.0f)
		{
			for (size_t i = 0; i < iSize; ++i)
			{
				Vector2 vRandomDirection = Vector2::RandomDirection();
				Vector2 vShakePosition = vRandomDirection * m_fAmplitude * (1.0f - fProgress);

				CTransform* pTransform = m_vecChildren[i]->GetTransform();
				pTransform->SetLocalPosition(m_vecOriginLocalPosition[i]);
				SAFE_RELEASE(pTransform);

				m_vecGravitationalWeight.push_back(RandomRange(0.9f, 1.1f));
			}

			m_eState = CS_FALL;
			m_fTimer = 0.0f;
		}

	}

	if (m_eState == CS_FALL)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / 0.5f;

		float fGravity = Gravity(m_fTimer) * fTime;

		m_bGeneration = false;


		size_t iSize = m_vecChildren.size();
		for (size_t i = 0; i < iSize; ++i)
		{
			CTransform* pTransform = m_vecChildren[i]->GetTransform();
			pTransform->SetLocalPosition(pTransform->GetLocalPosition() - Vector3(0.0f, fGravity * m_vecGravitationalWeight[i], 0.0f));
			SAFE_RELEASE(pTransform);

			CRenderer* pRenderer = m_vecChildren[i]->GetComponentFromType<CRenderer>(CT_RENDERER);
			Vector4 vColor = pRenderer->GetDiffuse();
			vColor.w = 1.0f - fProgress;
			pRenderer->SetDiffuse(vColor);
			SAFE_RELEASE(pRenderer);
		}


		if (fProgress >= 1.0f)
		{
			m_vecGravitationalWeight.clear();
			m_fTimer = 0.0f;
			m_eState = CS_FALL_IDLE;
		}
	}

	if (m_eState == CS_FALL_IDLE)
	{
		m_fTimer += fTime;

		if (m_fTimer >= 5.0f)
		{
			m_fTimer = 0.0f;
			m_iGenerationCounter = 0;
			m_eState = CS_GENERATION;
		}		
	}	
	
	if (m_eState == CS_GENERATION)
	{
		m_fTimer += fTime;
		size_t iSize = m_vecChildren.size();

		if (m_fTimer >= 0.05f)
		{
			m_fTimer = 0.0f;

			CRenderer* pRenderer = m_vecChildren[m_iGenerationCounter]->GetComponentFromType<CRenderer>(CT_RENDERER);
			Vector4 vColor = Vector4::white;
			pRenderer->SetDiffuse(vColor);
			SAFE_RELEASE(pRenderer);

			CTransform* pTransform = m_vecChildren[m_iGenerationCounter]->GetTransform();
			pTransform->SetLocalPosition(m_vecOriginLocalPosition[m_iGenerationCounter]);
			SAFE_RELEASE(pTransform);

			++m_iGenerationCounter;

			if (m_iGenerationCounter == iSize)
			{
				m_eState = CS_STANDBY;
				m_fTimer = 0.0f;
				m_bGeneration = true;
			}
		}
	}


	return 0;
}

int CCrumble::OnCollisionEnter(CCollider * pThis, CCollider * pTarget, float fTime)
{
	//CGameObject* pTargetObject = pTarget->GetGameObject();
	//
	//if (pTargetObject->GetTag() == "Madeline" && m_eState == CS_STANDBY)
	//{
	//	m_eState = CS_SHAKE;
	//	m_fTimer = 0.0f;
	//}
	//
	//SAFE_RELEASE(pTargetObject);
	return 0;
}
