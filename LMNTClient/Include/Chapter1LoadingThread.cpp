#include "Chapter1LoadingThread.h"
#include "ThreadManager.h"
#include "SceneManager.h"
#include "Scene\Chapter0.h"
#include "Scene\Chapter1.h"

CChapter1LoadingThread::CChapter1LoadingThread() :
	m_pLoadingScene(NULL)
{
}


CChapter1LoadingThread::~CChapter1LoadingThread()
{
}

void CChapter1LoadingThread::ReleaseLoadedScene()
{
	SAFE_RELEASE(m_pLoadingScene);
}

CScene * CChapter1LoadingThread::GetLoadingScene() const
{
	return m_pLoadingScene;
}

void CChapter1LoadingThread::Run()
{
	m_pLoadingScene = GET_SINGLE(CSceneManager)->CreateScene<CChapter1>("Chapter1");
}
