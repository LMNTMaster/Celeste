#include "FallBlock.h"
#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "Core.h"
#include "Collider.h"
#include "../GameData.h"
#include "SoundManager.h"


CFallBlock::CFallBlock():
	m_bSoundPlayAtShake(false),
	m_bSoundPlayAtRelease(false)
{
	m_eComponentType = CT_FALL_BLOCK;
	SetTag("FallBlock");
}


CFallBlock::~CFallBlock()
{
}

float CFallBlock::Gravity(float fElapseTime)
{
	return (448.0f * (-2.0f * fElapseTime)) / 0.4225f;
}

void CFallBlock::SetCameraShake(bool bShake)
{
	m_bUseCameraShake = bShake;
}

void CFallBlock::SetColliderFixed(bool bFixed)
{
	m_bFixedCollider = bFixed;
}

void CFallBlock::InteractionWithTerrain(bool bInteraction)
{
	m_bInteractionWithTerrain = bInteraction;
}

void CFallBlock::SetShakeTime(float fTime)
{
}

void CFallBlock::SetFallBlockState(FALL_BLOCK_STATE eState)
{
	m_eState = eState;
}

FALL_BLOCK_STATE CFallBlock::GetFallBlockState() const
{
	return m_eState;
}

void CFallBlock::Reset()
{
	m_pTransform->SetWorldPosition(m_vOriginPosition);
	m_pPlayer = NULL;
	m_eState = FBS_STANDBY;
	m_fFallTimer = 0.0f;
	m_fShakeTimer = 0.0f;
	m_fShakeTime = 1.0f;
}

bool CFallBlock::Initialize()
{
	m_bUseCameraShake = true;
	m_bFixedCollider = true;
	m_pPlayer = NULL; 
	m_eState = FBS_STANDBY;
	m_bInteractionWithTerrain = true;
	m_fFallTimer = 0.0f;
	m_fFrequency = 30;
	m_fAmplitude = 5.0f;
	m_fShakeTimer = 0.0f;
	m_fShakeTime = 1.0f;

	m_vOriginPosition = m_pTransform->GetWorldPosition();


	GET_SINGLE(CGameData)->AddObjectIntoAreaData(m_pGameObject);
	return true;
}

