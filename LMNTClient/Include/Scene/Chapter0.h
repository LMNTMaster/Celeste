#pragma once
#include "Scene.h"

LMNT_USING

class CChapter0:
	public CScene
{
	friend class CSceneManager;
	friend class CScene;

public:
	CChapter0();
	~CChapter0();

private:
	class CGameObject* LoadPrefab(const string& strFileName, class CLayer* pLayer, const Vector3& vPosition);

	bool LoadSoundFiles();
public:
	bool Initialize() override;
	void Start() override;
};

