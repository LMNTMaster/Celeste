#pragma once
#include "Component.h"
#include "Thread.h"

LMNT_USING

enum MENU_STATE
{
	MS_TITLE,
	MS_TITLE_FADE_OUT,
	MS_TITLE_TO_MENU,
	MS_MENU_ENTER,
	MS_MENU,
	MS_SELECT_CLIMB,
	MS_SELECT_OPTIONS,
	MS_SELECT_CREDITS,
	MS_SELECT_EXIT,
	MS_EXIT_MENU,

	MS_ENTER_SELECT_CHAPTER0,
	MS_SELECT_CHAPTER0,
	MS_CHAPTER0,
	MS_CONFIRM_CHAPTER0,
	MS_START_CHAPTER0,
	MS_EXIT_SELECT_CHAPTER0,

	MS_ENTER_SELECT_CHAPTER1,
	MS_SELECT_CHAPTER1,	
	MS_CHAPTER1,
	MS_CONFIRM_CHAPTER1,
	MS_START_CHAPTER1,
	MS_EXIT_SELECT_CHAPTER1,

	MS_TEMPSTATE00,
	MS_TEMPSTATE01,
	MS_TEMPSTATE02,
	MS_TEMPSTATE03,
	MS_TEMPSTATE04,
	MS_TEMPSTATE05
};


enum MENU_TYPE
{
	MT_NOT_IN_MENU = -1,
	MT_CLIMB,
	MT_OPTIONS,
	MT_CREDITS,
	MT_EXIT
};

class CTitleSceneManager :
	public CComponent
{
private:
	friend class CGameObject;
public:
	CTitleSceneManager();
	CTitleSceneManager(const CTitleSceneManager& component);
	~CTitleSceneManager();
	CTitleSceneManager* Clone() const	override;


private:
	MENU_STATE m_eState;
	MENU_TYPE m_eMenuType;
	class CGameObject* m_pTitle;
	class CGameObject* m_pBackground;
	class CGameObject* m_pMenuBackground;
	class CGameObject* m_pFader;
	class CGameObject* m_pRunning;
	class CGameObject* m_pInfo;
	Vector3 m_vOriginScale;
	Vector3 m_vTargetScale;

	int m_iChapter;
	int m_iLoadingChapter;
	class CChapter0LoadingThread* m_pChapter0LoadingThread;
	class CChapter1LoadingThread* m_pChapter1LoadingThread;
	Vector3 m_pLocalOffset[8];

	int m_iReserveToStartChapter;


	bool m_bChapter0OnLoading;
	bool m_bChapter1OnLoading;
	class CGameObject* m_pLoading;

	Vector3 m_vMenuOffset;
	vector<class CGameObject*> m_vecMenuText;
	vector<class CGameObject*> m_vecMenuIcon;

	float m_fBlinkTimer;
	float m_fTimer;


	bool m_bBlowoutChapter0;
	bool m_bBlowoutChapter1;

	class CScene* m_pChapter0;
	class CScene* m_pChapter1;

private:
	void UpdateMenuType(const Vector4& vColor);
	void SetTargetScale(const Vector3& vScale);
	void UpdateMenuPosition(float fProgress);

public:
	void SetChapter0End(class CScene* pScene);
	void SetChapter1End(class CScene* pScene);
	void SetMenuBackground(class CGameObject* pBackground);
	void SetOriginalScale(const Vector3& vScale);
	void SetBackground(class CGameObject* pBackground);
	void SetTitle(class CGameObject* pTitleObject);

public:
	bool Initialize() override;
	int Update(float fTime) override;


};

