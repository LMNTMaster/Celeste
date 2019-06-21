#pragma once

#include "Engine.h"

LMNT_BEGIN
class LMNT_DLL CPathManager
{
	DECLARE_SINGLE(CPathManager)
	
private:
	unordered_map<string, wstring>	m_mapPath;
	char m_strPath[MAX_PATH];

public:
	bool Initialize(HWND hWnd);
	bool CreatePath(const string& strKey, const wchar_t* pPath,
		const string& strBaseKey = ROOT_PATH);
	const wchar_t* FindPath(const string& strKey);
	const char* FindPathMultiByte(const string& strKey);

};
LMNT_END	

