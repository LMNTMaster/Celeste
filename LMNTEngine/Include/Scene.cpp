#include "Scene.h"
#include "Component\SceneComponent.h"
#include "Layer.h"
#include "GameObject.h"
#include "Device.h"
#include "Component\Camera.h"
#include "Transform.h"
#include "Collider.h"
#include "Component\ButtonCollider.h"

LMNT_USING

CScene::CScene():
	m_bInitialized(false),
	m_pCollisionMatrix(NULL),
	m_iIDCounter(0)
{
	CLayer*	pLayer = CreateLayer("Default");
	SAFE_RELEASE(pLayer);

	pLayer = CreateLayer("UI", INT_MAX);
	SAFE_RELEASE(pLayer);

	// 기본 메인카메라 생성
	m_pMainCamera = CreateCamera("Main Camera", Vector3(0.f, 0.f, -5.f));

	CCamera*	pCamera = m_pMainCamera->GetComponentFromType<CCamera>(CT_CAMERA);

	pCamera->SetPerspectiveProjection(DEVICE_RESOLUTION.iWidth / (float)DEVICE_RESOLUTION.iHeight, PI / 3.f);
	pCamera->SetOrthographyProjection(0.f, (float)DEVICE_RESOLUTION.iWidth, 0.f, (float)DEVICE_RESOLUTION.iHeight, -100.0f);

	SAFE_RELEASE(pCamera);
}

CScene::~CScene()
{
	CGameObject::ErasePrototypeList(this);
	SAFE_RELEASE(m_pMainCamera);
	Safe_Release_Map(m_mapCamera);
	Safe_Release_VecList(m_vecLayer);
	//Safe_Release_VecList(m_SceneComponentList);
	//Safe_Release_VecList(m_ColliderList);
	m_ColliderList.clear();

	if (m_pCollisionMatrix)
	{
		delete[] m_pCollisionMatrix;
		m_pCollisionMatrix = NULL;
	}
}

void CScene::AddGameObject(class CGameObject * pGameObject)
{
	pGameObject->m_iID = m_iIDCounter;
	++m_iIDCounter;
	m_vecGameObject.push_back(pGameObject);
}

void CScene::RemoveGameObject(class CGameObject * pGameObject)
{
	size_t iCount = m_vecGameObject.size();
	for (size_t i = 0; i < iCount; ++i)
	{
		if (m_vecGameObject[i] == pGameObject)
		{
			m_vecGameObject.erase(m_vecGameObject.begin() + i);
			break;
		}
	}
}

void CScene::AddCollisionMatrix()
{
	size_t iMatrixSize = m_vecLayer.size();

	bool* pMatrix = new bool[iMatrixSize * iMatrixSize];

	memset(pMatrix, 0, iMatrixSize * iMatrixSize);

	if (m_pCollisionMatrix)
	{	
		delete[] m_pCollisionMatrix;
		m_pCollisionMatrix = NULL;
	}

	m_pCollisionMatrix = pMatrix;
}

void CScene::SetCollisionMatrix(CLayer * pLayer1, CLayer * pLayer2, bool bCollision)
{
	int iLayer1Index = 0;
	int iLayer2Index = 0;

	for (size_t i = 0; i < m_vecLayer.size(); ++i)
	{
		if (m_vecLayer[i] == pLayer1)
		{
			iLayer1Index = i;
		}
		if (m_vecLayer[i] == pLayer2)
		{
			iLayer2Index = i;
		}
	}
	m_pCollisionMatrix[iLayer1Index * m_vecLayer.size() + iLayer2Index] = bCollision;
	m_pCollisionMatrix[iLayer2Index * m_vecLayer.size() + iLayer1Index] = bCollision;
}

bool CScene::GetLayerCollision(CLayer * pLayer1, CLayer* pLayer2)
{
	int iLayer1Index = 0;
	int iLayer2Index = 0;
	for (size_t i = 0; i < m_vecLayer.size(); ++i)
	{
		if (m_vecLayer[i] == pLayer1)
		{
			iLayer1Index = i;
		}
		if (m_vecLayer[i] == pLayer2)
		{
			iLayer2Index = i;
		}
	}
	return m_pCollisionMatrix[iLayer1Index * m_vecLayer.size() + iLayer2Index];
}

