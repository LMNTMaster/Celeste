#pragma once
#include "Scene.h"

LMNT_USING

class CIntroScene :
	public CScene
{
	friend class CSceneManager;
	friend class CScene;

public:
	CIntroScene();
	~CIntroScene();

public:
	bool Initialize() override;
};

