#include "GameObject.h"
#include "Scene.h"
#include "Transform.h"
#include "Layer.h"
#include "Component.h"
#include "Collider.h"
#include "Device.h"

LMNT_USING

unordered_map<class CScene*, unordered_map<string, CGameObject*>>	CGameObject::m_mapPrototype;

CGameObject::CGameObject() :
	m_pTransform(NULL),
	m_pScene(NULL),
	m_pLayer(NULL),
	m_iID(0)
{
	m_pTransform = new CTransform;
	m_pTransform->Initialize();
	m_pTransform->m_pGameObject = this;
}

CGameObject::CGameObject(const CGameObject & pGameObject)
{
	*this = pGameObject;

	m_pTransform = pGameObject.m_pTransform->Clone();

	m_ComponentList.clear();
	list<CComponent*>::const_iterator	iter;
	list<CComponent*>::const_iterator	iterEnd = pGameObject.m_ComponentList.end();

	for (iter = pGameObject.m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		CComponent*	pComponent = (*iter)->Clone();
		pComponent->SetGameObject(this);
		pComponent->SetTransform(m_pTransform);
		m_ComponentList.push_back(pComponent);
	}
}

CGameObject::~CGameObject()
{
	Safe_Release_VecList(m_ComponentList);
	SAFE_RELEASE(m_pTransform);
}

CTransform* CGameObject::GetTransform() const
{
	if (m_pTransform)
		m_pTransform->AddRef();

	return m_pTransform;
}

bool CGameObject::Save(FILE * pFile)
{
	//이름, ID, 레이어, 컴포넌트 갯수, 컴포넌트 타입 저장
	int iLength = this->GetTag().length();
	fwrite(&iLength, 4, 1, pFile);
	fwrite(this->GetTag().c_str(), sizeof(char), iLength, pFile);
	
	//ID 저장
	fwrite(&m_iID, 4, 1, pFile);

	//Layer 저장
	iLength = m_pLayer->GetTag().length();
	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_pLayer->GetTag().c_str(), sizeof(char), iLength, pFile);

	//컴포넌트 갯수 저장
	iLength = m_ComponentList.size();
	fwrite(&iLength, 4, 1, pFile);

	//Transform 저장 
	m_pTransform->Save(pFile);

	list<CComponent*>::iterator iter;
	list<CComponent*>::iterator iterEnd = m_ComponentList.end();
	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Save(pFile);
	}
	return true;
}

bool CGameObject::Load(FILE * pFile)
{
	//이름, ID, 레이어, 컴포넌트 갯수, 컴포넌트 타입 저장
	char cBuffer[256] = {};
	int iLength = 0;
	fwrite(&iLength, 4, 1, pFile);
	fwrite(cBuffer, sizeof(char), iLength, pFile);

	//ID 저장
	fwrite(&m_iID, 4, 1, pFile);

	//Layer 저장
	iLength = m_pLayer->GetTag().length();
	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_pLayer->GetTag().c_str(), sizeof(char), iLength, pFile);

	//컴포넌트 갯수 저장
	iLength = m_ComponentList.size();
	fwrite(&iLength, 4, 1, pFile);

	list<CComponent*>::iterator iter;
	list<CComponent*>::iterator iterEnd = m_ComponentList.end();
	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Save(pFile);
	}
	return true;
}


CGameObject * CGameObject::CreateObject(const string & strTag, CLayer * pLayer)
{
	CGameObject*	pObj = new CGameObject;

	pObj->SetTag(strTag);

	if (!pObj->Initialize())
	{
		SAFE_RELEASE(pObj);
		return NULL;
	}

	if (pLayer)
		pLayer->AddGameObject(pObj);

	//if (pObj->m_pScene)
	//{
	//	CScene* pScene = pObj->GetScene();
	//	pScene->AddGameObject(pObj);
	//	SAFE_RELEASE(pScene);
	//}
	return pObj;
}

