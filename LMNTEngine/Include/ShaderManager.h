#pragma once

#include "Engine.h"

LMNT_BEGIN

typedef struct _tagConstantBuffer
{
	ID3D11Buffer*	pBuffer;
	int				iSize;
	int				iRegister;
}CONSTANTBUFFER, *PCONSTANTBUFFER;


class CShaderManager
{
	DECLARE_SINGLE(CShaderManager)
private:
	unordered_map<string, class CShader*>	m_mapShader;
	unordered_map<string, ID3D11InputLayout*>	m_mapInputLayout;
	unordered_map<string, PCONSTANTBUFFER> m_mapConstantBuffer;
	vector<D3D11_INPUT_ELEMENT_DESC>	m_vecAddElement;

	UINT		m_iElementSize;

public:
	bool Initialize();
	class CShader* LoadShader(const string& strTag, const wchar_t* pFileName, char* pEntry[ST_END], const string& strPathKey = SHADER_PATH);
	class CShader* FindShader(const string& strTag);


	void AddInputElement(const char* pSemantic, int iSemanticIdx, DXGI_FORMAT eFmt, UINT iInputSlot, UINT iSize,
		D3D11_INPUT_CLASSIFICATION eInputClass = D3D11_INPUT_PER_VERTEX_DATA, UINT iInstanceStepRate = 0);
	ID3D11InputLayout* CreateInputLayout(const string& strKey, const string& strShaderKey);
	ID3D11InputLayout* FindInputLayout(const string& strKey);

	PCONSTANTBUFFER CreateConstantBuffer(const string& strKey, UINT iSize, int iRegister);
	bool UpdateConstantBuffer(const string& strKey, void* pData, int iCBufferTransfer);
	PCONSTANTBUFFER FindConstantBuffer(const string& strKey);

};

LMNT_END
