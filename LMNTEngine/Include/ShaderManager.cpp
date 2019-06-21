#include "ShaderManager.h"
#include "Shader.h"
#include "Device.h"

LMNT_USING
DEFINITION_SINGLE(CShaderManager)

CShaderManager::CShaderManager() :
	m_iElementSize(0)
{
}


CShaderManager::~CShaderManager()
{
	unordered_map<string, PCONSTANTBUFFER>::iterator iter;
	unordered_map<string, PCONSTANTBUFFER>::iterator iterEnd = m_mapConstantBuffer.end();
	for (iter = m_mapConstantBuffer.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second->pBuffer);
		SAFE_DELETE(iter->second);
	}


	Safe_Release_Map(m_mapInputLayout);
	Safe_Release_Map(m_mapShader);
}

bool CShaderManager::Initialize()
{
	char*	pEntry[ST_END] = {};

	pEntry[ST_VERTEX] = "StandardColorVS";
	pEntry[ST_PIXEL] = "StandardColorPS";
	CShader*	pShader = LoadShader(STANDARD_COLOR_SHADER, L"Standard.fx", pEntry);
	SAFE_RELEASE(pShader);

	pEntry[ST_VERTEX] = "StandardTextureVS";
	pEntry[ST_PIXEL] = "StandardTexturePS";
	pShader = LoadShader(STANDARD_TEXTURE_SHADER, L"Standard.fx", pEntry);
	SAFE_RELEASE(pShader);


	pEntry[ST_VERTEX] = "ColliderVS";
	pEntry[ST_PIXEL] = "ColliderPS";
	pShader = LoadShader(COLLIDER_SHADER, L"Collider.fx", pEntry);
	SAFE_RELEASE(pShader);

	pEntry[ST_VERTEX] = "ColoredTextureVS";
	pEntry[ST_PIXEL] = "ColoredTexturePS";
	pShader = LoadShader(COLORED_TEXTURE_SHADER, L"Standard.fx", pEntry);
	SAFE_RELEASE(pShader);


	pEntry[ST_VERTEX] = "UITextureVS";
	pEntry[ST_PIXEL] = "UITexturePS";
	pShader = LoadShader(UI_TEXTURE_SHADER, L"Standard.fx", pEntry);
	SAFE_RELEASE(pShader);


	AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12);
	AddInputElement("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16);

	CreateInputLayout(POS_COLOR_LAYOUT, STANDARD_COLOR_SHADER);

	AddInputElement("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12);
	AddInputElement("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8);

	CreateInputLayout(POS_UV_LAYOUT, STANDARD_TEXTURE_SHADER);

	// ================= 상수버퍼 ====================
	CreateConstantBuffer(TRANSFORM_CBUFFER, sizeof(TRANSFORMCBUFFER), 0);

	CreateConstantBuffer(COLLIDER_CBUFFER, sizeof(COLLIDERCBUFFER), 11);

	CreateConstantBuffer(MATERIAL_CBUFFER, sizeof(MATERIAL), 1);

	CreateConstantBuffer(ANIMATION2D_CBUFFER, sizeof(ANIMATION2DCBUFFER), 9);
	return true;
}

CShader * CShaderManager::LoadShader(const string & strTag, const wchar_t * pFileName, char * pEntry[ST_END], const string & strPathKey)
{
	CShader* pShader = FindShader(strTag);

	if (pShader)
	{
		return pShader;
	}
	pShader = new CShader;

	if (!pShader->LoadShader(strTag, pFileName, pEntry, strPathKey))
	{
		SAFE_RELEASE(pShader);
		return NULL;
	}


	pShader->AddRef();
	m_mapShader.insert(make_pair(strTag, pShader));
	return pShader;
}

//++Reference
CShader * CShaderManager::FindShader(const string & strTag)
{
	unordered_map<string, CShader*>::iterator iter = m_mapShader.find(strTag);

	if (iter == m_mapShader.end())
		return NULL;

	iter->second->AddRef();
	return iter->second;
}

void CShaderManager::AddInputElement(const char * pSemantic, int iSemanticIdx, DXGI_FORMAT eFmt, UINT iInputSlot, UINT iSize, D3D11_INPUT_CLASSIFICATION eInputClass, UINT iInstanceStepRate)
{
	D3D11_INPUT_ELEMENT_DESC	tElement = {};
	tElement.SemanticName = pSemantic;
	tElement.SemanticIndex = iSemanticIdx;
	tElement.Format = eFmt;
	tElement.InputSlot = iInputSlot;
	tElement.AlignedByteOffset = m_iElementSize;
	tElement.InputSlotClass = eInputClass;
	tElement.InstanceDataStepRate = iInstanceStepRate;
	m_iElementSize += iSize;

	m_vecAddElement.push_back(tElement);
}

ID3D11InputLayout * CShaderManager::CreateInputLayout(const string & strKey, const string & strShaderKey)
{
	ID3D11InputLayout*	pLayout = FindInputLayout(strKey);

	if (pLayout)
		return pLayout;

	CShader*	pShader = FindShader(strShaderKey);

	if (!pShader)
		return NULL;

	if (FAILED(DEVICE->CreateInputLayout(&m_vecAddElement[0], m_vecAddElement.size(), pShader->GetVSBlob()->GetBufferPointer(),
		pShader->GetVSBlob()->GetBufferSize(), &pLayout)))
	{
		m_vecAddElement.clear();
		m_iElementSize = 0;
		SAFE_RELEASE(pShader);
		return false;
	}

	m_vecAddElement.clear();
	m_iElementSize = 0;
	SAFE_RELEASE(pShader);

	m_mapInputLayout.insert(make_pair(strKey, pLayout));

	return pLayout;
}

ID3D11InputLayout * CShaderManager::FindInputLayout(const string & strKey)
{
	unordered_map<string, ID3D11InputLayout*>::iterator	iter = m_mapInputLayout.find(strKey);

	if (iter == m_mapInputLayout.end())
		return NULL;

	return iter->second;
}

PCONSTANTBUFFER CShaderManager::CreateConstantBuffer(const string & strKey, UINT iSize, int iRegister)
{
	PCONSTANTBUFFER	pConstantBuffer = FindConstantBuffer(strKey);

	if (pConstantBuffer)
		return pConstantBuffer;

	pConstantBuffer = new CONSTANTBUFFER;

	pConstantBuffer->iSize = iSize;
	pConstantBuffer->iRegister = iRegister;

	D3D11_BUFFER_DESC	tDesc = {};
	tDesc.ByteWidth = iSize;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &pConstantBuffer->pBuffer)))
	{
		SAFE_DELETE(pConstantBuffer);
		return NULL;
	}

	m_mapConstantBuffer.insert(make_pair(strKey, pConstantBuffer));

	return pConstantBuffer;
}

