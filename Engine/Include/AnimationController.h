#pragma once
#include "Ref.h"

//Animator Ŭ�������� �� Ŭ������ ������ �ְ�, ������ �ִ� AnimationController�� ����.
//���⿡���� AnimationState�� ���� �ϴ� StateMachine�� �����ϸ�, ���� �������� ����� �ִϸ��̼��� �����Ѵ�.

LMNT_BEGIN

typedef union LMNT_DLL _tagParameter
{
	bool	bValue;
	int		iValue;
	float	fValue;
}PARAMETER, *PPARAMETER;

class LMNT_DLL CAnimationController
{
private:
	friend class CAnimator;

public:
	CAnimationController();
	~CAnimationController();

private:
	class CAnimator* m_pAnimator;
	unordered_map<string, class CAnimationState*> m_mapState;
	unordered_map<string, PARAMETER> m_mapParameter;
	class CAnimationState* m_pEntryState;
	class CAnimationState* m_pAnyState;
	class CAnimationState* m_pExitState;
	class CAnimationState* m_pCurrentState;

public:
	void SetBool(const string& strKey, bool bValue);
	void SetInteger(const string& strKey, int iValue);
	void SetFloat(const string& strKey, float fValue);
	bool GetBool(const string& strKey) const;
	int GetInteger(const string& strKey) const;
	float GetFloat(const string& strKey) const;

	class CAnimationState* GetCurrentState() const;
	class CAnimator* GetAnimator() const;
	void SetEntryState(const string& strKey);
	void AddParameter(const string& strKey);
	void RemoveParameter(const string& strKey);
	
public:
	class CAnimationState* CreateAnimationState(const string& strTag);
	class CAnimationState* FindAnimationState(const string& strTag);
	void EraseAnimationState(const string& strTag);
	void TransitionState(const string& strKey);

public:

	void Update(float fTime);

};

LMNT_END