CGameObject * CGameObject::CreatePrototype(const string & strTag, CScene * pScene)
{

	unordered_map<CScene*, unordered_map<string, CGameObject*>>::iterator	iter = m_mapPrototype.find(pScene);

	if (iter == m_mapPrototype.end())
	{
		unordered_map<string, CGameObject*>	map;

		m_mapPrototype.insert(make_pair(pScene, map));
	}

	CGameObject*	pPrototype = FindPrototype(strTag, pScene);

	if (pPrototype)
	{
		pPrototype->AddRef();
		return pPrototype;
	}

	pPrototype = new CGameObject;

	pPrototype->SetTag(strTag);
	pPrototype->SetScene(pScene);

	if (!pPrototype->Initialize())
	{
		SAFE_RELEASE(pPrototype);
		return NULL;
	}

	unordered_map<string, CGameObject*>*	pMap = FindPrototypeList(pScene);

	pPrototype->AddRef();
	pMap->insert(make_pair(strTag, pPrototype));

	return pPrototype;
}

CGameObject * CGameObject::CreateClone(const string & strTag, CScene * pScene, CLayer * pLayer)
{
	CGameObject*	pPrototype = FindPrototype(strTag, pScene);

	if (!pPrototype)
		return NULL;

	CGameObject*	pClone = pPrototype->Clone();

	list<CComponent*>* ColliderList = pClone->GetComponentsFromType(CT_COLLIDER);
	list<CComponent*>::iterator iter;
	list<CComponent*>::iterator iterEnd = ColliderList->end();

	for (iter = ColliderList->begin(); iter != iterEnd; ++iter)
	{
		pScene->AddCollider((CCollider*)(*iter));
		(*iter)->Release();
	}

	//Safe_Release_VecList(ColliderList);

	if (pLayer)
		pLayer->AddGameObject(pClone);



	return pClone;
}

//다른 Scene으로 Prototype을 이전 해 줄 필요가 있을 때, 지워질 SceneComponent 의 소멸자에서 사용하도록 한다.
bool CGameObject::ChangePrototypeScene(const string & strTag, CScene * pCurScene, CScene * pChangeScene)
{
	unordered_map<CScene*, unordered_map<string, CGameObject*>>::iterator	iter = m_mapPrototype.find(pChangeScene);

	if (iter == m_mapPrototype.end())
	{
		unordered_map<string, CGameObject*>	map;

		m_mapPrototype.insert(make_pair(pChangeScene, map));
	}

	CGameObject*	pPrototype = FindPrototype(strTag, pCurScene);

	if (!pPrototype)
		return false;

	pPrototype->SetScene(pChangeScene);

	unordered_map<string, CGameObject*>*	pMap = FindPrototypeList(pCurScene);

	unordered_map<string, CGameObject*>::iterator	iterProto = pMap->find(strTag);

	pMap->erase(iterProto);

	pMap = FindPrototypeList(pChangeScene);

	pMap->insert(make_pair(strTag, pPrototype));

	return true;
}

bool CGameObject::ErasePrototype(const string & strTag, CScene * pScene)
{
	unordered_map<string, CGameObject*>*	pMap = FindPrototypeList(pScene);

	if (!pMap)
		return false;

	unordered_map<string, CGameObject*>::iterator	iter = pMap->find(strTag);

	if (iter == pMap->end())
		return false;

	SAFE_RELEASE(iter->second);

	pMap->erase(iter);

	return true;
}

bool CGameObject::ErasePrototypeList(CScene * pScene)
{
	unordered_map<CScene*, unordered_map<string, CGameObject*>>::iterator	iterMap = m_mapPrototype.find(pScene);

	if (iterMap == m_mapPrototype.end())
		return false;

	unordered_map<string, CGameObject*>::iterator	iter;
	unordered_map<string, CGameObject*>::iterator	iterEnd = iterMap->second.end();

	for (iter = iterMap->second.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}

	iterMap->second.clear();

	m_mapPrototype.erase(iterMap);

	return true;
}

unordered_map<string, CGameObject*>* CGameObject::FindPrototypeList(CScene * pScene)
{
	unordered_map<class CScene*, unordered_map<string, CGameObject*>>::iterator	iter = m_mapPrototype.find(pScene);

	if (iter == m_mapPrototype.end())
		return NULL;

	return &iter->second;
}

CGameObject * CGameObject::FindPrototype(const string & strTag, CScene * pScene)
{
	unordered_map<string, CGameObject*>*	pMap = FindPrototypeList(pScene);

	if (!pMap)
		return NULL;

	unordered_map<string, CGameObject*>::iterator	iter = pMap->find(strTag);

	if (iter == pMap->end())
		return NULL;

	return iter->second;
}

CScene* CGameObject::GetScene() const
{
	m_pScene->AddRef();
	return m_pScene;
}

CLayer* CGameObject::GetLayer() const
{
	m_pLayer->AddRef();
	return m_pLayer;
}