bool CShaderManager::UpdateConstantBuffer(const string & strKey, void * pData, int iCBufferTransfer)
{
	PCONSTANTBUFFER	pCBuffer = FindConstantBuffer(strKey);

	if (!pCBuffer)
		return false;

	D3D11_MAPPED_SUBRESOURCE	tMap = {};

	DEVICE_CONTEXT->Map(pCBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

	memcpy(tMap.pData, pData, pCBuffer->iSize);

	DEVICE_CONTEXT->Unmap(pCBuffer->pBuffer, 0);

	if (iCBufferTransfer & CBT_VERTEX)
		DEVICE_CONTEXT->VSSetConstantBuffers(pCBuffer->iRegister, 1, &pCBuffer->pBuffer);

	if (iCBufferTransfer & CBT_PIXEL)
		DEVICE_CONTEXT->PSSetConstantBuffers(pCBuffer->iRegister, 1, &pCBuffer->pBuffer);

	if (iCBufferTransfer & CBT_GEOMETRY)
		DEVICE_CONTEXT->GSSetConstantBuffers(pCBuffer->iRegister, 1, &pCBuffer->pBuffer);

	return true;
}

PCONSTANTBUFFER CShaderManager::FindConstantBuffer(const string & strKey)
{
	unordered_map<string, PCONSTANTBUFFER>::iterator	iter = m_mapConstantBuffer.find(strKey);

	if (iter == m_mapConstantBuffer.end())
		return NULL;

	return iter->second;
}
