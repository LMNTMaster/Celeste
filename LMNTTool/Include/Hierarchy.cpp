// Hierarchy.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Hierarchy.h"


// CHierarchy

IMPLEMENT_DYNCREATE(CHierarchy, CFormView)

CHierarchy::CHierarchy()
	: CFormView(IDD_DIALOG_HIERARCHY)
{

}

CHierarchy::~CHierarchy()
{
}

void CHierarchy::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_HIERARCHY, m_Tree);
}

BEGIN_MESSAGE_MAP(CHierarchy, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_HIERARCHY, &CHierarchy::OnTvnSelchangedTreeHierarchy)
	ON_NOTIFY(NM_CLICK, IDC_TREE_HIERARCHY, &CHierarchy::OnNMClickTreeHierarchy)
END_MESSAGE_MAP()


// CHierarchy 진단입니다.

#ifdef _DEBUG
void CHierarchy::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CHierarchy::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHierarchy 메시지 처리기입니다.

void CHierarchy::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	HTREEITEM tRootItem = {};
	tRootItem = m_Tree.InsertItem(L"Game Object", TVI_ROOT);
	HTREEITEM tItem = {};
	tItem = m_Tree.InsertItem(L"Camera", tRootItem);
	tItem = m_Tree.InsertItem(L"Test", tRootItem);

	m_Tree.EnsureVisible(tItem);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CHierarchy::OnTvnSelchangedTreeHierarchy(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


//게임 오브젝트 클릭 시 할 일
void CHierarchy::OnNMClickTreeHierarchy(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}



BOOL CHierarchy::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CFormView::OnDrop(pDataObject, dropEffect, point);
}
