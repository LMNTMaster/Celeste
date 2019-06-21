#include "SceneManager.h"
#include "Scene.h"
LMNT_USING

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager() :
	m_pCurrentScene(NULL),
	m_pNextScene(NULL),
	m_pPrevScene(NULL)
{
}

CSceneManager::~CSceneManager()
{
	SAFE_RELEASE(m_pCurrentScene);
	SAFE_RELEASE(m_pNextScene);
	SAFE_RELEASE(m_pPrevScene);
}

void CSceneManager::ClearPrevScene()
{
	SAFE_RELEASE(m_pPrevScene);
}

void CSceneManager::ClearNextScene()
{
	SAFE_RELEASE(m_pNextScene);
}

void CSceneManager::SetCurrentScene(CScene * pScene)
{
	pScene->AddRef();
	m_pCurrentScene = pScene;
}

CScene * CSceneManager::GetNextScene() const
{
	return m_pNextScene;
}
CScene * CSceneManager::GetPrevScene() const
{
	return m_pPrevScene;
}

void CSceneManager::SetNextScene(CScene * pNextScene)
{
	m_pNextScene = pNextScene;
}

CScene * CSceneManager::GetCurrentScene() const
{
	m_pCurrentScene->AddRef();
	return m_pCurrentScene;
}

void CSceneManager::ChangeScene() 
{
	if (m_pNextScene)
	{
		//바뀌는 Scene을 Prev에 넣어서 보관하도록 한다.
		SAFE_RELEASE(m_pPrevScene);
		m_pPrevScene = m_pCurrentScene;
		m_pCurrentScene = m_pNextScene;
		m_pCurrentScene->AddRef();
		SAFE_RELEASE(m_pNextScene);
	}
}

void CSceneManager::ChangeScene(CScene * pNextScene)
{
	SAFE_RELEASE(m_pNextScene);
	m_pNextScene = pNextScene;
}

//미리 Prev 또는 Next Scene이 설정이 되어 있어야 유효하다.
//Current -> Prev 시 Prev 가 Next로 이전되고, 기존의 Next는 해제.
//
void CSceneManager::ChangeScene(SCENE_CHANGE_CONDITION eCondition)
{
	if (eCondition == SCC_TO_PREVIOUS)
	{
		if (!m_pPrevScene)
			return;

		else
		{
			//Next에 Prev를 넣어주고, Scene을 전환할 것이기 때문에 이미 Next에 Scene이 있다면 해제해준다.
			if (m_pNextScene)
			{
				SAFE_RELEASE(m_pNextScene);
			}
			else
			{
				// ChangeScene할 때 Release하기때문에 Ref추가함
				m_pPrevScene->AddRef();
				m_pNextScene = m_pPrevScene;
			}
		}
	}
	else if (eCondition == SCC_TO_NEXT)
	{
		if (!m_pNextScene)
		{
			return;
		}
	}
}

bool CSceneManager::Initialize()
{
	return true;
}

int CSceneManager::Input(float fTime)
{
	return m_pCurrentScene->Input(fTime);
}

int CSceneManager::Update(float fTime)
{
	if (!m_pCurrentScene->m_bInitialized)
	{
		m_pCurrentScene->Start();
		m_pCurrentScene->m_bInitialized = true;
	}

	return m_pCurrentScene->Update(fTime);
}

int CSceneManager::LateUpdate(float fTime)
{
	return m_pCurrentScene->LateUpdate(fTime);
}


void CSceneManager::Render(float fTime)
{
	m_pCurrentScene->Render(fTime);
}

CScene * CSceneManager::CreateScene()
{
	CScene* pScene = new CScene;

	if (!pScene->Initialize())
	{
		SAFE_RELEASE(pScene);
		return NULL;
	}

	return pScene;
}
