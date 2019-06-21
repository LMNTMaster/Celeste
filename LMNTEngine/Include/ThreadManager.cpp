#include "ThreadManager.h"

LMNT_USING

DEFINITION_SINGLE(CThreadManager)

CThreadManager::CThreadManager()
{
}

CThreadManager::~CThreadManager()
{
	Safe_Delete_Map(m_mapThread);
}

bool CThreadManager::Initialize()
{
	return true;
}

CThread * CThreadManager::FindThread(const string & strKey)
{
	unordered_map<string, class CThread*>::iterator	iter = m_mapThread.find(strKey);

	if (iter == m_mapThread.end())
		return NULL;

	return iter->second;
}
