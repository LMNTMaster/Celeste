#pragma once
#include "Scene.h"



LMNT_USING

class CTestScene :
	public CScene
{
	friend class CSceneManager;
	friend class CScene;

public:
	CTestScene();
	~CTestScene();

public:
	bool Initialize() override;
};

