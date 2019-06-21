#include "MovingBlock.h"
#include "Transform.h"
#include "GameObject.h"
#include "Collider.h"
#include "Player.h"
#include "Scene.h"
#include "Layer.h"



CMovingBlock::CMovingBlock() :
	m_fForce(0.0f),
	m_fAcceleration(2000.0f),
	m_fBoostTimer(0.0f),
	m_bBoostable(0.0f),
	m_eState(MBS_IDLE),
	m_iTypes(0),
	m_pLight(NULL),
	m_bEnter(false),
	m_bDashed(false),
	m_pPlayer(NULL)
{
}


CMovingBlock::~CMovingBlock()
{
}

const Vector3 & CMovingBlock::GetForce() const
{
	return m_vForce;
}

void CMovingBlock::SetForce(const Vector3 & vForce)
{
	m_vForce = vForce;
}

void CMovingBlock::Reset()
{
	m_fForce = 0.0f;
	m_fBoostTimer = 0.0f;
	m_bBoostable = false;
	m_eState = MBS_IDLE;				
	m_bEnter = false;
	m_bDashed = false;
}

void CMovingBlock::SetBlockType(int iTypes)
{
	m_iTypes = iTypes;
}

void CMovingBlock::SetBeginPosition(const Vector3 & position)
{
	m_vBeginPosition = position;
}

void CMovingBlock::SetEndPosition(const Vector3 & position)
{
	m_vEndPosition = position;
}

bool CMovingBlock::GetBoostable() const
{
	return m_bBoostable;
}

bool CMovingBlock::Initialize()
{
	int iChildCount = m_pTransform->GetChildrenCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		CTransform* pChild = m_pTransform->GetChild(i);

		CRenderer* pRenderer = pChild->GetGameObject()->GetComponentFromType<CRenderer>(CT_RENDERER);

		string strAtlasKey = pRenderer->GetAtlasKey();

		if (strAtlasKey.find("light") != string::npos)
		{
			m_pLight = pRenderer;
			SAFE_RELEASE(pRenderer);
		}
		else
		{
			SAFE_RELEASE(pRenderer);
			continue;
		}
	}
	m_pLight->SetImageInfo("objects\\zipmover\\light01");


	CLayer* pLayer = m_pScene->FindLayer("Player");
	CGameObject* pPlayer = pLayer->FindGameObjectWithTag("Madeline");
	m_pPlayer = pPlayer;
	SAFE_RELEASE(pPlayer);
	





	return true;
}

