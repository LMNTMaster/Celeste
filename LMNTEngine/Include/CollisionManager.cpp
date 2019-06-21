#include "CollisionManager.h"
#include "Core.h"
#include "Layer.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Component\ButtonCollider.h"
#include "Collider.h"
#include "CollisionVoxel2D.h"
#include "CollisionVoxel3D.h"
#include "GameObject.h"
#include "Input.h"


#include "Transform.h"



LMNT_USING

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager() :
	m_vVoxelSize2D(Vector2(256.0f, 256.0f)),
	m_vVoxelSize3D(Vector3(32.0f, 32.0f, 32.0f)),
	m_pCoveredButtonCollider(NULL),
	m_fFixedTimeStep(1.0f / 60.0f),
	m_fCullingDistance(1000.0f)
{
}


CCollisionManager::~CCollisionManager()
{

	{
		unordered_map<Vector2, CCollisionVoxel2D*>::iterator iter;
		unordered_map<Vector2, CCollisionVoxel2D*>::iterator iterEnd = m_mapCollisionVoxel2D.end();
		for (iter = m_mapCollisionVoxel2D.begin(); iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}
		m_mapCollisionVoxel2D.clear();
	}


	{
		unordered_map<Vector3, CCollisionVoxel3D*>::iterator iter;
		unordered_map<Vector3, CCollisionVoxel3D*>::iterator iterEnd = m_mapCollisionVoxel3D.end();
		for (iter = m_mapCollisionVoxel3D.begin(); iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}
		m_mapCollisionVoxel3D.clear();
	}

}

float CCollisionManager::GetCullingDistance() const
{
	return m_fCullingDistance;
}

void CCollisionManager::SetCullingDistance(float fDistance)
{
	m_fCullingDistance = fDistance;
}

float CCollisionManager::GetFixedTimeStep() const
{
	return m_fFixedTimeStep;
}

void CCollisionManager::SetFixedTimeStep(float fTimeStep)
{
	m_fFixedTimeStep = fTimeStep;
}

Vector2 CCollisionManager::GetVoxelSize2D() const
{
	return m_vVoxelSize2D;
}

Vector3 CCollisionManager::GetVoxelSize3D() const
{
	return m_vVoxelSize3D;
}

CCollisionVoxel2D * CCollisionManager::GetVoxel2D(const Vector2 & position) 
{
	unordered_map<Vector2, CCollisionVoxel2D*>::const_iterator iter = m_mapCollisionVoxel2D.find(position);

	if (iter == m_mapCollisionVoxel2D.end())
	{
		return CreateVoxel(position);
	}

	return iter->second;
}

CCollisionVoxel2D * CCollisionManager::GetVoxel2D(const Vector3 & position) 
{
	unordered_map<Vector2, CCollisionVoxel2D*>::const_iterator iter = m_mapCollisionVoxel2D.find(Vector2(position.x, position.y));

	if (iter == m_mapCollisionVoxel2D.end())
	{
		return CreateVoxel(Vector2(position.x , position.y));
	}

	return iter->second;
}

CCollisionVoxel2D * CCollisionManager::GetVoxel2D(float x, float y)
{
	unordered_map<Vector2, CCollisionVoxel2D*>::const_iterator iter = m_mapCollisionVoxel2D.find(Vector2(x, y));

	if (iter == m_mapCollisionVoxel2D.end())
	{
		return CreateVoxel(Vector2(x, y));
	}

	return iter->second;
}

CCollisionVoxel3D * CCollisionManager::GetVoxel3D(const Vector3 & position) 
{
	unordered_map<Vector3, CCollisionVoxel3D*>::const_iterator iter = m_mapCollisionVoxel3D.find(position);

	if (iter == m_mapCollisionVoxel3D.end())
	{
		return CreateVoxel(position);
	}

	return iter->second;
}

CCollisionVoxel3D * CCollisionManager::GetVoxel3D(float x, float y, float z) 
{
	unordered_map<Vector3, CCollisionVoxel3D*>::const_iterator iter = m_mapCollisionVoxel3D.find(Vector3(x, y, z));

	if (iter == m_mapCollisionVoxel3D.end())
	{
		return CreateVoxel(Vector3(x, y, z));
	}

	return iter->second;
}

