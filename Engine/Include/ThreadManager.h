#pragma once

#include "Engine.h"

LMNT_BEGIN

class LMNT_DLL CThreadManager
{
	DECLARE_SINGLE(CThreadManager)

private:
	unordered_map<string, class CThread*>	m_mapThread;

public:
	bool Initialize();
	class CThread* FindThread(const string& strKey);

public:
	template <typename T>
	T* CreateThread(const string& strKey)
	{
		if (FindThread(strKey))
			return NULL;

		T* pThread = new T;

		pThread->Create();

		m_mapThread.insert(make_pair(strKey, pThread));

		return pThread;
	}

};

LMNT_END
