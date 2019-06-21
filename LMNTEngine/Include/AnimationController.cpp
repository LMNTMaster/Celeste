#include "AnimationController.h"
#include "AnimationState.h"
#include "Component\Animator.h"
#include "AnimationClip2D.h"

LMNT_USING

CAnimationController::CAnimationController() :
	m_pEntryState(NULL),
	m_pAnyState(NULL),
	m_pExitState(NULL),
	m_pCurrentState(NULL),
	m_pAnimator(NULL)
{
}

CAnimationController::~CAnimationController()
{

	unordered_map<string, CAnimationState*>::iterator iter;
	unordered_map<string, CAnimationState*>::iterator iterEnd = m_mapState.end();

	for (iter = m_mapState.begin(); iter != iterEnd; ++iter)
	{
		delete iter->second;
	}
	m_mapState.clear();
	
	//위에서 지워줌
	//SAFE_DELETE(m_pEntryState);
	//SAFE_DELETE(m_pAnyState);
	//SAFE_DELETE(m_pExitState);
	//SAFE_DELETE(m_pCurrentState);
	
	//Parameter는 스택에 할당했음.
	//Safe_Release_Map(m_mapState);
}

void CAnimationController::SetBool(const string & strKey, bool bValue)
{
	unordered_map<string, PARAMETER>::iterator iter = m_mapParameter.find(strKey);

	if (iter == m_mapParameter.end())
	{		
		assert(false);
		return;
	}

	iter->second.bValue = bValue;
}

void CAnimationController::SetInteger(const string & strKey, int iValue)
{
	unordered_map<string, PARAMETER>::iterator iter = m_mapParameter.find(strKey);

	if (iter == m_mapParameter.end())
	{
		assert(false);
		return;
	}

	iter->second.iValue = iValue;
}

void CAnimationController::SetFloat(const string & strKey, float fValue)
{
	unordered_map<string, PARAMETER>::iterator iter = m_mapParameter.find(strKey);

	if (iter == m_mapParameter.end())
	{
		assert(false);
		return;
	}

	iter->second.fValue = fValue;
}

bool CAnimationController::GetBool(const string & strKey) const
{
	unordered_map<string, PARAMETER>::const_iterator iter = m_mapParameter.find(strKey);

	if (iter == m_mapParameter.end())
	{
		assert(false);
		return false;
	}

	return iter->second.bValue;
}

int CAnimationController::GetInteger(const string & strKey) const
{
	unordered_map<string, PARAMETER>::const_iterator iter = m_mapParameter.find(strKey);

	if (iter == m_mapParameter.end())
	{
		assert(false);
		return 0;
	}

	return iter->second.iValue;
}

float CAnimationController::GetFloat(const string & strKey) const
{
	unordered_map<string, PARAMETER>::const_iterator iter = m_mapParameter.find(strKey);

	if (iter == m_mapParameter.end())
	{
		assert(false);
		return 0;
	}

	return iter->second.fValue;
}

CAnimationState * CAnimationController::GetCurrentState() const
{
	return m_pCurrentState;
}

CAnimator * CAnimationController::GetAnimator() const
{
	return m_pAnimator;
}

void CAnimationController::SetEntryState(const string & strKey)
{
	CAnimationState* pState = FindAnimationState(strKey);
	
	if(pState)
		m_pEntryState = pState;
}

//Parameter는 이름만 알고 있어도 어떤 값이든 사용할 수 있다. (union)
void CAnimationController::AddParameter(const string & strKey)
{
	unordered_map<string, PARAMETER>::iterator iter = m_mapParameter.find(strKey);

	if (iter == m_mapParameter.end())
	{
		PARAMETER pParam;
		m_mapParameter.insert(make_pair(strKey, pParam));
	}
}

void CAnimationController::RemoveParameter(const string & strKey)
{
	unordered_map<string, PARAMETER>::iterator iter = m_mapParameter.find(strKey);

	if (iter == m_mapParameter.end())
	{
		m_mapParameter.erase(iter);
	}
}

CAnimationState * CAnimationController::CreateAnimationState(const string & strTag)
{
	CAnimationState* pState = FindAnimationState(strTag);

	if (pState)
		return pState;

	pState = new CAnimationState;
	pState->m_pController = this;
	pState->m_pAnimator = m_pAnimator;
	pState->m_strStateName = strTag;

	m_mapState.insert(make_pair(strTag, pState));

	return pState;
}

CAnimationState * CAnimationController::FindAnimationState(const string & strTag)
{
	unordered_map<string, CAnimationState*>::iterator iter = m_mapState.find(strTag);

	if (iter == m_mapState.end())
	{
		return NULL;
	}
	return iter->second;
}

void CAnimationController::EraseAnimationState(const string & strTag)
{
	unordered_map<string, CAnimationState*>::iterator iter = m_mapState.find(strTag);

	if (iter == m_mapState.end())
		return;

	m_mapState.erase(iter);
}

void CAnimationController::TransitionState(const string & strKey)
{
	CAnimationState* pState = FindAnimationState(strKey);

	if (pState)
	{
		if (m_pCurrentState)
		{
			m_pCurrentState->ResetState();
		}
		m_pCurrentState = pState;
		m_pCurrentState->ResetState();
		m_pCurrentState->GetClip()->ResetClip();
		m_pAnimator->SetAnimationStateTransitioned();
	}
}

void CAnimationController::Update(float fTime)
{
	if (m_pCurrentState)
		m_pCurrentState->UpdateCondition(fTime);
	else
	{
		m_pAnimator->SetAnimationStateTransitioned();
		m_pCurrentState = m_pEntryState;
		m_pEntryState = NULL;
	}
}
