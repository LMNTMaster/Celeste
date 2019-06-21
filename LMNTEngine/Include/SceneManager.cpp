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
		//�ٲ�� Scene�� Prev�� �־ �����ϵ��� �Ѵ�.
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

//�̸� Prev �Ǵ� Next Scene�� ������ �Ǿ� �־�� ��ȿ�ϴ�.
//Current -> Prev �� Prev �� Next�� �����ǰ�, ������ Next�� ����.
//
void CSceneManager::ChangeScene(SCENE_CHANGE_CONDITION eCondition)
{
	if (eCondition == SCC_TO_PREVIOUS)
	{
		if (!m_pPrevScene)
			return;

		else
		{
			//Next�� Prev�� �־��ְ�, Scene�� ��ȯ�� ���̱� ������ �̹� Next�� Scene�� �ִٸ� �������ش�.
			if (m_pNextScene)
			{
				SAFE_RELEASE(m_pNextScene);
			}
			else
			{
				// ChangeScene�� �� Release�ϱ⶧���� Ref�߰���
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
