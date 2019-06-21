
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"

#include "Core.h"
#include "Scene.h"
#include "Layer.h"
#include "SceneManager.h"
#include "EditScene.h"
#include "ResourcesManager.h"
#include "PathManager.h"
#include "GameObject.h"
#include "EditorCursor.h"
#include "Component.h"
#include "Component/Renderer.h"
#include "Material.h"
#include "Transform.h"
#include "GridRenderer.h"
LMNT_USING


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolView
IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView():
	m_pGrid(NULL),
	m_bShowGrid(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CToolView::~CToolView()
{
	DESTROY_SINGLE(CCore);
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	cs.cx = 1280;
	cs.cy = 720;
	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	GET_SINGLE(CCore)->Init(AfxGetInstanceHandle(), m_hWnd,RESOLUTION(1280, 720), true);

	GET_SINGLE(CCore)->SetRenderMode(RM_2D);

	CScene* pScene = GET_SINGLE(CSceneManager)->CreateScene<CEditScene>("Editor Scene");
	GET_SINGLE(CSceneManager)->SetCurrentScene(pScene);

	LoadGameData();


	CLayer* pGridLayer = pScene->CreateLayer("Grid", -100);
	CLayer* pBackground = pScene->CreateLayer("BackgroundMap", -99);
	SAFE_RELEASE(pBackground);

	CLayer* pAreaLayer = pScene->CreateLayer("Area", 500000);
	SAFE_RELEASE(pAreaLayer);

	CLayer* pTempLayer = pScene->CreateLayer("BG0", -90);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = pScene->CreateLayer("BG1", -85);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = pScene->CreateLayer("BG2", -80);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = pScene->CreateLayer("BG3", -75);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = pScene->CreateLayer("BG4", -70);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = pScene->CreateLayer("BG5", -65);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = pScene->CreateLayer("BackTerrain", -5);
	SAFE_RELEASE(pTempLayer);
	CLayer* pLayer = pScene->FindLayer("Default"); // 0
	pTempLayer = pScene->CreateLayer("ForeTerrain", 5);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = pScene->CreateLayer("VFX0", 10);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = pScene->CreateLayer("VFX1", 15);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = pScene->CreateLayer("FG0", 65);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = pScene->CreateLayer("FG1", 70);
	SAFE_RELEASE(pTempLayer);
	pTempLayer = pScene->CreateLayer("FG2", 75);
	SAFE_RELEASE(pTempLayer);

	pTempLayer = pScene->CreateLayer("Collider", 90);
	SAFE_RELEASE(pTempLayer);




	CGameObject* pGrid = CGameObject::CreateObject("Grid", pGridLayer);
	CGridRenderer* pGridRenderer = pGrid->AddComponent<CGridRenderer>("GridRenderer");
	m_pGrid = pGridRenderer;

	

	SAFE_RELEASE(pGridRenderer);	
	SAFE_RELEASE(pGrid);

	CGameObject* pCursor = CGameObject::CreateObject("Cursor", pLayer);
	CEditorCursor* pCursorComponent = pCursor->AddComponent<CEditorCursor>("Cursor");
	
	SAFE_RELEASE(pCursorComponent);
	SAFE_RELEASE(pCursor);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pGridLayer);
	SAFE_RELEASE(pScene);
	// TODO: 여기에 특수화;된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void CToolView::ShowGrid()
{
	if (m_bShowGrid)
		m_pGrid->SetEnable();
	else
		m_pGrid->SetDisable();
}

void CToolView::LoadGameData()
{
	string strPath = GET_SINGLE(CPathManager)->FindPathMultiByte(TEXTURE_PATH);

	strPath += "Celeste\\Gameplay11.ap";

	FILE* pFile = NULL;

	fopen_s(&pFile, strPath.c_str(), "rb");

	if (!pFile)
		return ;
	else
	{
		int iCount = 0;
		int iAtlasSizeX = 0;
		int iAtlasSizeY = 0;
		fread(&iCount, 4, 1, pFile);
		fread(&iAtlasSizeX, 4, 1, pFile);
		fread(&iAtlasSizeY, 4, 1, pFile);

		for (int i = 0; i < iCount; ++i)
		{
			PATLASDATA pData = new ATLASDATA;
			int iLength = 0;
			fread(&iLength, 4, 1, pFile);
			fread(&pData->pDirectory, 1, iLength, pFile);
			fread(&pData->wX, 2, 1, pFile);
			fread(&pData->wY, 2, 1, pFile);
			fread(&pData->wWidth, 2, 1, pFile);
			fread(&pData->wHeight, 2, 1, pFile);
			pData->iAtlasSizeX = iAtlasSizeX;
			pData->iAtlasSizeY = iAtlasSizeY;

			if (pData->wHeight == 0 && pData->wWidth == 0 && pData->wX == 0 && pData->wY == 0)
			{
				delete pData;
				continue;
			}

			GET_SINGLE(CResourcesManager)->InsertAtlasData(pData);
		}

		fclose(pFile);
	}
}