void CGameObject::SetScene(CScene * pScene)
{
	m_pScene = pScene;

	m_pTransform->SetScene(pScene);

	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->SetScene(pScene);
	}
}

void CGameObject::SetLayer(CLayer * pLayer)
{
	m_pLayer = pLayer;

	m_pTransform->SetLayer(pLayer);

	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->SetLayer(pLayer);
	}
}

void CGameObject::ChangeLayer(CLayer * pTargetLayer)
{
	m_pLayer->RemoveGameObject(this);
	pTargetLayer->AddGameObject(this);
	this->Release();
}

bool CGameObject::Initialize()
{
	return true;
}

int CGameObject::Input(float fTime)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
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

int CGameObject::Update(float fTime)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
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

	m_pTransform->Update(fTime);

	return 0;
}

int CGameObject::LateUpdate(float fTime)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
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

	m_pTransform->LateUpdate(fTime);

	return 0;
}

int CGameObject::OnCollisionEnter(class CCollider* pThis, class CCollider* pTarget, float fTime)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		(*iter)->OnCollisionEnter(pThis, pTarget, fTime);

		++iter;
	}
	return 0;
}

int CGameObject::OnCollisionStay(class CCollider* pThis, class CCollider* pTarget, float fTime)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		(*iter)->OnCollisionStay(pThis, pTarget, fTime);

		++iter;
	}
	return 0;
}

int CGameObject::OnCollisionExit(class CCollider* pThis, class CCollider* pTarget, float fTime)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		(*iter)->OnCollisionExit(pThis, pTarget, fTime);

		++iter;
	}
	return 0;
}

int CGameObject::OnMouseEnter(const Vector2 & vPosition)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		(*iter)->OnMouseEnter(vPosition);

		++iter;
	}
	return 0;
}

int CGameObject::OnMouseStay(const Vector2 & vPosition, float fTime)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		(*iter)->OnMouseStay(vPosition, fTime);

		++iter;
	}
	return 0;
}

int CGameObject::OnMouseExit(const Vector2 & vPosition)
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		(*iter)->OnMouseExit(vPosition);

		++iter;
	}
	return 0;
}

void CGameObject::Render(float fTime)
{

	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd;)
	{
		if ((*iter)->IsDestroy())
		{
			SAFE_RELEASE((*iter));
			iter = m_ComponentList.erase(iter);
			continue;
		}
		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Render(fTime);

		++iter;
	}
}

CGameObject * CGameObject::Clone() const
{
	return new CGameObject(*this);
}

bool CGameObject::CheckComponentFromType(COMPONENT_TYPE eType)
{
	list<CComponent*>::const_iterator	iter;
	list<CComponent*>::const_iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetComponentType() == eType)
			return true;
	}

	return false;
}

CComponent * CGameObject::AddComponent(CComponent * pComponent)
{
	pComponent->SetScene(m_pScene);
	pComponent->SetLayer(m_pLayer);
	pComponent->SetGameObject(this);
	pComponent->SetTransform(m_pTransform);
	pComponent->AddRef();

	m_ComponentList.push_back(pComponent);

	if (pComponent->GetComponentType() == CT_COLLIDER)
	{
		m_ColliderList.push_back(dynamic_cast<CCollider*>(pComponent));
	}
	return pComponent;
}

const list<class CCollider*>* CGameObject::GetColliderList() const
{
	return &m_ColliderList;
}

//여러 컴포넌트 타입을 캐싱해서 쓰면 안됨
const list<CComponent*>* CGameObject::GetComponentsFromTag(const string & strTag)
{
	m_FindComponentList.clear();

	list<CComponent*>::const_iterator	iter;
	list<CComponent*>::const_iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			m_FindComponentList.push_back(*iter);
		}
	}

	return &m_FindComponentList;
}


//여러 컴포넌트 타입을 캐싱해서 쓰면 안됨
//Collider 찾을 때 주의.
list<CComponent*>* CGameObject::GetComponentsFromType(int eType)
{
	m_FindComponentList.clear();

	list<CComponent*>::const_iterator	iter;
	list<CComponent*>::const_iterator	iterEnd = m_ComponentList.end();

	for (iter = m_ComponentList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetComponentType() == eType)
		{
			(*iter)->AddRef();
			m_FindComponentList.push_back(*iter);
		}
	}

	return &m_FindComponentList;
}
