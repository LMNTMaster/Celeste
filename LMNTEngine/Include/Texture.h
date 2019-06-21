#pragma once
#include "Ref.h"
#include "DirectXTex.h"

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex64_Debug")
#else
#pragma comment(lib, "DirectXTex64")
#endif // _DEBUG
#else
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug")
#else
#pragma comment(lib, "DirectXTex")
#endif // _DEBUG
#endif //

LMNT_BEGIN

class LMNT_DLL CTexture :
	public CRef
{
private:
	friend class CResourcesManager;

private:
	CTexture();
	~CTexture();

private:
	vector<ScratchImage*>		m_vecScratchImage;
	ID3D11ShaderResourceView*	m_pRSView;
	string		m_strKey;
	wstring		m_strFullPath;

public:
	bool LoadTexture(const string& strKey, const wchar_t* pFileName, const string& strPathKey);
	bool LoadTexture(const string& strKey, const vector<wstring>& vecFileName, const string& strPathKey);
	bool LoadTextureFromFullPath(const string& strKey, const wchar_t* pFullPath);
	bool LoadTextureFromFullPath(const string& strKey, const vector<wstring>& vecFileName);
	bool CreateShaderResourceView();
	bool CreateShaderResourceArrayView();
	void SetTexture(int iRegister);
};

LMNT_END