CLayer * CScene::CreateLayer(const string & strTag, int iZOrder)
{
	CLayer*	pLayer = new CLayer;

	pLayer->SetTag(strTag);
	pLayer->SetZOrder(iZOrder);
	pLayer->SetScene(this);

	if (!pLayer->Initialize())
	{
		SAFE_RELEASE(pLayer);
		return NULL;
	}

	pLayer->AddRef();
	m_vecLayer.push_back(pLayer);
	AddCollisionMatrix();

	if (m_vecLayer.size() >= 2)
	{
		sort(m_vecLayer.begin(), m_vecLayer.end(), CScene::LayerSort);
	}
	return pLayer;
}

CLayer * CScene::FindLayer(unsigned int iIndex) const
{
	m_vecLayer[iIndex]->AddRef();
	return m_vecLayer[iIndex];
}

CLayer * CScene::FindLayer(const string & strTag)
{
	for (size_t i = 0; i < m_vecLayer.size(); ++i)
	{
		if (m_vecLayer[i]->GetTag() == strTag)
		{
			m_vecLayer[i]->AddRef();
			return m_vecLayer[i];
		}
	}

	return NULL;
}

size_t CScene::GetLayerCount() const
{
	return m_vecLayer.size();
}

bool CScene::ChangeLayerZOrder(const string & strTag, int iZOrder)
{
	CLayer*	pLayer = FindLayer(strTag);

	if (!pLayer)
		return false;

	pLayer->SetZOrder(iZOrder);

	SAFE_RELEASE(pLayer);

	if (m_vecLayer.size() >= 2)
		sort(m_vecLayer.begin(), m_vecLayer.end(), CScene::LayerSort);

	return true;
}

CGameObject * CScene::CreateCamera(const string & strKey, const Vector3 & vPos, const Vector3 & vRot)
{
	CGameObject*	pCameraObject = FindCameraObject(strKey);

	if (pCameraObject)
		return pCameraObject;

	pCameraObject = CGameObject::CreateObject(strKey);

	CTransform*	pTransform = pCameraObject->GetTransform();
	
	pTransform->SetWorldPosition(vPos);
	pTransform->SetWorldRotation(vRot);
	
	SAFE_RELEASE(pTransform);

	CCamera*	pCamera = pCameraObject->AddComponent<CCamera>(strKey);

	SAFE_RELEASE(pCamera);

	pCameraObject->AddRef();

	m_mapCamera.insert(make_pair(strKey, pCameraObject));

	return pCameraObject;
}

CGameObject * CScene::FindCameraObject(const string & strKey)
{
	unordered_map<string, class CGameObject*>::iterator	iter = m_mapCamera.find(strKey);

	if (iter == m_mapCamera.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CCamera * CScene::FindCamera(const string & strKey)
{
	CGameObject*	pCameraObj = FindCameraObject(strKey);

	if (!pCameraObj)
		return NULL;

	CCamera*	pCamera = pCameraObj->GetComponentFromType<CCamera>(CT_CAMERA);

	SAFE_RELEASE(pCameraObj);

	return pCamera;
}

CTransform * CScene::FindCameraTransform(const string & strKey)
{
	CGameObject*	pCameraObject = FindCameraObject(strKey);

	if (!pCameraObject)
		return NULL;

	CTransform*	pTransform = pCameraObject->GetTransform();
	
	SAFE_RELEASE(pCameraObject);

	return pTransform;
	return NULL;
}

CGameObject * CScene::GetMainCameraObject()
{
	m_pMainCamera->AddRef();
	return m_pMainCamera;
}

CCamera * CScene::GetMainCamera()
{
	return m_pMainCamera->GetComponentFromType<CCamera>(CT_CAMERA);
}

CTransform * CScene::GetMainCameraTransform()
{
	//return NULL;
	CTransform* pTransform = m_pMainCamera->GetTransform();
	return pTransform;
}

const list<class CCollider*>* CScene::GetColliderList() const
{
	return &m_ColliderList;
}

const list<class CButtonCollider*>* CScene::GetButtonColliderList() const
{
	return &m_ButtonColliderList;
}

void CScene::RemoveCollider(CCollider * pCollider)
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for(iter= m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pCollider)
		{
			m_ColliderList.erase(iter);
			return;
		}
	}
}

