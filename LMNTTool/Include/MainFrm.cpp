
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"

#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"
#include "Layer.h"
#include "Component/Camera.h"
#include "SceneManager.h"
#include "Component.h"
#include "Component/Tile.h"
#include "Component/Renderer.h"
#include "Material.h"
#include "Transmitter.h"


LMNT_USING

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_GAMEOBJECT_CREATEGAMEOBJECTC, &CMainFrame::OnGameobjectCreategameobjectc)
	ON_COMMAND(ID_32772, &CMainFrame::On32772)
	ON_COMMAND(ID_GAMEOBJECT_CREATEAREA, &CMainFrame::OnGameobjectCreatearea)
	ON_COMMAND(ID_GAMEOBJECT_CREATECOLLIDER, &CMainFrame::OnGameobjectCreatecollider)
	ON_COMMAND(ID_GAMEOBJECT_CREATEJUMPTHRUTILE, &CMainFrame::OnGameobjectCreatejumpthrutile)
	//ON_COMMAND(ID_GAMEOBJECT_CREATECRUMBLETILE, &CMainFrame::OnGameobjectCreatecrumbletile)
	ON_COMMAND(ID_GAMEOBJECT_CREATEDANGERTILE, &CMainFrame::OnGameobjectCreatedangertile)
	ON_COMMAND(ID_GAMEOBJECT_CREATEPREFAB, &CMainFrame::OnGameobjectCreateprefab)
	ON_COMMAND(ID_CREATECOLLIDER_DIRT, &CMainFrame::OnCreatecolliderDirt)
	ON_COMMAND(ID_CREATECOLLIDER_SNOW, &CMainFrame::OnCreatecolliderSnow)
	ON_COMMAND(ID_CREATECOLLIDER_ASPHALT, &CMainFrame::OnCreatecolliderAsphalt)
	ON_COMMAND(ID_CREATECOLLIDER_WOOD, &CMainFrame::OnCreatecolliderWood)
	ON_COMMAND(ID_CREATECOLLIDER_BRICK, &CMainFrame::OnCreatecolliderBrick)
	ON_COMMAND(ID_CREATECOLLIDER_METALGIRDER, &CMainFrame::OnCreatecolliderMetalgirder)
	ON_COMMAND(ID_CREATECOLLIDER_2, &CMainFrame::OnCreatecollider2)
	ON_COMMAND(ID_VIEW_BACKGROUND_MAP, &CMainFrame::OnViewBackgroundMap)
	ON_COMMAND(ID_CREATECOLLIDER_DEADZONE, &CMainFrame::OnCreatecolliderDeadzone)
	ON_COMMAND(ID_GAMEOBJECT_CREATESPRING, &CMainFrame::OnGameobjectCreatespring)
	ON_COMMAND(ID_GAMEOBJECT_CREATECHECKPOINT, &CMainFrame::OnGameobjectCreatecheckpoint)
	ON_COMMAND(ID_GAMEOBJECT_CREATEREFILLCRISTAL, &CMainFrame::OnGameobjectCreaterefillcristal)
	ON_COMMAND(ID_GAMEOBJECT_CREATESTRAWBERRY, &CMainFrame::OnGameobjectCreatestrawberry)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	m_bShowBackground = false;
	_CrtSetBreakAlloc(147);
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("도구 모음을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}
	//
	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("상태 표시줄을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	cs.cx = 1700;
	cs.cy = 720;
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{

	GET_SINGLE(CTransmitter)->Initialize();

	m_SplitWnd.CreateStatic(this, 1, 2);
	m_pHierarchy = NULL;
	//m_SplitWnd.CreateView(0, 0, RUNTIME_CLASS(CHierarchy), CSize(250, 720), pContext);
	m_SplitWnd.CreateView(0, 0, RUNTIME_CLASS(CToolView), CSize(1280, 720), pContext);
	m_SplitWnd.CreateView(0, 1, RUNTIME_CLASS(CInspector), CSize(420, 720), pContext);

	//m_pHierarchy = (CHierarchy*)m_SplitWnd.GetPane(0, 0);
	m_pToolView = (CToolView*)m_SplitWnd.GetPane(0, 0);
	m_pInspector = (CInspector*)m_SplitWnd.GetPane(0, 1);	
	m_tRect = { 0,0,1280,720 };

	GET_SINGLE(CTransmitter)->m_pMainFrame = this;



	return TRUE;
}


void CMainFrame::OnGameobjectCreategameobjectc()
{
	//카메라 위치에 오브젝트 생성. 타겟으로 설정 함.
	RECT rc = {};
	m_pToolView->GetDC()->GetClipBox(&rc);		
	m_pInspector->OnGameobjectCreategameobject();
	m_tRect = rc;

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CMainFrame::On32772()
{
	m_pToolView->m_bShowGrid = !m_pToolView->m_bShowGrid;
	m_pToolView->ShowGrid();
}

bool CMainFrame::HasFocus()
{
	return GetFocus()->GetSafeHwnd() == m_hWnd;
}


void CMainFrame::OnGameobjectCreatearea()
{
	m_pInspector->CreateArea();
}


void CMainFrame::OnGameobjectCreatecollider()
{
	//m_pInspector->CreateCollider();
}


void CMainFrame::OnGameobjectCreatejumpthrutile()
{
	m_pInspector->CreateJumpthru();
}


void CMainFrame::OnGameobjectCreatecrumbletile()
{
	m_pInspector->CreateCrumble();
}


void CMainFrame::OnGameobjectCreatedangertile()
{
	m_pInspector->CreateDanger();
}


void CMainFrame::OnGameobjectCreateprefab()
{
	m_pInspector->LoadPrefab();
}


void CMainFrame::OnCreatecolliderDirt()
{
	m_pInspector->CreateCollider("Dirt");
}


void CMainFrame::OnCreatecolliderSnow()
{
	m_pInspector->CreateCollider("SnowSoft");
}


void CMainFrame::OnCreatecolliderAsphalt()
{
	m_pInspector->CreateCollider("Asphalt");
}


void CMainFrame::OnCreatecolliderWood()
{
	m_pInspector->CreateCollider("WoodWalkWay");
}


void CMainFrame::OnCreatecolliderBrick()
{
	m_pInspector->CreateCollider("Brick");
}


void CMainFrame::OnCreatecolliderMetalgirder()
{
	m_pInspector->CreateCollider("MetalGirder");
}


void CMainFrame::OnCreatecollider2()
{
	m_pInspector->CreateCollider("ZipMover");
}


void CMainFrame::OnViewBackgroundMap()
{
	m_bShowBackground = !m_bShowBackground;
	m_pInspector->ShowBackgroundMap1(m_bShowBackground);
}


void CMainFrame::OnCreatecolliderDeadzone()
{
	m_pInspector->CreateCollider("DeadZone");
}


void CMainFrame::OnGameobjectCreatespring()
{
	m_pInspector->CreateSpring();
}


void CMainFrame::OnGameobjectCreatecheckpoint()
{
	m_pInspector->CreateCheckPoint();
}


void CMainFrame::OnGameobjectCreaterefillcristal()
{
	m_pInspector->CreateRefillCristal();
}


void CMainFrame::OnGameobjectCreatestrawberry()
{
	m_pInspector->CreateStrawberry();
}