int CFallBlock::Update(float fTime)
{
	if (m_eState == FBS_SHAKE)
	{
		if (!m_bSoundPlayAtShake)
		{
			if (m_pGameObject->GetTag() == "PrologueFallBlockObject")
			{
				GET_SINGLE(CSoundManager)->Play("game_00_fallingblock_prologue_shake");
				m_bSoundPlayAtShake = true;
			}
			else if(m_pGameObject->GetTag() == "PrologueBridge00")
			{
				GET_SINGLE(CSoundManager)->Play("game_00_bridge_rumble_loop");
				m_bSoundPlayAtShake = true;
			}
		}


		m_fShakeTimer += fTime;
		float fProgress = m_fShakeTimer / m_fShakeTime;
		Vector2 vRandomDirection = Vector2::RandomDirection();
		vRandomDirection.y = fabsf(vRandomDirection.y);
		Vector2 vShakePosition = vRandomDirection * m_fAmplitude * (1.0f - fProgress);
			//sinf(PI * m_fFrequency * m_fShakeTimer) * m_fAmplitude * (1.0f - fProgress);

		vShakePosition.y = 0;

		m_pTransform->SetWorldPosition(m_vOriginPosition + Vector3(vShakePosition.x, vShakePosition.y, 0.0f));
		
		
		if (m_bFixedCollider)
		{
			CCollider* pCollider = m_pGameObject->GetComponentFromType<CCollider>(CT_COLLIDER);
			Vector3 vScale = pCollider->GetScale();
			Vector3 deltaPivot = Vector3(0, (1.0f / vScale.y) * vShakePosition.y, 0.0f );
			pCollider->SetPivot(deltaPivot);
			SAFE_RELEASE(pCollider);
		}


		if (fProgress >= 1.0f)
		{
			if (!m_bSoundPlayAtRelease)
			{
				if (m_pGameObject->GetTag() == "PrologueFallBlockObject")
				{
					GET_SINGLE(CSoundManager)->Play("game_00_fallingblock_prologue_release");
					m_bSoundPlayAtRelease = true;
				}
				else if(m_pGameObject->GetTag() == "PrologueBridge00")
				{
					m_bSoundPlayAtRelease = true;
					int iRand = (rand() % 6) + 1;
					switch (iRand)
					{
					case 1:
						GET_SINGLE(CSoundManager)->Play("game_00_bridge_supportbreak_01");
						break;
					case 2:
						GET_SINGLE(CSoundManager)->Play("game_00_bridge_supportbreak_02");
						break;
					case 3:
						GET_SINGLE(CSoundManager)->Play("game_00_bridge_supportbreak_03");
						break;
					case 4:
						GET_SINGLE(CSoundManager)->Play("game_00_bridge_supportbreak_04");
						break;
					case 5:
						GET_SINGLE(CSoundManager)->Play("game_00_bridge_supportbreak_05");
						break;
					case 6:
						GET_SINGLE(CSoundManager)->Play("game_00_bridge_supportbreak_06");
						break;
					default:
						break;
					}
				}
			}


			m_eState = FBS_FALL;
			CCollider* pCollider = m_pGameObject->GetComponentFromType<CCollider>(CT_COLLIDER);
			pCollider->SetPivot(Vector3(0.0f, 0.0f, 0.0f));
			m_pTransform->SetWorldPosition(m_vOriginPosition);
			SAFE_RELEASE(pCollider);
		}
	}

	if (m_eState == FBS_FALL)
	{
		m_fFallTimer += fTime;
		float fGravity = Gravity(m_fFallTimer) * fTime ;
		m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition() - Vector3(0.0f, fGravity, 0.0f));

	}
	return 0;


}

int CFallBlock::OnCollisionEnter(CCollider * pThis, CCollider * pTarget, float fTime)
{
	CGameObject* pTargetObject = pTarget->GetGameObject();
	string strTag = pTargetObject->GetTag();
	SAFE_RELEASE(pTargetObject);
	if (strTag == "Madeline" && m_eState == FBS_STANDBY)
	{
		m_eState = FBS_SHAKE;
		m_pPlayer = pTargetObject;
	}
	else
	{
		if (strTag != "Madeline" && strTag != "Spike" && m_bInteractionWithTerrain && m_eState == FBS_FALL)
		{
			if (m_pGameObject->GetTag() == "PrologueBridge00" || m_pGameObject->GetTag() == "PrologueFallBlockObject")
				GET_SINGLE(CSoundManager)->Play("game_00_fallingblock_prologue_impact");

			if(m_bUseCameraShake)
				GET_SINGLE(CGameData)->ShakeCamera(0.5f, 1.0f);
			
			m_eState = FBS_FALL_IDLE;
			CTransform* pTargetTransform = pTarget->GetTransform();
			Vector3 vTargetPosition = pTargetTransform->GetWorldPosition();
			SAFE_RELEASE(pTargetTransform);
			Vector3 vTargetScale = pTarget->GetScale();
			Vector3 vTargetPivot = pTarget->GetPivot();
			Vector3 TargetLT = vTargetPosition - vTargetScale * vTargetPivot;
			Vector3 position = Vector3( m_pTransform->GetWorldPosition().x, TargetLT.y - pThis->GetScale().y , m_pTransform->GetWorldPosition().z);
			m_pTransform->SetWorldPosition(position);
		}
	}
	return 0;
}

int CFallBlock::OnCollisionExit(CCollider * pThis, CCollider * pTarget, float fTime)
{
	//CGameObject* pTargetObject = pTarget->GetGameObject();
	//string strTag = pTargetObject->GetTag();
	//SAFE_RELEASE(pTargetObject);
	//if (strTag == "Madeline")
	{
		m_pPlayer = NULL;
	}
	return 0;
}
