#include "LoadingThread.h"
#include "ThreadManager.h"
#include "SceneManager.h"
#include "Scene\Chapter0.h"
#include "Scene\Chapter1.h"

CLoadingThread::CLoadingThread():
	m_pCurrentScene(NULL),
	m_pLoadingScene(NULL)
{
}


CLoadingThread::~CLoadingThread()
{
}

void CLoadingThread::SetStage(int iStage)
{
	m_iStage = iStage;
}

void CLoadingThread::SetCurrentScene(CScene * pScene)
{
	m_pCurrentScene = pScene;
}

class CScene*  CLoadingThread::GetLoadingScene() const
{
	return m_pLoadingScene;
}

void CLoadingThread::Run()
{
	m_pLoadingScene = GET_SINGLE(CSceneManager)->CreateScene<CChapter1>("Stage0");
}
