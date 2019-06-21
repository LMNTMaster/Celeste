#pragma once

#include "Thread.h"
#include "Scene.h"

LMNT_USING

class CLoadingThread : public CThread
{
private:
	class CScene* m_pCurrentScene;
	class CScene* m_pLoadingScene;

	int m_iStage;
	
public:
	CLoadingThread();
	~CLoadingThread();

public:
	void SetStage(int iStage);
	void SetCurrentScene(class CScene* pScene);
	class CScene*  GetLoadingScene() const;

public:
	void Run() override;


};

