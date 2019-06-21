#include "Transition.h"

LMNT_USING

CTransition::CTransition()
{
}


CTransition::~CTransition()
{
}

void CTransition::SetExitTime(bool bHasExitTime)
{
	m_bHasExitTime = bHasExitTime;
}

bool CTransition::HasExitTime() const
{
	return m_bHasExitTime;
}

void CTransition::SetTransitionTarget(const string & strKey)
{
	m_strTransitionTarget = strKey;
}

const string & CTransition::GetTransitionTarget() const
{
	return m_strTransitionTarget;
}


//pParameter			: Controller�� Parameter�� �ּ�
//eCompareType		: ���� Ÿ��
//eComparisionCondition : �� ������
//tParameter			: ���� ��
void CTransition::AddParameterCondition(PPARAMETER pParameter, PARAMETER_TYPE eCompareType, PARAMETER_CONDITION eCompareCondition, PARAMETER tTargetValue)
{
	m_vecParameter.push_back(pParameter);
	m_vecComparisonType.push_back(eCompareType);
	m_vecComparisonCondition.push_back(eCompareCondition);
	m_vecTransitionValue.push_back(tTargetValue);
}

//Transition����		: True
//Transition�Ұ���	: False
bool CTransition::GetTransitionCondition()
{
	bool bResult = true;

	for (int i = 0; i < m_vecParameter.size(); ++i)
	{
		if (m_vecParameter[i])
		{
			switch (m_vecComparisonType[i])
			{
			case LMNT::PT_BOOL:
			{
				switch (m_vecComparisonCondition[i])
				{
				case LMNT::PC_EQUALS:
				{
					bResult &= m_vecParameter[i]->bValue == m_vecTransitionValue[i].bValue;
				}break;
				case LMNT::PC_NOT_EQUAL:
				{
					bResult &= m_vecParameter[i]->bValue != m_vecTransitionValue[i].bValue;
				}break;
				}
			}
			break;

			case LMNT::PT_INT:
			{
				switch (m_vecComparisonCondition[i])
				{
				case LMNT::PC_GRATER:
				{
					bResult &= m_vecParameter[i]->iValue > m_vecTransitionValue[i].iValue;
				}break;
				case LMNT::PC_LESS:
				{
					bResult &= m_vecParameter[i]->iValue < m_vecTransitionValue[i].iValue;
				}break;
				case LMNT::PC_EQUALS:
				{
					bResult &= m_vecParameter[i]->iValue == m_vecTransitionValue[i].iValue;
				}break;
				case LMNT::PC_NOT_EQUAL:
				{
					bResult &= m_vecParameter[i]->iValue != m_vecTransitionValue[i].iValue;
				}break;
				}
			}
			break;
			case LMNT::PT_FLOAT:
			{
				//�Ǽ� ���������� ��Һ񱳸� �ϵ��� ��.
				switch (m_vecComparisonCondition[i])
				{
				case LMNT::PC_GRATER:
				{
					bResult &= m_vecParameter[i]->fValue > m_vecTransitionValue[i].fValue;
				}break;
				case LMNT::PC_LESS:
				{
					bResult &= m_vecParameter[i]->fValue < m_vecTransitionValue[i].fValue;
				}break;
				}
			}break;
			}
		}
	}


	return bResult;
}