void CScene::RemoveButtonCollider(CButtonCollider * pCollider)
{
	list<CButtonCollider*>::iterator iter;
	list<CButtonCollider*>::iterator iterEnd = m_ButtonColliderList.end();

	for (iter = m_ButtonColliderList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pCollider)
		{
			m_ButtonColliderList.erase(iter);
			return;
		}
	}
}

void CScene::AddCollider(CCollider * pCollider)
{
	m_ColliderList.push_back(pCollider);
}

void CScene::AddButtonCollider(CButtonCollider * pCollider)
{
	m_ButtonColliderList.push_back(pCollider);
}

bool CScene::Save(FILE * pFile)
{
	int iLayerCount = m_vecLayer.size();
	fwrite(&iLayerCount, 4, 1, pFile);
	//현재 Scene의 오브젝트 카운터
	fwrite(&m_iIDCounter, 4, 1, pFile);
	int iObjectCount = m_vecGameObject.size();
	fwrite(&iObjectCount, 4, 1, pFile);

	//로드 중 레이어간의 상호작용이 있을 가능성이 있으므로, 레이어를 우선 적으로 생성할 수 있도록
	//먼저 Scene의 Layer정보부터 저장한다.
	for (int i = 0; i < iLayerCount; ++i)
	{
		int iLength = m_vecLayer[i]->GetTag().length();
		int iZ = m_vecLayer[i]->GetZOrder();
		
		fwrite(&iLength, 4, 1, pFile);
		fwrite(m_vecLayer[i]->GetTag().c_str(), sizeof(char), iLength, pFile);
		fwrite(&iZ, 4, 1, pFile);
	}

	for (int i = 0; i < iObjectCount; ++i)
	{
		m_vecGameObject[i]->Save(pFile);
	}
	return true;
}

bool CScene::Load(FILE * pFile)
{
	vector<CLayer*> vecTempLayer;
	int iLayerCount = 0;
	fread(&iLayerCount, 4, 1, pFile);	
	fread(&m_iIDCounter, 4, 1, pFile);
	int iObjectCount = 0;
	fread(&iObjectCount, 4, 1, pFile);

	for (int i = 0; i < iLayerCount; ++i)
	{
		int iLength = 0;
		int iZ = 0;
		char cBuffer[256] = {};
		fread(&iLength, 4, 1, pFile);
		fread(&cBuffer[0], sizeof(char), iLength, pFile);
		fread(&iZ, 4, 1, pFile);

		CLayer* pLayer = FindLayer(cBuffer);
		if (pLayer)
		{
			//이미 존재하는 레이어일 경우, Z오더를 변경해준다.
			pLayer->SetZOrder(iZ);
		}
		else
		{
			//존재하지 않는 레이어일 경우, 새로 만들어준다.
			pLayer = CreateLayer(string(cBuffer), iZ);
		}
		vecTempLayer.push_back(pLayer);
		SAFE_RELEASE(pLayer);
	}

	//모든 오브젝트를 읽어온다.
	for (int i = 0; i < iObjectCount; ++i)
	{
		CGameObject* pGameObject = new CGameObject;
		pGameObject->Load(pFile);
	}

	//오브젝트를 돌면서 계층구조를 정리한다.


	return true;
}

bool CScene::LayerSort(CLayer * p1, CLayer * p2)
{
	return p1->GetZOrder() < p2->GetZOrder();
}

bool CScene::Initialize()
{
	return true;
}
void CScene::Start()
{
}

