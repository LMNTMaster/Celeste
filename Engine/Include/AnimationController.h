#pragma once
#include "Ref.h"

//Animator 클래스에서 이 클래스를 가지고 있고, 가지고 있는 AnimationController를 실행.
//여기에서는 AnimationState를 노드로 하는 StateMachine을 실행하며, 현재 상주중인 노드의 애니메이션을 실행한다.

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