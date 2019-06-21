#pragma once
#include "Scene.h"

LMNT_USING

class CChapter1 :
	public CScene
{
private:
	friend class CSceneManager;
	friend class CScene;

public:
	CChapter1();
	~CChapter1();

private:
	bool LoadSoundFiles();
	
public:
	bool Initialize() override;
	void Start() override;
};