void CCollisionManager::SetVoxelSize2D(const Vector2 & vSize)
{
	m_vVoxelSize2D = vSize;
}

void CCollisionManager::SetVoxelSize2D(float fWidth, float fHeight)
{
	m_vVoxelSize2D.x = fWidth;
	m_vVoxelSize2D.y = fHeight;
}

void CCollisionManager::SetVoxelSize3D(const Vector3 & vSize)
{
	m_vVoxelSize3D = vSize;	
}

void CCollisionManager::SetVoxelSize3D(float x, float y, float z)
{
	m_vVoxelSize3D.x = x;
	m_vVoxelSize3D.y = y;
	m_vVoxelSize3D.z = z;
}

void CCollisionManager::RemoveVoxel2D(const Vector2 & vKeyPosition)
{
	unordered_map<Vector2, CCollisionVoxel2D*>::iterator iter = m_mapCollisionVoxel2D.find(vKeyPosition);
	if (iter == m_mapCollisionVoxel2D.end())
		return;

	delete iter->second;
	m_mapCollisionVoxel2D.erase(iter);

}

void CCollisionManager::RemoveVoxel2D(float fKeyX, float fKeyY)
{
	unordered_map<Vector2, CCollisionVoxel2D*>::iterator iter = m_mapCollisionVoxel2D.find(Vector2(fKeyX, fKeyY));
	if (iter == m_mapCollisionVoxel2D.end())
		return;

	delete iter->second;
	m_mapCollisionVoxel2D.erase(iter);

}

void CCollisionManager::RemoveVoxel2D(CCollisionVoxel2D * pTarget)
{
}

void CCollisionManager::RemoveVoxel3D(const Vector3 & vKeyPosition)
{
	unordered_map<Vector3, CCollisionVoxel3D*>::iterator iter = m_mapCollisionVoxel3D.find(vKeyPosition);
	if (iter == m_mapCollisionVoxel3D.end())
		return;

	delete iter->second;
	m_mapCollisionVoxel3D.erase(iter);

}

void CCollisionManager::RemoveVoxel3D(float fKeyX, float fKeyY, float fKeyZ)
{
	unordered_map<Vector3, CCollisionVoxel3D*>::iterator iter = m_mapCollisionVoxel3D.find(Vector3(fKeyX, fKeyY, fKeyZ));
	if (iter == m_mapCollisionVoxel3D.end())
		return;

	delete iter->second;
	m_mapCollisionVoxel3D.erase(iter);

}


CCollisionVoxel3D* CCollisionManager::CreateVoxel(const Vector3& vKeyPosition)
{
	CCollisionVoxel3D* pVoxel;

	pVoxel = new CCollisionVoxel3D;
	m_mapCollisionVoxel3D.insert(make_pair(vKeyPosition, pVoxel));

	return pVoxel;
}

CCollisionVoxel2D* CCollisionManager::CreateVoxel(const Vector2& vKeyPosition)
{
	CCollisionVoxel2D* pVoxel;

	pVoxel = new CCollisionVoxel2D;
	pVoxel->SetKeyPosition(vKeyPosition);
	m_mapCollisionVoxel2D.insert(make_pair(vKeyPosition, pVoxel));

	return pVoxel;
}

