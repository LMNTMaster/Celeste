#include "stdafx.h"
#include "EditorCursor.h"
#include "Input.h"
#include "Transform.h"
#include "Component.h"
#include "GameObject.h"
#include "Component/RectCollider2D.h"
#include "MainFrm.h"
#include "Inspector.h"
#include "Transmitter.h"
#include "Layer.h"
#include "Scene.h"
#include "CollisionManager.h"
#include "CollisionVoxel2D.h"
#include "Component/Renderer.h"


CEditorCursor::CEditorCursor():
	m_pDragTarget(NULL),
	m_bOnDrag(false),
	m_bCameraMove(false)
{
	m_eComponentType = CT_CURSOR;
	SetTag("Editor Cursor");
}


CEditorCursor::~CEditorCursor()
{
}

bool CEditorCursor::CheckCollisionList(class CGameObject* pTarget) const
{
	list<CGameObject*>::const_iterator iter;
	list<CGameObject*>::const_iterator iterEnd = m_CollisionList.end();

	for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
	{
		if (pTarget == *iter)
		{
			return false;
		}
	}
	return true;
}

void CEditorCursor::RemoveFromCollisionList(CGameObject * pTarget)
{
	list<CGameObject*>::const_iterator iter;
	list<CGameObject*>::const_iterator iterEnd = m_CollisionList.end();

	for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
	{
		if (pTarget == *iter)
		{
			m_CollisionList.erase(iter);
			break;
		}
	}
}

CGameObject * CEditorCursor::TopObject() const
{
	list<CGameObject*>::const_iterator iter;
	list<CGameObject*>::const_iterator iterEnd = m_CollisionList.end();

	int iLayer = INT_MIN;
	
	CGameObject* pTop = NULL;

	for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
	{
		CLayer* pLayer = (*iter)->GetLayer();
		if (iLayer < pLayer->GetZOrder())
		{
			iLayer = pLayer->GetZOrder();
			pTop = *iter;
		}
		SAFE_RELEASE(pLayer);
	}

	return pTop;
}

bool CEditorCursor::Initialize()
{
	//CCollider* pCollider = m_pGameObject->AddComponent<CRectCollider2D>("CursorCollider");
	//pCollider->SetScale(2.0f, 2.0f, 2.0f);
	//SAFE_RELEASE(pCollider);


	vPrevCursorPosition = GET_SINGLE(CInput)->GetCursorPosition(SPACE_DEVICE);
	return true;
}

