#pragma once
#include "Thread.h"
#include "Scene.h"

LMNT_USING

class CChapter0LoadingThread :
	public CThread
{
private:
	class CScene* m_pLoadingScene;
	
public:
	CChapter0LoadingThread();
	~CChapter0LoadingThread();

public:
	void ReleaseLoadedScene();
	class CScene*  GetLoadingScene() const;

public:
	void Run() override;


};

