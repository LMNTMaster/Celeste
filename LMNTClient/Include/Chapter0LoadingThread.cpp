#include "Chapter0LoadingThread.h"
#include "ThreadManager.h"
#include "SceneManager.h"
#include "Scene\Chapter0.h"
#include "Scene\Chapter1.h"

CChapter0LoadingThread::CChapter0LoadingThread():
	m_pLoadingScene(NULL)
{
}


CChapter0LoadingThread::~CChapter0LoadingThread()
{
}

void CChapter0LoadingThread::ReleaseLoadedScene()
{
	SAFE_RELEASE(m_pLoadingScene);
}

CScene*  CChapter0LoadingThread::GetLoadingScene() const
{
	return m_pLoadingScene;
}

void CChapter0LoadingThread::Run()
{
	m_pLoadingScene = GET_SINGLE(CSceneManager)->CreateScene<CChapter0>("Chapter0");
}
