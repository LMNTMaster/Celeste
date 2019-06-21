
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once
#include "ToolView.h"
#include "Inspector.h"
#include "Hierarchy.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:
	RECT m_tRect;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	CSplitterWnd		m_SplitWnd;

public:
	CToolView* m_pToolView;
	CInspector* m_pInspector;
	CHierarchy* m_pHierarchy;

	bool m_bShowBackground;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnGameobjectCreategameobjectc();
	afx_msg void On32772();
	bool HasFocus();
	afx_msg void OnGameobjectCreatearea();
	afx_msg void OnGameobjectCreatecollider();
	afx_msg void OnGameobjectCreatejumpthrutile();
	afx_msg void OnGameobjectCreatecrumbletile();
	afx_msg void OnGameobjectCreatedangertile();
	afx_msg void OnGameobjectCreateprefab();
	afx_msg void OnCreatecolliderDirt();
	afx_msg void OnCreatecolliderSnow();
	afx_msg void OnCreatecolliderAsphalt();
	afx_msg void OnCreatecolliderWood();
	afx_msg void OnCreatecolliderBrick();
	afx_msg void OnCreatecolliderMetalgirder();
	afx_msg void OnCreatecollider2();
	afx_msg void OnViewBackgroundMap();
	afx_msg void OnCreatecolliderDeadzone();
	afx_msg void OnGameobjectCreatespring();
	afx_msg void OnGameobjectCreatecheckpoint();
	afx_msg void OnGameobjectCreaterefillcristal();
	afx_msg void OnGameobjectCreatestrawberry();
};