int CEditorCursor::Update(float fTime)
{
	if (GETKEY(VK_LMENU))
	{
		if (GETKEYDOWN('1'))
		{			
			GET_SINGLE(CTransmitter)->m_pMainFrame->m_pInspector->CreateDanger(DIR_TOP);
		}
		if (GETKEYDOWN('2'))
		{
			GET_SINGLE(CTransmitter)->m_pMainFrame->m_pInspector->CreateDanger(DIR_RIGHT);
		}
		if (GETKEYDOWN('3'))
		{
			GET_SINGLE(CTransmitter)->m_pMainFrame->m_pInspector->CreateDanger(DIR_LEFT);
		}
		if (GETKEYDOWN('4'))
		{
			GET_SINGLE(CTransmitter)->m_pMainFrame->m_pInspector->CreateDanger(DIR_BOTTOM);
		}
	}

	{
		//Area
		if (GETKEYDOWN(VK_F1))
		{
			CInspector* pInspector = GET_SINGLE(CTransmitter)->m_pMainFrame->m_pInspector;
			pInspector->m_bShowArea = !pInspector->m_bShowArea;

			int iSize = pInspector->m_vecArea.size();
			for (int i = 0; i < iSize; ++i)
			{
				pInspector->m_vecArea[i]->Enable(pInspector->m_bShowArea);
			}
		}
		//Colliders
		if (GETKEYDOWN(VK_F2))
		{
			CInspector* pInspector = GET_SINGLE(CTransmitter)->m_pMainFrame->m_pInspector;
			pInspector->m_bShowCollider = !pInspector->m_bShowCollider;

			{
				CLayer* pLayer = m_pScene->FindLayer("Collider");
				list<CGameObject*>::const_iterator iter = pLayer->GetAllGameObjects()->begin();
				list<CGameObject*>::const_iterator iterEnd = pLayer->GetAllGameObjects()->end();

				for (; iter != iterEnd; ++iter)
				{
					(*iter)->Enable(pInspector->m_bShowCollider);
				}
				SAFE_RELEASE(pLayer);
			}
		}

		//Background
		if (GETKEYDOWN(VK_F3))
		{
			CInspector* pInspector = GET_SINGLE(CTransmitter)->m_pMainFrame->m_pInspector;
			pInspector->m_bShowBackground = !pInspector->m_bShowBackground;

			{
				CLayer* pLayer = m_pScene->FindLayer("BG0");
				list<CGameObject*>::const_iterator iter = pLayer->GetAllGameObjects()->begin();
				list<CGameObject*>::const_iterator iterEnd = pLayer->GetAllGameObjects()->end();

				for (; iter != iterEnd; ++iter)
				{
					(*iter)->Enable(pInspector->m_bShowBackground);
				}
				SAFE_RELEASE(pLayer);
			}
			{
				CLayer* pLayer = m_pScene->FindLayer("BG1");
				list<CGameObject*>::const_iterator iter = pLayer->GetAllGameObjects()->begin();
				list<CGameObject*>::const_iterator iterEnd = pLayer->GetAllGameObjects()->end();

				for (; iter != iterEnd; ++iter)
				{
					(*iter)->Enable(pInspector->m_bShowBackground);
				}
				SAFE_RELEASE(pLayer);
			}
			{
				CLayer* pLayer = m_pScene->FindLayer("BG2");
				list<CGameObject*>::const_iterator iter = pLayer->GetAllGameObjects()->begin();
				list<CGameObject*>::const_iterator iterEnd = pLayer->GetAllGameObjects()->end();

				for (; iter != iterEnd; ++iter)
				{
					(*iter)->Enable(pInspector->m_bShowBackground);
				}
				SAFE_RELEASE(pLayer);
			}
			{
				CLayer* pLayer = m_pScene->FindLayer("BG3");
				list<CGameObject*>::const_iterator iter = pLayer->GetAllGameObjects()->begin();
				list<CGameObject*>::const_iterator iterEnd = pLayer->GetAllGameObjects()->end();

				for (; iter != iterEnd; ++iter)
				{
					(*iter)->Enable(pInspector->m_bShowBackground);
				}
				SAFE_RELEASE(pLayer);
			}
			{
				CLayer* pLayer = m_pScene->FindLayer("BG4");
				list<CGameObject*>::const_iterator iter = pLayer->GetAllGameObjects()->begin();
				list<CGameObject*>::const_iterator iterEnd = pLayer->GetAllGameObjects()->end();

				for (; iter != iterEnd; ++iter)
				{
					(*iter)->Enable(pInspector->m_bShowBackground);
				}
				SAFE_RELEASE(pLayer);
			}
			{
				CLayer* pLayer = m_pScene->FindLayer("BG5");
				list<CGameObject*>::const_iterator iter = pLayer->GetAllGameObjects()->begin();
				list<CGameObject*>::const_iterator iterEnd = pLayer->GetAllGameObjects()->end();

				for (; iter != iterEnd; ++iter)
				{
					(*iter)->Enable(pInspector->m_bShowBackground);
				}
				SAFE_RELEASE(pLayer);
			}
		}

		//Objects / Tiles
		if (GETKEYDOWN(VK_F4))
		{
			CInspector* pInspector = GET_SINGLE(CTransmitter)->m_pMainFrame->m_pInspector;
			pInspector->m_bShowTerrain = !pInspector->m_bShowTerrain;

			{
				CLayer* pLayer = m_pScene->FindLayer("BackTerrain");
				list<CGameObject*>::const_iterator iter = pLayer->GetAllGameObjects()->begin();
				list<CGameObject*>::const_iterator iterEnd = pLayer->GetAllGameObjects()->end();

				for (; iter != iterEnd; ++iter)
				{
					(*iter)->Enable(pInspector->m_bShowTerrain);
				}
				SAFE_RELEASE(pLayer);
			}
			{
				CLayer* pLayer = m_pScene->FindLayer("Default");
				list<CGameObject*>::const_iterator iter = pLayer->GetAllGameObjects()->begin();
				list<CGameObject*>::const_iterator iterEnd = pLayer->GetAllGameObjects()->end();

				for (; iter != iterEnd; ++iter)
				{
					(*iter)->Enable(pInspector->m_bShowTerrain);
				}
				SAFE_RELEASE(pLayer);
			}
			{
				CLayer* pLayer = m_pScene->FindLayer("ForeTerrain");
				list<CGameObject*>::const_iterator iter = pLayer->GetAllGameObjects()->begin();
				list<CGameObject*>::const_iterator iterEnd = pLayer->GetAllGameObjects()->end();

				for (; iter != iterEnd; ++iter)
				{
					(*iter)->Enable(pInspector->m_bShowTerrain);
				}
				SAFE_RELEASE(pLayer);
			}
		}

	}


	if (GETKEYDOWN(VK_UP))
	{
		if (GETKEY(VK_LSHIFT))
		{
			if (GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap)
			{
				CTransform* pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -10.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap1->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap2->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap3->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap4->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap5->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap6->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap7->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
			}
		}
		else
		{
			if (GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap)
			{
				CTransform* pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -1.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap1->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap2->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap3->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap4->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap5->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap6->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap7->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, -1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
			}
		}
	}
	if (GETKEYDOWN(VK_DOWN))
	{
		if (GETKEY(VK_LSHIFT))
		{
			if (GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap)
			{
				CTransform* pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap1->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap2->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap3->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap4->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap5->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap6->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap7->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 10.0f, 0.0f));
				SAFE_RELEASE(pTransform);
			}
		}
		else
		{
			if (GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap)
			{
				CTransform* pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap1->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap2->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap3->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap4->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap5->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap6->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap7->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(0.0f, 1.0f, 0.0f));
				SAFE_RELEASE(pTransform);
			}
		}
	}

	if (GETKEYDOWN(VK_LEFT))
	{
		if (GETKEY(VK_LSHIFT))
		{
			if (GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap)
			{
				CTransform* pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap1->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap2->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap3->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap4->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap5->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap6->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap7->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
			}
		}
		else
		{
			if (GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap)
			{
				CTransform* pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap1->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap2->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap3->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap4->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap5->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap6->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap7->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(-1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);			
			}
		}
	}

	if (GETKEYDOWN(VK_RIGHT))
	{
		if (GETKEY(VK_LSHIFT))
		{
			if (GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap)
			{
				CTransform* pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap1->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap2->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap3->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap4->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap5->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap6->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap7->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(10.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
			}
		}
		else
		{
			if (GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap)
			{
				CTransform* pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap1->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap2->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap3->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap4->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap5->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap6->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);

				pTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pBackgroundMap7->GetTransform();
				pTransform->SetWorldPosition(pTransform->GetWorldPosition() + Vector3(1.0f, 0.0f, 0.0f));
				SAFE_RELEASE(pTransform);
			}
		}
	}


	if (GET_SINGLE(CTransmitter)->GetMainFrame()->GetFocus())
	{

		CTransform* pCameraTransform = m_pScene->GetMainCameraTransform();
		Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
		SAFE_RELEASE(pCameraTransform);

		Vector2 vCursorPosition = GET_SINGLE(CInput)->GetCursorPosition(SPACE_DEVICE);
		m_pTransform->SetWorldPosition(Vector3(vCursorPosition.x, vCursorPosition.y, 0));
		Vector2 vDeltaCursorPosition = vPrevCursorPosition - vCursorPosition;
		RECT screen = GET_SINGLE(CTransmitter)->GetMainFrame()->m_tRect;
		Vector2 vCursorWorldPosition = vCursorPosition + Vector2(vCameraPosition.x, vCameraPosition.y);
		//CLayer* pLayer = m_pScene->FindLayer("Default");

		RECT tScreenRect = {};
		CMainFrame* pMainFrame = GET_SINGLE(CTransmitter)->GetMainFrame();
		tScreenRect = pMainFrame->m_tRect;

		if (GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pTargetGameObject)
		{
			if (GETKEYDOWN(VK_DELETE))
			{
				if (GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pTargetGameObject->CheckComponentFromType(CT_TILE))
				{
					CTransform* pTileTransform = GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pTargetGameObject->GetTransform();
					Vector3 vPos = pTileTransform->GetWorldPosition();
					SAFE_RELEASE(pTileTransform);
					//GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->RemoveTile((int)vPos.x, (int)vPos.y);
				}
				GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pTargetGameObject->Destroy(true);
				GET_SINGLE(CTransmitter)->GetMainFrame()->m_pInspector->m_pTargetGameObject = NULL;
			}
		}

		if (vCursorPosition.x > 0 && vCursorPosition.x <= tScreenRect.right && vCursorPosition.y > 0 && vCursorPosition.y <= tScreenRect.bottom)
		{
			size_t iLayerCount = m_pScene->GetLayerCount();

			bool bEnter = false;

			for (size_t i = 0; i < iLayerCount; ++i)
			{
				CLayer* pCurrentLayer = m_pScene->FindLayer(i);
				if (pCurrentLayer->GetTag() == "Grid")
				{
					SAFE_RELEASE(pCurrentLayer);
					continue;
				}
				if (pCurrentLayer->GetTag() == "UI")
				{
					SAFE_RELEASE(pCurrentLayer);
					continue;
				}
				if (pCurrentLayer->GetTag() == "BackgroundMap")
				{
					SAFE_RELEASE(pCurrentLayer);
					continue;
				}


				list<CGameObject*>::const_iterator iter = pCurrentLayer->GetAllGameObjects()->begin();
				list<CGameObject*>::const_iterator iterEnd = pCurrentLayer->GetAllGameObjects()->end();

				for (; iter != iterEnd; ++iter)
				{					
					if ((*iter)->GetTag() == "Cursor")
						continue;
					if (!(*iter)->IsEnable())
						continue;

					//CCollider* pTarget = *iter;
					RECT rc = {};
					CTransform* pTargetTransform = (*iter)->GetTransform();

					Vector3 vPivot = pTargetTransform->GetPivot();
					Vector3 vPosition = pTargetTransform->GetWorldPosition();
					Vector3 vScale = pTargetTransform->GetWorldScale();
					SAFE_RELEASE(pTargetTransform);

					rc.left = vPosition.x - (vPivot.x * vScale.x);
					rc.right = rc.left + vScale.x;
					rc.top = vPosition.y - (vPivot.y * vScale.y);
					rc.bottom = rc.top + vScale.y;
					POINT pt;
					pt.x = (int)vCursorWorldPosition.x;
					pt.y = (int)vCursorWorldPosition.y;

					if (PtInRect(&rc, pt))
					{
						bEnter = true;
						//CGameObject* pTarget = (*iter)->GetGameObject();
						m_CollisionList.push_back(*iter);
						//SAFE_RELEASE(pTarget);
					}
				}
				SAFE_RELEASE(pCurrentLayer);
			}


			if (m_CollisionList.size() == 0)
			{
				if (GETKEYDOWN(VK_LBUTTON))
				{
					m_bOnDrag = true;
					m_vBeginDragPosition = Vector3(vCursorPosition.x, vCursorPosition.y, 0);
				}
			}

			if (m_CollisionList.size() == 1)
			{
				if (GETKEYDOWN(VK_LBUTTON))
				{
					CMainFrame* pMainFrame = GET_SINGLE(CTransmitter)->GetMainFrame();
					CGameObject* pTargetObject = *m_CollisionList.begin();

					CTransform* pTargetTransform = pTargetObject->GetTransform();
					CTransform* pParentTransform = pTargetTransform->GetParent();
					SAFE_RELEASE(pTargetTransform);

					if (pParentTransform)
					{
						pMainFrame->m_pInspector->SetGameObject(pParentTransform->GetGameObject());
						m_pDragTarget = pParentTransform->GetGameObject();
					}
					else
					{
						pMainFrame->m_pInspector->SetGameObject(pTargetObject);
						m_pDragTarget = pTargetObject;
					}
				}
			}
			else if (m_CollisionList.size() > 1)
			{
				if (GETKEYDOWN(VK_LBUTTON))
				{
					CMainFrame* pMainFrame = GET_SINGLE(CTransmitter)->GetMainFrame();
					CGameObject* pTargetObject = TopObject();	
					CTransform* pTargetTransform = pTargetObject->GetTransform();
					CTransform* pParentTransform = pTargetTransform->GetParent();
					SAFE_RELEASE(pTargetTransform);

					if (pParentTransform)
					{
						pMainFrame->m_pInspector->SetGameObject(pParentTransform->GetGameObject());
						m_pDragTarget = pParentTransform->GetGameObject();
					}
					else
					{
						pMainFrame->m_pInspector->SetGameObject(pTargetObject);
						m_pDragTarget = pTargetObject;
					}
				}
			}

			if (GETKEYDOWN(VK_LBUTTON))
			{
				if (!bEnter && (vCursorPosition.x > 0 && vCursorPosition.x <= screen.right &&
					vCursorPosition.y > 0 && vCursorPosition.y <= screen.bottom))
				{
					if (m_pDragTarget)
					{
						if (!GETKEY(VK_LSHIFT))
						{
							m_pDragTarget = NULL;
							CMainFrame* pMainFrame = GET_SINGLE(CTransmitter)->GetMainFrame();
							pMainFrame->m_pInspector->m_pTargetGameObject = NULL;
							pMainFrame->m_pInspector->ResetInspector();
						}
						else
						{
							m_pDragTarget = NULL;
							CMainFrame* pMainFrame = GET_SINGLE(CTransmitter)->GetMainFrame();
							pMainFrame->m_pInspector->m_pTargetGameObject = NULL;
						}
					}
				}
			}

			if (GETKEYUP(VK_LBUTTON))
			{
				if (m_pDragTarget)
				{
					m_pDragTarget = NULL;
				}
				else if (!m_pDragTarget && m_bOnDrag)
				{
					m_vEndDragPosition = Vector3(vCursorPosition.x, vCursorPosition.y, 0);
				}
			}

			//Draw Mode
			if (GETKEY(VK_LBUTTON))
			{
				//생성
				if (!m_pDragTarget )
				{
					if (!bEnter && (vCursorPosition.x > 0 && vCursorPosition.x <= tScreenRect.right &&
						vCursorPosition.y > 0 && vCursorPosition.y <= tScreenRect.bottom) && pMainFrame->m_pInspector->m_bDrawMode)
					{
						CGameObject* pTile = pMainFrame->m_pInspector->CreateTile();

						int iGridSize = pMainFrame->m_pInspector->m_iGridSize;

						//커서의 위치를 기준으로 이동함.
						float fFixedX = (round((vCursorWorldPosition.x - iGridSize / 2.0f) / iGridSize)  *  iGridSize);
						float fFixedY = (round((vCursorWorldPosition.y - iGridSize / 2.0f) / iGridSize)  *  iGridSize);

						CTransform* pTransform = pTile->GetTransform();
						Vector3 origin = pTransform->GetWorldPosition();
						pTransform->SetWorldPosition(fFixedX, fFixedY, 0.0f);

						SAFE_RELEASE(pTransform);
					}
				}
			}



			if (m_pDragTarget)
			{
				//이동
				CMainFrame* pMainFrame = GET_SINGLE(CTransmitter)->GetMainFrame();
				Vector2 deltaPosition = GET_SINGLE(CInput)->GetCursorDeltaPosition(SPACE_DEVICE);
				pMainFrame->m_pInspector->UpdateData(TRUE);

				if (pMainFrame->m_pInspector->m_bFixedMove)
				{
					int iGridSize = pMainFrame->m_pInspector->m_iGridSize;

					//커서의 위치를 기준으로 이동함.
					float fFixedX = (round((vCursorWorldPosition.x - iGridSize / 2.0f) / iGridSize)  *  iGridSize);
					float fFixedY = (round((vCursorWorldPosition.y - iGridSize / 2.0f) / iGridSize)  *  iGridSize);

					//CTransform* pDragTargetTransform = m_pDragTarget->GetTransform();
					//Vector3 origin = pDragTargetTransform->GetWorldPosition();
					//SAFE_RELEASE(pDragTargetTransform);

					pMainFrame->m_pInspector->m_vPosition = Vector3(fFixedX, fFixedY, 0.0f);
					pMainFrame->m_pInspector->UpdateData(FALSE);
					pMainFrame->m_pInspector->CummitTransform();

					//pMainFrame->m_pInspector->ChangeIndex(m_pDragTarget, (int)origin.x, (int)origin.y);
				}
				else
				{
					CTransform* pDragTargetTransform = m_pDragTarget->GetTransform();
					if (pDragTargetTransform)
					{
						Vector3 origin = pDragTargetTransform->GetWorldPosition();
						SAFE_RELEASE(pDragTargetTransform);

						pMainFrame->m_pInspector->m_vPosition -= vDeltaCursorPosition;
						pMainFrame->m_pInspector->UpdateData(FALSE);
						pMainFrame->m_pInspector->CummitTransform();

						//pMainFrame->m_pInspector->ChangeIndex(m_pDragTarget, (int)origin.x, (int)origin.y);
					}
				}
			}


			if (GETKEYDOWN(VK_RBUTTON))
			{
				if ((vCursorPosition.x > 0 && vCursorPosition.x <= tScreenRect.right &&
					vCursorPosition.y > 0 && vCursorPosition.y <= tScreenRect.bottom))
				{
					if (m_pDragTarget)
						m_pDragTarget = NULL;

					m_bCameraMove = true;
				}
			}

			if (GETKEYUP(VK_RBUTTON) && m_bCameraMove)
			{
				m_bCameraMove = false;
			}


			if (m_bCameraMove)
			{
				CTransform* pCameraTransform = m_pScene->GetMainCameraTransform();
				Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
				pCameraTransform->SetWorldPosition(vCameraPosition + Vector3(vDeltaCursorPosition.x, vDeltaCursorPosition.y, 0.0f));
				SAFE_RELEASE(pCameraTransform);
			}

			vPrevCursorPosition = vCursorPosition;

			m_CollisionList.clear();
		}
		//SAFE_RELEASE(pLayer);

	}
	return 0;
}
