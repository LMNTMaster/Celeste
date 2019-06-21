#pragma once
#include "Engine.h"
#include "afxwin.h"
#include "GameObject.h"

LMNT_USING
// CInspector 폼 뷰입니다.

enum BERRY_TYPE
{
	BT_NORMAL,
	BT_WING,
	BT_GOLDEN,
	BT_GOLDEN_WING,
	BT_GHOST_NORMAL,
	BT_GHOST_WING,
	BT_GHOST_GOLDEN_NORMAL,
	BT_GHOST_GOLDEN_WING
};

typedef struct _tagStrawberry
{
	BERRY_TYPE eType;
	CGameObject* pGameObject;	
}STRAWBERRY, *PSTRAWBERRY;
typedef struct _tagPrefab
{
	string strFileName;
	CGameObject* pObject;
}PREFAB, *PPREFAB;

typedef struct _tagSpike
{
	DIRECTION eDirection;
	CGameObject* pObject;
}SPIKE, *PSPIKE;


class CInspector : public CFormView
{
	DECLARE_DYNCREATE(CInspector)

protected:
	CInspector();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CInspector();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	CGameObject* m_pTargetGameObject;
	class CGridRenderer* m_pGridRenderer;
	vector<CGameObject*> m_vecArea;
	vector<CGameObject*> m_vecCollider;
	vector<CGameObject*> m_vecJumpthru;
	vector<CGameObject*> m_vecCrumble;
	vector<PSPIKE> m_vecDanger;
	vector<CGameObject*> m_vecEventBlock;
	vector<CGameObject*> m_vecObject;
	vector<CGameObject*> m_vecSpring;
	vector<PPREFAB> m_vecPrefab;
	vector<CGameObject*> m_vecCheckPoint;
	vector<CGameObject*> m_vecRefill;
	vector<PSTRAWBERRY> m_vecStrawberry;
	Vector3 m_vPosition;
	Vector3 m_vRotation;
	Vector3 m_vScale;
	BOOL m_bFixedMove;
	bool m_bShowArea;
	bool m_bShowTerrain;
	bool m_bShowBackground;
	bool m_bShowCollider;
	afx_msg void OnEnChangeEditPositionx();
	afx_msg void OnEnChangeEditPositiony();
	afx_msg void OnEnChangeEditPositionz();
	afx_msg void OnEnChangeEditRotationx();
	afx_msg void OnEnChangeEditRotationy();
	afx_msg void OnEnChangeEditRotationz();
	afx_msg void OnEnChangeEditScalex();
	afx_msg void OnEnChangeEditScaley();
	afx_msg void OnEnChangeEditScalez();

public:

	CButton m_ButtonSave;
	CButton m_ButtonLoad;
	CComboBox m_TilesetCombo;
	CComboBox m_LayerCombo;
	CComboBox m_TileTypeCombo;
	int m_iGridSize;
	int m_iTileIndex;
	BOOL m_bDrawMode;
	CString m_strTagName;
	CString m_strAtlasKey;
	class CGameObject* m_pBackgroundMap;	
	class CGameObject* m_pBackgroundMap1;
	class CGameObject* m_pBackgroundMap2;
	class CGameObject* m_pBackgroundMap3;
	class CGameObject* m_pBackgroundMap4;
	class CGameObject* m_pBackgroundMap5;
	class CGameObject* m_pBackgroundMap6;
	class CGameObject* m_pBackgroundMap7;
	
	void SetGameObject(CGameObject* pTarget);
	virtual void OnInitialUpdate();
	class CGameObject* CreateTile();
	void OnGameobjectCreategameobject();
	void UpdateTransform();
	void CummitTransform();
	void SetTileIndexCombo(int iCount);
	void SetTileIndexCombo(int iCount, int iIndex);
	void SetCombo();
	void ResetInspector();

	afx_msg void OnStnClickedStaticTransform();
	afx_msg void OnCbnEditchangeComboTileset();
	afx_msg void OnCbnCloseupComboTileset();
	afx_msg void OnEnChangeEditGridsize();
	afx_msg void OnBnClickedCheckDrawMode();
	afx_msg void OnEnChangeEditTileindex();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnEnChangeEditName();
	afx_msg void OnCbnCloseupComboLayer();
	afx_msg void OnEnChangeEditAtlaskey();
	afx_msg void OnCbnCloseupComboTilesetType();

	//unordered_map<POINT, class CGameObject*> m_mapTile;

public:
	//void AddTile(class CGameObject* pTarget);
	//void RemoveTile(int iX, int iY);
	//class CGameObject* FindIndex(int iX, int iY);
	//
	//int CalculateTile(class CGameObject* pTarget);
	//void ChangeIndex(class CGameObject* pTarget, int iOriginX, int iOriginY);



	int GetTopEdge();
	int GetBottomEdge();
	int GetLeftEdge();
	int GetRightEdge();
	int GetTopSingle();
	int GetBottomSingle();
	int GetLeftSingle();
	int GetRightSingle();
	int GetSingle();
	int GetOuterLeftTopCorner();
	int GetOuterRightTopCorner();
	int GetOuterLeftBottomCorner();
	int GetOuterRightBottomCorner();
	int GetInnerLeftTopCorner();
	int GetInnerRightTopCorner();
	int GetInnerLeftBottomCorner();
	int GetInnerRightBottomCorner();
	int GetInnerBlock();
	int GetEmptyBlock();

	bool TopEdge(class CGameObject* pTarget);
	bool BottomEdge(class CGameObject* pTarget);
	bool LeftEdge(class CGameObject* pTarget);
	bool RightEdge(class CGameObject* pTarget);
	bool TopSingle(class CGameObject* pTarget);
	bool BottomSingle(class CGameObject* pTarget);
	bool LeftSingle(class CGameObject* pTarget);
	bool RightSingle(class CGameObject* pTarget);
	bool Single(class CGameObject* pTarget);
	bool OuterLeftTopCorner(class CGameObject* pTarget);
	bool OuterRightTopCorner(class CGameObject* pTarget);
	bool OuterLeftBottomCorner(class CGameObject* pTarget);
	bool OuterRightBottomCorner(class CGameObject* pTarget);
	bool InnerLeftTopCorner(class CGameObject* pTarget);
	bool InnerRightTopCorner(class CGameObject* pTarget);
	bool InnerLeftBottomCorner(class CGameObject* pTarget);
	bool InnerRightBottomCorner(class CGameObject* pTarget);
	bool InnerBlock(class CGameObject* pTarget);
	bool EmptyBlock(class CGameObject* pTarget);


	int GetCurselIndexFromTileIndex(int iTileIndex);
	int GetTileIndexFromCombo();
	class CGameObject* CreateArea();
	class CGameObject* CreateCollider();
	class CGameObject* CreateCollider(const string& strName);
	class CGameObject* CreateJumpthru();
	class CGameObject* CreateCrumble();
	class CGameObject* CreateDanger();
	class CGameObject* CreateDanger(DIRECTION eFaceDir);
	class CGameObject* CreateSpring();
	class CGameObject* LoadPrefab();
	class CGameObject* LoadPrefab(const string& strFileName, class CLayer* pLayer);
	
	class CGameObject* CreateCheckPoint();
	class CGameObject* CreateRefillCristal();
	class CGameObject* CreateStrawberry(BERRY_TYPE eType = BT_NORMAL);

	bool IsPrefab(const string& strTag);
	void ShowBackgroundMap1(bool bShow);

};


