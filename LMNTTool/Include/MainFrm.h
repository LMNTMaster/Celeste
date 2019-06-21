
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#include "ToolView.h"
#include "Inspector.h"
#include "Hierarchy.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:
	RECT m_tRect;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	CSplitterWnd		m_SplitWnd;

public:
	CToolView* m_pToolView;
	CInspector* m_pInspector;
	CHierarchy* m_pHierarchy;

	bool m_bShowBackground;

// ������ �޽��� �� �Լ�
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


