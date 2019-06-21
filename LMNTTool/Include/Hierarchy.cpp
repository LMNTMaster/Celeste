// Hierarchy.cpp : ���� �����Դϴ�.
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


// CHierarchy �����Դϴ�.

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


// CHierarchy �޽��� ó�����Դϴ�.

void CHierarchy::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	HTREEITEM tRootItem = {};
	tRootItem = m_Tree.InsertItem(L"Game Object", TVI_ROOT);
	HTREEITEM tItem = {};
	tItem = m_Tree.InsertItem(L"Camera", tRootItem);
	tItem = m_Tree.InsertItem(L"Test", tRootItem);

	m_Tree.EnsureVisible(tItem);
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}


void CHierarchy::OnTvnSelchangedTreeHierarchy(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


//���� ������Ʈ Ŭ�� �� �� ��
void CHierarchy::OnNMClickTreeHierarchy(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}



BOOL CHierarchy::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CFormView::OnDrop(pDataObject, dropEffect, point);
}
