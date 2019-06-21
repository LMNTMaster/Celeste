#pragma once
#include "Component.h"
#include "../Client.h"
#include "Component/Renderer.h"
#include "Component/Animator.h"
#include "AnimationController.h"
#include "AnimationState.h"
#include "AnimationClip2D.h" 
#include "Component/Text.h"
#include <queue>

LMNT_USING
enum PORTRAIT_DIRECTION
{
	PD_LEFT,
	PD_RIGHT,
};
enum PORTRAIT_CHARACTER
{
	PC_MADELINE,
	PC_GRANNY,
	PC_THEO
};
enum DIALOGUE_STATE
{
	DS_NONE,
	DS_OPEN,
	DS_NORMAL,
	DS_CLOSE
};


class CDialogue :
	public CComponent
{
private:
	friend class CGameObject;
	CDialogue();
	~CDialogue();

private:
	float m_fTimer;
	DIALOGUE_STATE m_eState;
	PORTRAIT_DIRECTION m_ePortraitDirection;
	PORTRAIT_CHARACTER m_eCharacter;

	CGameObject* m_pBackPanel;
	CGameObject* m_pPortraitPanel;

	CAnimationController* m_pPortraitController;
	Vector2 m_vIndexPosition;
	vector<CGameObject*> m_vecText;
	float m_fTextTimer;
	float m_fTextInterval;
	queue<wchar_t> m_qString;
	string m_strAnimationKey;
	bool m_bEnd;



public:
	DIALOGUE_STATE GetState() const;
	bool End() const;
	void SetReserveAnimation(const string& strAnimationKey);
	void SetBackground(PORTRAIT_CHARACTER eCharacter);
	void SetString(const wstring& strText);
	void SetAnimation(const string& strAnimationKey);
	CGameObject* DrawWord(const Vector3& vOffset, const wstring& strText);
	void DrawDialogueText();
	void Next(PORTRAIT_DIRECTION eDirection, PORTRAIT_CHARACTER eCharacter, const string& strAnimationKey, const wstring& strText);
	void OpenDialogue(PORTRAIT_DIRECTION eDirection, PORTRAIT_CHARACTER eCharacter, const string& strAnimationKey);
	void CloseDialogue();

public:
	bool Initialize() override;
	int Update(float fTime) override;
	void Render(float fTime) override;
};

