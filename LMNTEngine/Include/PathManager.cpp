#include "PathManager.h"

LMNT_USING

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}


CPathManager::~CPathManager()
{
}


bool CPathManager::Initialize(HWND hWnd)
{
	wchar_t strPath[MAX_PATH] = {};

	//���� ���������� ����ִ� �����θ� ������ �Լ�.
	GetModuleFileNameW(NULL, strPath, MAX_PATH);

	//null���ڸ� ������ ���ڿ��� �� �ڿ������� ��ȸ�� �Ѵ�.
	for (int i = lstrlenW(strPath) - 1; i >= 0; --i)
	{
		//���ڿ� �и��� ���ش�.
		if (strPath[i] == '/' || strPath[i] == '\\')
		{
			//�����͸� ����� ���ڿ��� �ּҰ� + �̵��� �� �� ���ؿͼ� �޺κ��� 0���� �� ����������
			//�׷� ���� ���� ���ϴ� ��θ� ���� �� �� �ִ�.
			memset(strPath + (i + 1), 0, sizeof(wchar_t) * (MAX_PATH - (i + 1)));
			break;
		}
	}


	m_mapPath.insert(make_pair(ROOT_PATH, strPath));

	// Shader ��� ����
	if (CreatePath(SHADER_PATH, L"Shader\\") == false)
	{
		return false;
	}

	if (CreatePath(TEXTURE_PATH, L"Texture\\") == false) 
	{
		return false;
	}
	if (CreatePath(DATA_PATH, L"Data\\") == false)
	{
		return false;
	}
	if (CreatePath(PREFAB_PATH, L"Data\\Prefab\\") == false)
	{
		return false;
	}
	if (CreatePath(BGM_PATH, L"Sound\\BGM\\") == false)
	{
		return false;
	}
	if (CreatePath(SFX_PATH, L"Sound\\SFX\\") == false)
	{
		return false;
	}
	if (CreatePath(UI_SFX_PATH, L"Sound\\SFX\\UI\\") == false)
	{
		return false;
	}



	return true;
}

bool CPathManager::CreatePath(const string & strKey,
	const wchar_t * pPath, const string & strBaseKey)
{
	const wchar_t* pBasePath = FindPath(strBaseKey);

	wstring strPath;

	//��θ� ����� ã�ƿ����� ���ڿ��� �޾ƿ� �н��� ������ش�.
	if (pBasePath)
	{
		strPath = pBasePath;
	}

	//���ڿ��� �̾� �ٿ��ش�.
	strPath += pPath;

	m_mapPath.insert(make_pair(strKey, strPath));

	return true;
}

const wchar_t * CPathManager::FindPath(const string & strKey)
{
	unordered_map<string, wstring>::iterator iter = m_mapPath.find(strKey);

	//Ű���� ã�� ���ߴٸ� nullptr �� ��ȯ
	if (iter == m_mapPath.end())
	{
		return nullptr;
	}

	//Ű���� ����� ã�Ҵٸ�
	//�ش� ���ڿ��� ��ȯ���ش�.
	return iter->second.c_str();
}

const char * CPathManager::FindPathMultiByte(const string & strKey)
{
	const wchar_t* pPath = FindPath(strKey);

	if (pPath == nullptr)
	{
		return nullptr;
	}

	memset(m_strPath, 0, sizeof(char) * MAX_PATH);

	WideCharToMultiByte(CP_ACP, 0, pPath, -1, m_strPath, lstrlenW(pPath), 0, 0);

	return m_strPath;
}
