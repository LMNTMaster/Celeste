#include "Thread.h"

LMNT_USING


CThread::CThread()
{
	m_hThread = NULL;
	m_hStartEvent = NULL;
}


CThread::~CThread()
{
	if (m_hStartEvent)
		CloseHandle(m_hStartEvent);

	if (m_hThread)
	{
		// 스레드는 스레드가 종료되면 handle 을 signal 상태로 만들어준다.
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
}

void CThread::Create()
{
	m_hStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, CThread::ThreadFunc,
		this, 0, NULL);

	WaitForSingleObject(m_hStartEvent, INFINITE);
}

void CThread::Start()
{
	SetEvent(m_hStartEvent);
}

UINT CThread::ThreadFunc(void * pArg)
{
	CThread*	pThread = (CThread*)pArg;

	pThread->Start();

	pThread->Run();

	return 0;
}