void CCollisionManager::ClearVoxel2D()
{
	unordered_map<Vector2, CCollisionVoxel2D*>::iterator iter;
	unordered_map<Vector2, CCollisionVoxel2D*>::iterator iterEnd = m_mapCollisionVoxel2D.end();
	for (iter = m_mapCollisionVoxel2D.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	m_mapCollisionVoxel2D.clear();
}

void CCollisionManager::ClearVoxel3D()
{
	unordered_map<Vector3, CCollisionVoxel3D*>::iterator iter;
	unordered_map<Vector3, CCollisionVoxel3D*>::iterator iterEnd = m_mapCollisionVoxel3D.end();
	for (iter = m_mapCollisionVoxel3D.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	m_mapCollisionVoxel3D.clear();
}

bool CCollisionManager::Initialize()
{
	return true;
}

int CCollisionManager::UpdateCollision(float fTime)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	Vector2 vCursorPosition = GET_SINGLE(CInput)->GetCursorPosition(SPACE_DEVICE);
	{
		list<CButtonCollider*>::const_iterator iter;
		list<CButtonCollider*>::const_iterator iterEnd = pScene->GetButtonColliderList()->end();

		for (iter = pScene->GetButtonColliderList()->begin(); iter != iterEnd; ++iter)
		{
			CGameObject* pButton = (*iter)->GetGameObject();
			bool bEnable = pButton->IsEnable();
			SAFE_RELEASE(pButton);
			
			if ((*iter)->IsEnable() && bEnable)
			{
				bool bPrevCollision = (*iter)->GetCollision();
				bool bCurrentCollision = (*iter)->CheckCollision(vCursorPosition);

				if (!bPrevCollision && bCurrentCollision)
				{
					if (m_pCoveredButtonCollider)
					{
						CGameObject* pCoveredButton = m_pCoveredButtonCollider->GetGameObject();
						pCoveredButton->OnMouseExit(vCursorPosition);
						m_pCoveredButtonCollider->SetCollision(false);
						SAFE_RELEASE(pCoveredButton);

						CGameObject* pTargetButton = (*iter)->GetGameObject();
						pTargetButton->OnMouseEnter(vCursorPosition);
						(*iter)->SetCollision(true);
						m_pCoveredButtonCollider = (*iter);
						SAFE_RELEASE(pTargetButton);
					}
					else
					{
						CGameObject* pTargetButton = (*iter)->GetGameObject();
						pTargetButton->OnMouseEnter(vCursorPosition);
						(*iter)->SetCollision(true);
						m_pCoveredButtonCollider = (*iter);
						SAFE_RELEASE(pTargetButton);
					}
				}
				else if (bPrevCollision && bCurrentCollision)
				{
					CGameObject* pTargetButton = (*iter)->GetGameObject();
					pTargetButton->OnMouseStay(vCursorPosition, fTime);								
					SAFE_RELEASE(pTargetButton);
				}
				else if (bPrevCollision && !bCurrentCollision)
				{
					CGameObject* pTargetButton = (*iter)->GetGameObject();
					pTargetButton->OnMouseExit(vCursorPosition);
					(*iter)->SetCollision(false);
					m_pCoveredButtonCollider = NULL;
					SAFE_RELEASE(pTargetButton);
				}
				else 
				{
					continue;
				}
			}
		}
	}







	CTransform* pCameraTransform = pScene->GetMainCameraTransform();

	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	SAFE_RELEASE(pCameraTransform);

	{
		//레이어를 돌면서, 각 Collider의 자신이 포함되는 Voxel을 업데이트 함.
		//Voxel은 Collider 내부에서 먼저 전부 지우고, 재할당 한다.
	
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	
		list<CCollider*>::const_iterator iter;
		list<CCollider*>::const_iterator iterEnd = pScene->GetColliderList()->end();
	
		for (iter = pScene->GetColliderList()->begin(); iter != iterEnd; ++iter)
		{
			(*iter)->SetCollisionVoxel();				
		}

		SAFE_RELEASE(pScene);
	}



	{
		unordered_map<Vector2, CCollisionVoxel2D*>::iterator iter;
		unordered_map<Vector2, CCollisionVoxel2D*>::iterator iterEnd = m_mapCollisionVoxel2D.end();

		for (iter = m_mapCollisionVoxel2D.begin(); iter != iterEnd;)
		{
			size_t iCount = (*iter).second->GetContainedColliderCount();
			
			if (iCount == 0)
			{
				delete (*iter).second;
				iter = m_mapCollisionVoxel2D.erase(iter);
				continue;			
			}			
			//else if (iCount < 2)
			//{				
			//	unordered_map<CCollider*, CCollider*>::const_iterator iterTemp = (*iter).second->m_mapContainedCollider.begin();
			//	size_t iCollisionCount = iterTemp->second->m_mapCollision.size();
			//	
			//	//타일의 오브젝트가 1개일 때, 이전에 충돌한 오브젝트가 1개 이상이면
			//	//이전에 충돌한 오브젝트와 Exit 처리하도록 한다.
			//	if (iCollisionCount > 0)
			//	{
			//		CGameObject* pGameObjectA = iterTemp->second->GetGameObject();
			//		CCollider* pColliderA = iterTemp->second;
			//	
			//		unordered_map<CCollider*, CCollider*>::const_iterator iterTarget = iterTemp->second->m_mapCollision.begin();
			//		unordered_map	<CCollider*, CCollider*>::const_iterator iterTargetEnd = iterTemp->second->m_mapCollision.end();
			//	
			//		for (; iterTarget != iterTargetEnd; )
			//		{
			//			CGameObject* pGameObjectB = iterTarget->second->GetGameObject();
			//			CCollider* pColliderB = iterTarget->second;
			//	
			//			pGameObjectA->OnCollisionExit(pColliderB, fTime);
			//			pGameObjectB->OnCollisionExit(pColliderA, fTime);
			//	
			//			iterTarget = pColliderA->m_mapCollision.erase(iterTarget);
			//			pColliderB->m_mapCollision.erase(pColliderB->m_mapCollision.find(pColliderA));
			//			iterTargetEnd = pColliderA->m_mapCollision.end();
			//	
			//			SAFE_RELEASE(pGameObjectB);
			//		}
			//		SAFE_RELEASE(pGameObjectA);
			//	}
			//
			//	++iter;
			//	continue;
			//}
			else
			{
				//각 충돌체끼리 충돌하고, 함수 호출 해줌
				unordered_map<CCollider*, CCollider*>::iterator iterA = (*iter).second->GetContainedColliders()->begin();
				unordered_map<CCollider*, CCollider*>::iterator iterB = (*iter).second->GetContainedColliders()->begin();
				unordered_map<CCollider*, CCollider*>::iterator iterAEnd = (*iter).second->GetContainedColliders()->end();
				unordered_map<CCollider*, CCollider*>::iterator iterBEnd = (*iter).second->GetContainedColliders()->end();

				--iterAEnd;
				//O(n * (n / 2))s
				for (; iterA != iterAEnd; ++iterA)
				{
					iterB = ++iterA;
					--iterA;
					for (; iterB != iterBEnd; ++iterB)
					{
						CCollider* pColliderA = (*iterA).second;
						CCollider* pColliderB = (*iterB).second;

						if (!pColliderA || !pColliderB)
						{
							continue;
						}
						
						CGameObject* pGameObjectA = pColliderA->GetGameObject();
						CGameObject* pGameObjectB = pColliderB->GetGameObject();

						CLayer* pLayerA = pGameObjectA->GetLayer();
						CLayer* pLayerB = pGameObjectB->GetLayer();

						if (pGameObjectA != pGameObjectB)
						{
							if (pScene->GetLayerCollision(pLayerA, pLayerB))
							{
								bool isCollide = pColliderA->CollisionCheck(pColliderB);
								bool wasCollided = pColliderA->ContainCollision(pColliderB);

								if (isCollide && !wasCollided)
								{
									pGameObjectA->OnCollisionEnter(pColliderA, pColliderB, fTime);
									pGameObjectB->OnCollisionEnter(pColliderB, pColliderA, fTime);
						
									pColliderA->AddCollisionList(pColliderB);
									pColliderB->AddCollisionList(pColliderA);
								}
								else if (isCollide && wasCollided)
								{
									pGameObjectA->OnCollisionStay(pColliderA, pColliderB, fTime);
									pGameObjectB->OnCollisionStay(pColliderB, pColliderA, fTime);
						
									pColliderA->AddCollisionList(pColliderB);
									pColliderB->AddCollisionList(pColliderA);
								}
								else if (!isCollide && wasCollided)
								{
									pGameObjectA->OnCollisionExit(pColliderA, pColliderB, fTime);
									pGameObjectB->OnCollisionExit(pColliderB, pColliderA, fTime);
						
									pColliderA->RemoveTargetFromCollisionList(pColliderB);
									pColliderB->RemoveTargetFromCollisionList(pColliderA);
								}
							}
						}

						SAFE_RELEASE(pLayerA);
						SAFE_RELEASE(pLayerB);
						SAFE_RELEASE(pGameObjectA);
						SAFE_RELEASE(pGameObjectB);
					}
				}						
				++iter;
			}
		}
	}

	{
		unordered_map<Vector3, CCollisionVoxel3D*>::iterator iter;
		unordered_map<Vector3, CCollisionVoxel3D*>::iterator iterEnd = m_mapCollisionVoxel3D.end();
		
		for (iter = m_mapCollisionVoxel3D.begin(); iter != iterEnd; ++iter)
		{

		}
	}

	SAFE_RELEASE(pScene);
	
	return 0;
}
