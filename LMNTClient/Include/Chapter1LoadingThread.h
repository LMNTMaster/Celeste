#pragma once
#include "Thread.h"
#include "Scene.h"

LMNT_USING

class CChapter1LoadingThread :
	public CThread
{
private:
	class CScene* m_pLoadingScene;

public:
	CChapter1LoadingThread();
	~CChapter1LoadingThread();

public:
	void ReleaseLoadedScene();
	class CScene*  GetLoadingScene() const;

public:
	void Run() override;

};

