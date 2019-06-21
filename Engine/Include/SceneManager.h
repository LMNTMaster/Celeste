#pragma once
#include "Engine.h"
#include "Scene.h"

LMNT_BEGIN

enum SCENE_CHANGE_CONDITION
{
	SCC_TO_PREVIOUS,
	SCC_TO_NEXT
};

class LMNT_DLL CSceneManager
{
	DECLARE_SINGLE(CSceneManager)
	
private:
	friend class CCore;

private:
	class CScene* m_pPrevScene;
	class CScene* m_pCurrentScene;
	class CScene* m_pNextScene;


private:
	void ChangeScene();

public:
	void ClearPrevScene();
	void ClearNextScene();
	void SetCurrentScene(CScene* pScene);
	class CScene* GetNextScene() const;
	class CScene* GetPrevScene() const;
	void SetNextScene(class CScene* pNextScene);
	class CScene* GetCurrentScene() const;
	void ChangeScene(class CScene* pNextScene);
	void ChangeScene(SCENE_CHANGE_CONDITION eCondition);

public:
	bool Initialize();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Render(float fTime);


public:
	class CScene* CreateScene();

	template<typename T>
	T* CreateScene(const string& strTag)
	{
		T* pScene = new T;

		if (!pScene->Initialize())
		{
			SAFE_RELEASE(pScene);
			return NULL;
		}
		return pScene;
	}

};

LMNT_END

