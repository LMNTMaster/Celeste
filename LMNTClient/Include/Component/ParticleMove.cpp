#include "ParticleMove.h"
#include "../Client.h"
#include "GameObject.h"
#include "Transform.h"
#include "Component/Renderer.h"
#include "Component.h"
#include "Scene.h"
#include "Device.h"


CParticleMove::CParticleMove():
	m_fTimer(0.0f),
	m_fFadeTimer(0.0f),
	m_bFading(false),
	m_bUseFadeout(false),
	m_bUseSpeedReduce(false),
	m_pManager(NULL),
	m_bUseSnow(false)
{
	m_eComponentType = CT_PARTICLEMOVE;
	SetTag("ParticleMove");
}

CParticleMove::CParticleMove(const CParticleMove & component)
{
	*this = component;
}


CParticleMove::~CParticleMove()
{
}

CParticleMove * CParticleMove::Clone() const
{
	return new CParticleMove(*this);
}


void CParticleMove::UseSnow()
{
	m_bUseSnow = true;
}

void CParticleMove::SetManager(CParticleManager * pManager)
{
	m_pManager = pManager;
}

void CParticleMove::SetReduceSpeed(bool bUse)
{
	m_bUseSpeedReduce = bUse;
}

void CParticleMove::SetFadeout(bool bUse)
{
	m_bUseFadeout = bUse;
}

void CParticleMove::SetTargetPosition(const Vector3 & vPosition)
{
	m_vTargetPosition = vPosition;
}

void CParticleMove::SetMoveSpeed(float fSpeed)
{
	m_fMoveSpeed = fSpeed;
}

void CParticleMove::SetFrequency(float fTime)
{
	m_fFrequency = fTime;
}

void CParticleMove::SetAmplitude(float fDistance)
{
	m_fAmplitude = fDistance;
}

bool CParticleMove::Initialize()
{
	m_vSpawnPosition = m_pTransform->GetWorldPosition();
	m_vPivotPosition = m_vSpawnPosition;
	return true;
}

int CParticleMove::Update(float fTime)
{

	if (m_bFading)
	{
		m_fFadeTimer -= fTime;
		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		Vector4 vDiffuse = pRenderer->GetDiffuse();
		pRenderer->SetDiffuse(Vector4(vDiffuse.x, vDiffuse.y, vDiffuse.z, m_fFadeTimer));		
		
		if (m_fFadeTimer <= 0)
		{
			m_bFading = false;
			pRenderer->SetDiffuse(Vector4(vDiffuse.x, vDiffuse.y, vDiffuse.z, 0.0f));
			m_pGameObject->Destroy(true);
		}
		SAFE_RELEASE(pRenderer);
	}

	if (!m_bUseSpeedReduce)
	{
		m_fEffectiveMoveSpeed = m_fMoveSpeed;
	}
	else
	{
		float fDistanceBetweenOrigin = (m_vTargetPosition - m_vSpawnPosition).Length();
		float fDistanceBetweenCurrent = (m_vTargetPosition - m_vPivotPosition).Length();
		float fProgress =1.0f - ( fDistanceBetweenCurrent / fDistanceBetweenOrigin);
		float fRate = cosf((PI / 2.0f) * fProgress);
		m_fEffectiveMoveSpeed = fRate * m_fMoveSpeed;
	}



	m_fTimer += fTime;
	Vector3 vDirection = (m_vTargetPosition - m_vSpawnPosition).Normalize();
	Vector3 vRight = Vector3(-vDirection.y, vDirection.x, 0.0f);

	m_vPivotPosition = m_vPivotPosition + vDirection * m_fEffectiveMoveSpeed * fTime;

	Vector3 position = m_vPivotPosition + vRight * m_fAmplitude  * sinf(m_fTimer * (PI / m_fFrequency));
	
	CTransform* pCameraTransform = m_pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	Vector2 vScreenSize;
	Vector2 vBuffer;

	vScreenSize.x = 1280.0f;
	vScreenSize.y = 720.0f;

	vBuffer.x = 100.0f;
	vBuffer.y = 100.0f;


	if (m_bUseSnow)
	{
		bool bMoved = false;
		while (position.y < vCameraPosition.y - vBuffer.y || position.y > vCameraPosition.y + vScreenSize.y + vBuffer.y)
		{
			if (position.y < vCameraPosition.y - vBuffer.y)
			{
				position.y += vScreenSize.y + vBuffer.y; 
				//m_vPivotPosition.y += vScreenSize.y + vBuffer.y;
				//bMoved = true;
			}

			if (position.y > vCameraPosition.y + vScreenSize.y + vBuffer.y)
			{
				position.y -= vScreenSize.y + vBuffer.y;
				//m_vPivotPosition.y -= vScreenSize.y + vBuffer.y;
				//bMoved = true;
			}
		}


		while (position.x < vCameraPosition.x - vBuffer.x || position.x > vCameraPosition.x + vScreenSize.x + vBuffer.x)
		{
			if (position.x < vCameraPosition.x - vBuffer.x)
			{
				position.x += vScreenSize.x + vBuffer.x;
				//m_vPivotPosition.x += vScreenSize.x + vBuffer.x;
				//bMoved = true;
			}

			if (position.x > vCameraPosition.x + vScreenSize.x + vBuffer.x)
			{
				position.x -= vScreenSize.x + vBuffer.x;
				//m_vPivotPosition.x -= vScreenSize.x + vBuffer.x;
				//bMoved = true;
			}
		}

		if (bMoved)
		{
			//m_vTargetPosition = Vector3(vCameraPosition.x - vBuffer.x - 1000.0f, vCameraPosition.y + RandomRange(0, vScreenSize.y), 0.0f);
		}
	}


	m_pTransform->SetWorldPosition(position);

	if (!m_bFading && (m_vPivotPosition - m_vTargetPosition).Length() < 30.0f )
	{
		if (m_bUseFadeout)
		{
			m_bFading = true;
			m_fFadeTimer = 1.0f;
		}
		else
		{
			//m_pGameObject->Destroy(true);
			//m_pManager->ReduceCount();			
		}
	}

	return 0;
}