int CMovingBlock::Update(float fTime)
{
	// "objects\\zipmover\\light00" : No Lights
	// "objects\\zipmover\\light01" : Red (Idle)
	// "objects\\zipmover\\light02" : Yellow (Return)
	// "objects\\zipmover\\light03" : Green (Move - Arrive)
 


	if (m_iTypes & MBT_TOUCH && !(m_iTypes & MBT_DASH) && m_eState == MBS_IDLE)
	{
		if (m_bEnter)
		{
			m_eState = MBS_MOVE;
			m_pLight->SetImageInfo("objects\\zipmover\\light03");
		}
	}
	//접촉 + 대시
	else if (m_iTypes & MBT_TOUCH && m_iTypes & MBT_DASH && m_eState == MBS_IDLE)
	{
		CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
		bool bDash = pPlayer->OnDash();
		SAFE_RELEASE(pPlayer);
		if (m_bEnter && bDash)
		{
			m_eState = MBS_MOVE;
			m_pLight->SetImageInfo("objects\\zipmover\\light03");
		}
	}
	//대시
	else if (!(m_iTypes & MBT_TOUCH) && m_iTypes & MBT_DASH && m_eState == MBS_IDLE)
	{
		CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
		if (pPlayer)
		{
			bool bDash = pPlayer->OnDash();
			SAFE_RELEASE(pPlayer);

			CTransform* pTransform = m_pPlayer->GetTransform();
			Vector3 vPlayerPosition = pTransform->GetWorldPosition();
			SAFE_RELEASE(pTransform);
			if (bDash && (m_pTransform->GetWorldPosition() - vPlayerPosition).Length() < 2000.0f)
			{
				m_eState = MBS_MOVE;
				m_pLight->SetImageInfo("objects\\zipmover\\light03");
			}
		}
		else
		{
			CLayer* pLayer = m_pScene->FindLayer("Player");
			CGameObject* pPlayerObject = pLayer->FindGameObjectWithTag("Madeline");
			m_pPlayer = pPlayerObject;
			pPlayer = pPlayerObject->GetComponentFromType<CPlayer>(CT_PLAYER);
			bool bDash = pPlayer->OnDash();
			SAFE_RELEASE(pPlayerObject);
			SAFE_RELEASE(pPlayer);

			CTransform* pTransform = m_pPlayer->GetTransform();
			Vector3 vPlayerPosition = pTransform->GetWorldPosition();
			SAFE_RELEASE(pTransform);
			if (bDash && (m_pTransform->GetWorldPosition() - vPlayerPosition).Length() < 2000.0f)
			{
				m_eState = MBS_MOVE;
				m_pLight->SetImageInfo("objects\\zipmover\\light03");
			}

		}
	}

	//이동 중 90% 도착 상태에서 대시 가능하도록

	if (m_eState == MBS_MOVE)
	{
		Vector3 position	 = m_pTransform->GetWorldPosition();
		Vector3 direction = (m_vEndPosition - position).Normalize();

		m_fForce += m_fAcceleration * fTime;

		m_pTransform->SetWorldPosition(position + direction * m_fForce * fTime);

		if (m_bEnter)
		{
			CTransform* pPlayerTransform = m_pPlayer->GetTransform();
			pPlayerTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition() + direction * m_fForce * fTime);
			SAFE_RELEASE(pPlayerTransform);
		}


		float fTotalLength = (m_vBeginPosition - m_vEndPosition).Length();
		float fCurrentLength = (m_vBeginPosition - position).Length();
		float fProgress = fCurrentLength / fTotalLength;

		if (fProgress >= 0.5f)
		{
			m_bBoostable = true;
			
			if (m_pPlayer)
			{
				CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
				pPlayer->SetPotentialForce(m_vForce);
				pPlayer->SetBoost(true);
				SAFE_RELEASE(pPlayer);
			}
		}

		if ((position - m_vEndPosition).Length() <= 5.0f)
		{
			m_eState = MBS_ARRIVE;
			m_fBoostTimer = 0.0f;
		}
	}

	if (m_eState == MBS_ARRIVE)
	{
		m_fBoostTimer += fTime;
		if (m_fBoostTimer >= 0.25f)
		{
			if (m_pPlayer)
			{
				m_bBoostable = false;
				CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
				pPlayer->SetPotentialForce(Vector3::Zero);
				pPlayer->SetBoost(false);
				SAFE_RELEASE(pPlayer);
			}
		}

		if (m_fBoostTimer >= 0.5f)
		{
			m_fBoostTimer = 0.0f;
			m_eState = MBS_RETURN;
			m_pLight->SetImageInfo("objects\\zipmover\\light02");
		}
	}

	if (m_eState == MBS_RETURN)
	{
		Vector3 position = m_pTransform->GetWorldPosition();
		Vector3 direction = (m_vBeginPosition - position).Normalize();
		

		m_pTransform->SetWorldPosition(position + direction * 100.0f * fTime);

		if (m_bEnter)
		{
			CTransform* pPlayerTransform = m_pPlayer->GetTransform();
			pPlayerTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition() + direction * 100.0f * fTime);
			SAFE_RELEASE(pPlayerTransform);
		}

		if ((position - m_vBeginPosition).Length() <= 5.0f)
		{
			m_eState = MBS_IDLE;
			Reset();
			m_pLight->SetImageInfo("objects\\zipmover\\light01");
		}	
	}
	
	return 0;
}

int CMovingBlock::OnCollisionEnter(CCollider * pThis, CCollider * pTarget, float fTime)
{
	return 0;
}

int CMovingBlock::OnCollisionStay(CCollider * pThis, CCollider * pTarget, float fTime)
{
		CGameObject* pTargetObject = pTarget->GetGameObject();
		string strTarget = pTargetObject->GetTag();
		if (strTarget == "Madeline")
		{
			m_bEnter = true;
			m_pPlayer = pTargetObject;
		}
		SAFE_RELEASE(pTargetObject);

	return 0;
}

int CMovingBlock::OnCollisionExit(CCollider * pThis, CCollider * pTarget, float fTime)
{
	if (pTarget)
	{
		CGameObject* pTargetObject = pTarget->GetGameObject();
		string strTarget = pTargetObject->GetTag();
		if (strTarget == "Madeline")
		{
			m_bEnter = false;
		}

		if (!m_bBoostable && m_eState != MBS_MOVE && m_eState != MBS_RETURN)
		{
			if (m_pPlayer)
			{
				CPlayer* pPlayer = m_pPlayer->GetComponentFromType<CPlayer>(CT_PLAYER);
				pPlayer->ResetPotentialForce();
				pPlayer->SetBoost(false);
				SAFE_RELEASE(pPlayer);
			}

			if(!(m_iTypes & MBT_DASH))
				m_pPlayer = NULL;
		}

		SAFE_RELEASE(pTargetObject);
	}
	return 0;
}
