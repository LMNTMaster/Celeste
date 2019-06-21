#include "CameraMove.h"
#include "Device.h"
#include "Transform.h"
#include "GameObject.h"
#include "../GameData.h"

#include "Input.h"
#include "Scene.h"
#include "Layer.h"
#include "Player.h"

CCameraMove::CCameraMove()
{
	m_eComponentType = CT_CAMERA_MOVE;
	SetTag("CameraMove");
}
CCameraMove::~CCameraMove()
{
}
CCameraMove::CCameraMove(const CCameraMove & component)
{
	*this = component;
}

CCameraMove* CCameraMove::Clone()
{
	return new CCameraMove(*this);
}

bool CCameraMove::Sort(CGameObject * pSrc, CGameObject * pDest)
{
	CLayer* pLayer1 = pSrc->GetLayer();
	CLayer* pLayer2 = pDest->GetLayer();

	int iLayer1ZOrder = pLayer1->GetZOrder();
	int iLayer2ZOrder = pLayer1->GetZOrder();
	SAFE_RELEASE(pLayer1);
	SAFE_RELEASE(pLayer2);

	return iLayer1ZOrder < iLayer2ZOrder;
}

void CCameraMove::Unlock()
{
	m_bLock = false;
}

void CCameraMove::Shake(float fDuration, float fAmplitude)
{
	m_bShake = true;
	m_fShakeDuration = fDuration;
	m_fShakeTimer = 0.0f;
	m_fShakeAmplitude = fAmplitude;
}

void CCameraMove::AddBackground(CGameObject * pTarget)
{
	m_vecBackground.push_back(pTarget);
	sort(m_vecBackground.begin(), m_vecBackground.end(), CCameraMove::Sort);
}

void CCameraMove::AddBackground(class CScene* pScene)
{
	int iLayerCount = pScene->GetLayerCount();

	for (int i = 0; i < iLayerCount; ++i)
	{
		CLayer* pLayer = pScene->FindLayer(i);
		string strTag = pLayer->GetTag();

		if (strTag == "BG0" || strTag == "BG1" || strTag == "BG2" || strTag == "BG3" || strTag == "BG4" || strTag == "BG5" || strTag == "FG0" || strTag == "FG1" || strTag == "FG2")
		{
			list<CGameObject*>::const_iterator iter = pLayer->GetAllGameObjects()->begin();
			list<CGameObject*>::const_iterator iterEnd = pLayer->GetAllGameObjects()->end();
			for (; iter != iterEnd; ++iter)
			{
				AddBackground(*iter);
			}
		}
		SAFE_RELEASE(pLayer);
	}
}

void CCameraMove::SetPlayer(CGameObject * pPlayer)
{
	m_pPlayer = pPlayer;
}

void CCameraMove::SetSpeed(float fSpeed)
{
	m_fMoveSpeed = fSpeed;
}

float CCameraMove::GetSpeed() const
{
	return m_fMoveSpeed;
}

void CCameraMove::SetAreaData(vector<PAREA>* vecArea)
{	
	m_vecArea = *vecArea;
}

PAREA CCameraMove::GetArea() const
{
	return m_pCurrentArea;
}

bool CCameraMove::Initialize()
{
	m_bLock = true;
	m_vScreenSize = Vector2(DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight);

	return true;
}

