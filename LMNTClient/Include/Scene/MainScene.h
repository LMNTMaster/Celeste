#pragma once
#include "Scene.h"

LMNT_USING

class CMainScene:
	public CScene
{
	friend class CSceneManager;
	friend class CScene;

public:
	CMainScene();
	~CMainScene();

private:
	class CGameObject* LoadPrefab(const string& strFileName, class CLayer* pLayer, const Vector3& vPosition);

public:
	bool Initialize() override;
};

