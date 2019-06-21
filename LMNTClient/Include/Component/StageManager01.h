#pragma once
#include "Component.h"
#include "Dialogue.h"

LMNT_USING

class CStageManager01 :
	public CComponent
{
private:
	enum LABLE_STATE
	{
		LS_HIDE,
		LS_ON_GENERATION,
		LS_ON_STANDBY,
		LS_ON_HIDING
	};
	enum SOUND_FADE_STATE
	{
		SFS_NORMAL,
		SFS_FADE_IN,
		SFS_FADE,
		SFS_FADE_OUT
	};
private:
	friend class CGameObject;
	CStageManager01();
	~CStageManager01();


private:
	bool m_bInitialized;
	class CGameObject* m_pComplete;
	class CGameObject* m_pPlayer;
	class CGameObject* m_pTheo;
	class CGameObject* m_pNPCLable;
	class CGameObject* m_pCampfire;
	class CGameObject* m_pFader;
	LABLE_STATE m_eLableState;
	SOUND_FADE_STATE m_eFadeState;
	float m_fSoundFadingTimer;
	float m_fLableTimer;
	bool m_bOnInteraction;
	int m_iTheoInterationCount;

	float m_fDialogueTimer;


	PAREA m_pTheoEventArea;
	PAREA m_pMonologueEventArea;

	int m_iTheoDialogueProgress;
	int m_iSelfDialogueProgress;
	CDialogue* m_pDialogue;

private:
	void SoundFadeOut();
	void SoundFadeIn();
	void CreateZipMover(const string& strFileName, const Vector3& vBeginPosition, const Vector3& bEndPosition, const Vector3& vForce, int iBlockType);
	
	void LockPlayer();
	void UnLockPlayer();

public:
	void SetMadelineCampfire(class CGameObject* pTarget);
	void SetDialogue(class CDialogue* pDialogue);
	void InitializeDialogue();
	void SetPlayer(class CGameObject* pPlayer);

public:
	bool Initialize() override;
	int Update(float fTime) override;
};

