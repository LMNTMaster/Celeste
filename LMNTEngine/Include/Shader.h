#pragma once
#include "Ref.h"

LMNT_BEGIN

class LMNT_DLL CShader :
	public CRef
{

private:
	friend class CShaderManager;
	
private:
	CShader();
	~CShader();


private:
	ID3DBlob*		m_pVSBlob;
	ID3D11VertexShader*	m_pVS;
	ID3DBlob*		m_pPSBlob;
	ID3D11PixelShader*	m_pPS;

public:
	ID3DBlob* GetVSBlob()	const;
	ID3DBlob* GetPSBlob()	const;

public:
	bool LoadShader(const string& strTag, const wchar_t* pFileName,
		char* pEntry[ST_END], const string& strPathKey = SHADER_PATH);
	bool LoadVertexShader(const wchar_t* pFileName, char* pEntry,
		const string& strPathKey = SHADER_PATH);
	bool LoadPixelShader(const wchar_t* pFileName, char* pEntry,
		const string& strPathKey = SHADER_PATH);
	void SetShader();

};

LMNT_END
