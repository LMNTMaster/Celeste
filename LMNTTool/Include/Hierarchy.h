#pragma once
#include "afxcmn.h"



// CHierarchy �� ���Դϴ�.

class CHierarchy : public CFormView
{
	DECLARE_DYNCREATE(CHierarchy)

protected:
	CHierarchy();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CHierarchy();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HIERARCHY };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_Tree;
	afx_msg void OnTvnSelchangedTreeHierarchy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTreeHierarchy(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
};


