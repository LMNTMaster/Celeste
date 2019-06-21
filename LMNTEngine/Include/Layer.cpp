#include "Layer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"
#include "Device.h"

LMNT_USING


CLayer::CLayer() :
	m_iZOrder(0)
{
}

CLayer::~CLayer()
{
	Safe_Release_VecList(m_GameObjectList);
}

void CLayer::SetScene(CScene * pScene)
{
	m_pScene = pScene;
}

void CLayer::SetZOrder(int iZOrder)
{
	m_iZOrder = iZOrder;
}

int CLayer::GetZOrder() const
{
	return m_iZOrder;
}

void CLayer::RemoveGameObject(CGameObject * pGameObject)
{
	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = m_GameObjectList.end();

	for (iter = m_GameObjectList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pGameObject)
		{
			m_GameObjectList.erase(iter);
			break;
		}
	}
}

void CLayer::AddGameObject(CGameObject * pObj)
{
	pObj->SetScene(m_pScene);
	pObj->SetLayer(this);
	pObj->AddRef();
	m_GameObjectList.push_back(pObj);
}

const list<class CGameObject*>* CLayer::GetAllGameObjects() const
{
	return &m_GameObjectList;
}

CGameObject* CLayer::FindGameObjectWithTag(const string & strTag)
{
	list<CGameObject*>::iterator iter;
	list<CGameObject*>::iterator iterEnd = m_GameObjectList.end();

	for (iter = m_GameObjectList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return (*iter);
		}
	}
	return NULL;
}

bool CLayer::Save(FILE * pFile)
{
	return false;
}

bool CLayer::Load(FILE * pFile)
{
	return false;
}


bool CLayer::Initialize()
{
	return true;
}

int CLayer::Input(float fTime)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_GameObjectList.end();

	for (iter = m_GameObjectList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_GameObjectList.erase(iter);
			//m_pScene->RemoveGameObject(*iter);
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Input(fTime);

		++iter;
	}

	return 0;
}

int CLayer::Update(float fTime)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_GameObjectList.end();

	for (iter = m_GameObjectList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_GameObjectList.erase(iter);
			//m_pScene->RemoveGameObject(*iter);
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(fTime);

		++iter;
	}
	return 0;
}

int CLayer::LateUpdate(float fTime)
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_GameObjectList.end();

	for (iter = m_GameObjectList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_GameObjectList.erase(iter);
			//m_pScene->RemoveGameObject(*iter);
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->LateUpdate(fTime);

		++iter;
	}
	return 0;
}

int CLayer::Collision(float fTime)
{	
	return 0;
}

void CLayer::Render(float fTime)
{
	//화면 밖의 오브젝트는 렌더하지 않음.
	CTransform* pCameraTransform = m_pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	Vector3 vOuterScreen = vCameraPosition + Vector3(DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight, 0.0f);
	SAFE_RELEASE(pCameraTransform);

	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_GameObjectList.end();

	for (iter = m_GameObjectList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_GameObjectList.erase(iter);
			//m_pScene->RemoveGameObject(*iter);
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		{
			CTransform* pIterTransform = (*iter)->GetTransform();
			Vector3 vPosition = pIterTransform->GetWorldPosition();
			Vector3 vScale = pIterTransform->GetWorldScale();
			Vector3 vPivot = pIterTransform->GetPivot();
			Vector3 LT = vPosition - vScale * vPivot;
			Vector3 RB = LT + vScale;
			SAFE_RELEASE(pIterTransform);		
			
			if(m_iZOrder < 100000 && (vCameraPosition.x > RB.x || vOuterScreen.x < LT.x || vCameraPosition.y > RB.y || vOuterScreen.y < LT.y))
			{
				++iter;
				continue;
			}
			else
			{
				(*iter)->Render(fTime);
				++iter;
			}
		}
	}
}
