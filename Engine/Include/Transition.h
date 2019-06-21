#pragma once
#include "Engine.h"
#include "AnimationController.h"

LMNT_BEGIN

class LMNT_DLL CTransition
{
private:
	friend class CAnimationState;

public:
	CTransition();
	~CTransition();

private:
	string m_strTransitionTarget;
	vector<PPARAMETER>m_vecParameter;
	vector<PARAMETER_TYPE>m_vecComparisonType;
	vector<PARAMETER_CONDITION> m_vecComparisonCondition;
	vector<PARAMETER> m_vecTransitionValue;
	bool m_bHasExitTime;

public:
	void SetExitTime(bool bHasExitTime);
	bool HasExitTime() const;
	void SetTransitionTarget(const string& strKey);
	void AddParameterCondition(PPARAMETER pParameter, PARAMETER_TYPE eCompareType, PARAMETER_CONDITION eCompareCondition, PARAMETER tTargetValue);
	bool GetTransitionCondition();
	const string& GetTransitionTarget() const;

};

LMNT_END