int CScene::Input(float fTime)
{
	//{
	//	list<CSceneComponent*>::iterator iter;
	//	list<CSceneComponent*>::iterator iterEnd = m_SceneComponentList.end();
	//	for (iter = m_SceneComponentList.begin(); iter != iterEnd;)
	//	{
	//		if ((*iter)->IsDestroy())
	//		{
	//			SAFE_RELEASE((*iter));
	//			iter = m_SceneComponentList.erase(iter);
	//			continue;
	//		}
	//		else if (!(*iter)->IsEnable())
	//		{
	//			++iter;
	//			continue;
	//		}
	//
	//		(*iter)->Input(fTime);
	//		++iter;
	//	}
	//}

	{
		vector<CLayer*>::iterator	iter;
		vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

		for (iter = m_vecLayer.begin(); iter != iterEnd;)
		{
			if ((*iter)->IsDestroy())
			{
				SAFE_RELEASE((*iter));
				iter = m_vecLayer.erase(iter);
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
	}
	
	m_pMainCamera->Input(fTime);

	return 0;
}


int CScene::Update(float fTime)
{
	//{
	//	list<CSceneComponent*>::iterator iter;
	//	list<CSceneComponent*>::iterator iterEnd = m_SceneComponentList.end();
	//	for (iter = m_SceneComponentList.begin(); iter != iterEnd;)
	//	{
	//		if ((*iter)->IsDestroy())
	//		{
	//			SAFE_RELEASE((*iter));
	//			iter = m_SceneComponentList.erase(iter);
	//			continue;
	//		}
	//		else if (!(*iter)->IsEnable())
	//		{
	//			++iter;
	//			continue;
	//		}
	//
	//		(*iter)->Update(fTime);
	//		++iter;
	//	}
	//}

	{
		vector<CLayer*>::iterator	iter;
		vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

		for (iter = m_vecLayer.begin(); iter != iterEnd;)
		{
			if ((*iter)->IsDestroy())
			{
				SAFE_RELEASE((*iter));
				iter = m_vecLayer.erase(iter);
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
	}

	m_pMainCamera->Update(fTime);

	return 0;
}

int CScene::LateUpdate(float fTime)
{
	//{
	//	list<CSceneComponent*>::iterator iter;
	//	list<CSceneComponent*>::iterator iterEnd = m_SceneComponentList.end();
	//	for (iter = m_SceneComponentList.begin(); iter != iterEnd;)
	//	{
	//		if ((*iter)->IsDestroy())
	//		{
	//			SAFE_RELEASE((*iter));
	//			iter = m_SceneComponentList.erase(iter);
	//			continue;
	//		}
	//		else if (!(*iter)->IsEnable())
	//		{
	//			++iter;
	//			continue;
	//		}
	//
	//		(*iter)->LateUpdate(fTime);
	//		++iter;
	//	}
	//}

	{
		vector<CLayer*>::iterator	iter;
		vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

		for (iter = m_vecLayer.begin(); iter != iterEnd;)
		{
			if ((*iter)->IsDestroy())
			{
				SAFE_RELEASE((*iter));
				iter = m_vecLayer.erase(iter);
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
	}

	m_pMainCamera->LateUpdate(fTime);

	return 0;
}

int CScene::Collision(float fTime)
{
	return 0;
}

void CScene::Render(float fTime)
{
	//{
	//	list<CSceneComponent*>::iterator iter;
	//	list<CSceneComponent*>::iterator iterEnd = m_SceneComponentList.end();
	//	for (iter = m_SceneComponentList.begin(); iter != iterEnd;)
	//	{
	//		if ((*iter)->IsDestroy())
	//		{
	//			SAFE_RELEASE((*iter));
	//			iter = m_SceneComponentList.erase(iter);
	//			continue;
	//		}
	//		else if (!(*iter)->IsEnable())
	//		{
	//			++iter;
	//			continue;
	//		}
	//
	//		(*iter)->Render(fTime);
	//		++iter;
	//	}
	//}


	m_pMainCamera->Render(fTime);
	

	{
		vector<CLayer*>::iterator	iter;
		vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

		for (iter = m_vecLayer.begin(); iter != iterEnd;)
		{
			if ((*iter)->IsDestroy())
			{
				SAFE_RELEASE((*iter));
				iter = m_vecLayer.erase(iter);
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



}
