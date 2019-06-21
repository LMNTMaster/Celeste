#pragma once

#include "Engine.h"

LMNT_BEGIN

class LMNT_DLL CThread
{
	friend class CThreadManager;

protected:
	CThread();

public:
	virtual ~CThread() = 0;

private:
	HANDLE		m_hThread;
	HANDLE		m_hStartEvent;

public:
	void Create();
	void Start();

public:
	virtual void Run() = 0;

public:
	static UINT __stdcall ThreadFunc(void* pArg);
};

LMNT_END
