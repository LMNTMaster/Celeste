#include "AnimationState.h"
#include "Transition.h"
#include "AnimationClip2D.h"
#include "Component\Animator.h"
#include "AnimationController.h"

LMNT_USING

CAnimationState::CAnimationState():
	m_pController(NULL),
	m_pAnimator(NULL),
	m_bReserveTransition(false),
	m_pAnimationClip(NULL),
	m_pTransition(NULL)
{
	//m_strStateName �� Controller���� State �߰� �� ���� ����.

}

CAnimationState::~CAnimationState()
{
	SAFE_DELETE(m_pAnimationClip);
}

bool CAnimationState::End() const
{
	return m_bEnd;
}

CAnimationClip2D * CAnimationState::GetClip() const
{
	return m_pAnimationClip;
}

bool CAnimationState::AddClip(const string & strClipName, ANIMATION_OPTION eOption, float fFrameTime,
	const string & strTextureKey, const wchar_t * pFileName, const string & strPathKey)
{
	CAnimationClip2D* pClip = new CAnimationClip2D;
	pClip->m_pAnimationController = m_pController;
	pClip->m_pAnimationState = this;
	pClip->m_pAnimator = m_pController->GetAnimator();

	pClip->SetClipInfo(strClipName, eOption, fFrameTime, strTextureKey, pFileName, strPathKey);
	m_pAnimationClip = pClip;
	return true;
}

//FrameMax : �ؽ����� ��ü ũ��
//Start : ���� ������ ��ġ
//Length : ����
bool CAnimationState::AddClip(const string & strClipName, ANIMATION_OPTION eOption, int iFrameMaxX, int iFrameMaxY, int iStartX, int iStartY, int iLengthX, int iLengthY, float fFrameTime, const string & strTexKey, const wchar_t * pFileName, const string & strPathKey)
{
	CAnimationClip2D* pClip = new CAnimationClip2D;
	pClip->m_pAnimationController = m_pController;
	pClip->m_pAnimationState = this;
	pClip->m_pAnimator = m_pController->GetAnimator();

	pClip->SetClipInfo(strClipName, eOption, iFrameMaxX, iFrameMaxY, iStartX, iStartY,
		iLengthX, iLengthY, fFrameTime, strTexKey, pFileName, strPathKey);

	m_pAnimationClip = pClip;
	return true;
}

bool CAnimationState::AddClip(const string & strClipName, ANIMATION_OPTION eOption, int iFrameMax, float fFrameTime, const string & strTexKey, vector<wstring>* vecFileName, const string & strPathKey)
{
	CAnimationClip2D*	pClip = new CAnimationClip2D;
	pClip->m_pAnimationController = m_pController;
	pClip->m_pAnimationState = this;
	pClip->m_pAnimator = m_pController->GetAnimator();

	pClip->SetClipInfo(strClipName, eOption, iFrameMax, fFrameTime,
		strTexKey, vecFileName, strPathKey);

	m_pAnimationClip = pClip;
	return true;
}

void CAnimationState::AddTransition(CTransition * pTransition)
{
	m_vecTransition.push_back(pTransition);
}

void CAnimationState::ResetState()
{
	m_bReserveTransition = false;
}

const string & CAnimationState::GetStateName() const
{
	return m_strStateName;
}

void CAnimationState::UpdateCondition(float fTime)
{
	m_bEnd = m_pAnimationClip->Update(fTime);

	
	//bool bIsEnd = m_pAnimationClip->Update(fTime);

	if (!m_bReserveTransition)
	{
		//Transition�� �Ѱ��� ���ٸ� ���� ��ü�� ���ϱ� ������ Clip�� ���¿� ���� �ݺ�, �Ǵ� GameObject ����.
		for (size_t i = 0; i < m_vecTransition.size(); ++i)
		{
			if (m_vecTransition[i]->GetTransitionCondition())
			{
				//Transition �� �� �ֵ��� ������.
				m_bReserveTransition = true;
				m_pTransition = m_vecTransition[i];
			}
		}
	}

	if (m_bReserveTransition)
	{
		//ExitTime�� �ְ�, ������ �������̶��
		if (m_pTransition->m_bHasExitTime && m_bEnd)
		{
			//Transition!!
			m_pController->TransitionState(m_pTransition->GetTransitionTarget());
		}
		else if (m_pTransition->m_bHasExitTime && !m_bEnd)
		{
			//bIsEnd�� True�� ������ ��ٸ�
		}
		else if (!m_pTransition->m_bHasExitTime)
		{
			//Transition!!
			m_pController->TransitionState(m_pTransition->GetTransitionTarget());
		}
	}
}
