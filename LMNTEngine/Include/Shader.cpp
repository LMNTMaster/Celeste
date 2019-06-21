#include "Shader.h"
#include "PathManager.h"
#include "Device.h"

LMNT_USING

CShader::CShader() :
	m_pVSBlob(NULL),
	m_pVS(NULL),
	m_pPSBlob(NULL),
	m_pPS(NULL)
{
}


CShader::~CShader()
{
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pPS);
}

ID3DBlob * CShader::GetVSBlob() const
{
	return m_pVSBlob;
}

ID3DBlob * CShader::GetPSBlob() const
{
	return m_pPSBlob;
}

bool CShader::LoadShader(const string & strTag, const wchar_t * pFileName, char * pEntry[ST_END], const string & strPathKey)
{
	SetTag(strTag);

	if (!LoadVertexShader(pFileName, pEntry[ST_VERTEX], strPathKey))
		return false;

	if (!LoadPixelShader(pFileName, pEntry[ST_PIXEL], strPathKey))
		return false;

	return true;
}

bool CShader::LoadVertexShader(const wchar_t * pFileName, char * pEntry, const string & strPathKey)
{
	UINT	iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif // _DEBUG

	// ��θ� ������ش�.
	const wchar_t*	pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);
	wstring	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	ID3DBlob*	pErr = NULL;

	


	if (FAILED(D3DCompileFromFile(strFullPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry, "vs_5_0", iFlag, 0, &m_pVSBlob, &pErr)))
	{
		MessageBoxA(NULL, (char*)pErr->GetBufferPointer(),
			"Shader Error", MB_OK);
		return false;
	}

	// Shader�� ������ش�.
	if (FAILED(DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(), NULL,
		&m_pVS)))
		return false;

	return true;
}

bool CShader::LoadPixelShader(const wchar_t * pFileName, char * pEntry, const string & strPathKey)
{
	UINT	iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif // _DEBUG

	// ��θ� ������ش�.
	const wchar_t*	pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);
	wstring	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	ID3DBlob*	pErr = NULL;
	if (FAILED(D3DCompileFromFile(strFullPath.c_str(), NULL,	D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry, "ps_5_0", iFlag, 0, &m_pPSBlob, &pErr)))
	{
		MessageBoxA(NULL, (char*)pErr->GetBufferPointer(),
			"Shader Error", MB_OK);
		return false;
	}

	// Shader�� ������ش�.
	if (FAILED(DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(),
		m_pPSBlob->GetBufferSize(), NULL,
		&m_pPS)))
		return false;

	return true;
}

// ������ ���������ο� �ش� Shader�� �����Ѵ�.
void CShader::SetShader()
{
	DEVICE_CONTEXT->VSSetShader(m_pVS, NULL, 0);
	DEVICE_CONTEXT->PSSetShader(m_pPS, NULL, 0);
}