int CCameraMove::Update(float fTime)
{
	if (m_bLock)
	{

		//캐릭터의 구역 확인
		CTransform* pPlayerTransform = m_pPlayer->GetTransform();
		Vector3 vPlayerPosition = pPlayerTransform->GetWorldPosition();
		SAFE_RELEASE(pPlayerTransform);

		m_pCurrentArea = GET_SINGLE(CGameData)->GetCurrentArea();
		m_pPrevArea = GET_SINGLE(CGameData)->GetPrevArea();

		if (m_pCurrentArea)
		{
			Vector2 CameraRegionMin = Vector2(m_pCurrentArea->vRange.x, m_pCurrentArea->vRange.y);
			Vector2 CameraRegionMax = Vector2(m_pCurrentArea->vRange.x + m_pCurrentArea->vRange.z - m_vScreenSize.x, m_pCurrentArea->vRange.y + m_pCurrentArea->vRange.w - m_vScreenSize.y);
			Vector2 RegionCenter = (CameraRegionMax + CameraRegionMin) / 2.0f;
			Vector3 PlayerPositionRelativeToCamera = Vector3(vPlayerPosition.x - m_vScreenSize.x / 2, vPlayerPosition.y - m_vScreenSize.y / 2, 0);


			Vector3 position = m_pTransform->GetWorldPosition();



			Vector3 lerp = XMVectorLerp(m_pTransform->GetWorldPosition(), PlayerPositionRelativeToCamera, fTime * 2.0f);
			if (lerp.x < CameraRegionMin.x)
			{
				lerp.x = Lerp(m_pTransform->GetWorldPosition().x, m_pCurrentArea->vRange.x, fTime * 10.0f);
			}
			//오른쪽 경계
			if (lerp.x > CameraRegionMax.x)
			{
				lerp.x = Lerp(m_pTransform->GetWorldPosition().x, CameraRegionMax.x, fTime * 10.0f);
			}
			//위쪽 경계
			if (lerp.y < CameraRegionMin.y)
			{
				lerp.y = Lerp(m_pTransform->GetWorldPosition().y, m_pCurrentArea->vRange.y, fTime * 10.0f);
			}
			//아래쪽 경계
			if (lerp.y > CameraRegionMax.y)
			{
				lerp.y = Lerp(m_pTransform->GetWorldPosition().y, CameraRegionMax.y, fTime * 10.0f);
			}

			Vector3 vDistance = Vector3(lerp.x, lerp.y, -10.0f) - m_pTransform->GetWorldPosition();


			m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition() + vDistance);

			for (size_t i = 0; i < m_vecBackground.size(); ++i)
			{
				CLayer* pLayer = m_vecBackground[i]->GetLayer();
				float fMoveRate = 0;
				switch (pLayer->GetZOrder())
				{
				case -90:	fMoveRate = 0.95f; break;
				case -85:	fMoveRate = 0.9f; break;
				case -80:	fMoveRate = 0.75f; break;
				case -75:	fMoveRate = 0.50f; break;
				case -70:	fMoveRate = 0.25f; break;
				case -65:	fMoveRate = 0.125f; break;
				case 65:	fMoveRate = -0.125f; break;
				case 70:	fMoveRate = -0.25f; break;
				case 75:	fMoveRate = -0.5f; break;
				default:
					break;
				}

				CTransform* pBack = m_vecBackground[i]->GetTransform();
				pBack->SetWorldPosition(pBack->GetWorldPosition() + vDistance  * fMoveRate);
				SAFE_RELEASE(pBack);
				SAFE_RELEASE(pLayer);
			}


			if (m_bShake)
			{
				m_fShakeTimer += fTime;
				float fProgress = m_fShakeTimer / m_fShakeDuration;

				Vector2 vRandomDirection = Vector2::RandomDirection();
				Vector2 vShakePosition = vRandomDirection * m_fShakeAmplitude * (1.0f - fProgress);
				m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition() + Vector3(vShakePosition.x, vShakePosition.y, 0.0f));
				if (fProgress >= m_fShakeDuration)
				{
					m_fShakeAmplitude = 0;
					m_fShakeDuration = 0;
					m_fShakeTimer = 0;
					m_bShake = false;
				}
			}
		}//if (m_pCurrentArea)
	}
	else
	{

		for (size_t i = 0; i < m_vecBackground.size(); ++i)
		{
			CLayer* pLayer = m_vecBackground[i]->GetLayer();
			float fMoveRate = 0;
			switch (pLayer->GetZOrder())
			{
			case -90:	fMoveRate = 0.95f; break;
			case -85:	fMoveRate = 0.9f; break;
			case -80:	fMoveRate = 0.75f; break;
			case -75:	fMoveRate = 0.50f; break;
			case -70:	fMoveRate = 0.25f; break;
			case -65:	fMoveRate = 0.125f; break;
			case 65:	fMoveRate = -0.125f; break;
			case 70:	fMoveRate = -0.25f; break;
			case 75:	fMoveRate = -0.5f; break;
			default:
				break;
			}

			CTransform* pBack = m_vecBackground[i]->GetTransform();
			pBack->SetWorldPosition(pBack->GetWorldPosition() + pBack->GetDeltaPosition()  * fMoveRate);
			SAFE_RELEASE(pBack);
			SAFE_RELEASE(pLayer);
		}
	}

	return 0;
}
