#pragma once
#include "Component.h"
#include "Dialogue.h"

LMNT_USING

class CStageManager00 :
	public CComponent
{
private:
	enum EVENT_STATE
	{
		ES_NORMAL,
		ES_EVENT00,
		ES_EVENT01,
		ES_EVENT02,
		ES_EVENT03,
		ES_EVENT04,
		ES_EVENT05,
		ES_EVENT06,
		ES_EVENT07,
		ES_EVENT08,
		ES_EVENT09,
		ES_EVENT_END,
	};
	enum SIGN_STATE
	{
		SS_HIDE,
		SS_CLIMB0,
		SS_CLIMB1,
		SS_DASH
	};

private:
	friend class CGameObject;
	CStageManager00();
	~CStageManager00();

private:
	bool m_bInitialized;

	bool m_bBeginEvent;
	float m_fBeginEventTimer;
	float m_fEndEventTimer;

	class CGameObject* m_pFader;
	class CGameObject* m_pPlayer;
	class CGameObject* m_pGranny;
	class CGameObject* m_pBird;
	class CGameObject* m_pTutorialSign;
	class CGameObject* m_pEventText;
	float m_fSlowTimer;
	EVENT_STATE m_eEventState;
	bool m_bDialogueEvent;
	int m_iDialogueProgress;
	float m_fDialogueTimer;

	SIGN_STATE m_eSignState;
	SIGN_STATE m_eSignReserveState;
	float m_fSignYAngle;
	float m_fTutorialSignRotationTimer;
	bool m_bExitTutorial;
	bool m_bOnTutorial;
	bool m_bTutorialClimb;	
	bool m_bTutorialDash;

	bool m_bCameraEvent;
	float m_fCameraEventTimer;

	bool m_bTextFadeOut;

private:
	CDialogue* m_pDialogue;
	class CGameObject* m_pFallBlock;
	vector<class CGameObject*> m_vecFallBlockBridge;

public:
	void SetBird(class CGameObject* pBird);
	void InitializeDialogue();
	void SetDialogue(CDialogue* pDialogue);
	void AddFallBlockBridge(class CGameObject* pBlock);
	void SetPlayer(class CGameObject* pPlayer);
	void SetGranny(class CGameObject* pGranny);
	void SetFallBlock(class CGameObject* pBlock);

	void DashPlayer();
public:
	bool Initialize() override;
	int Update(float fTime) override;
};

