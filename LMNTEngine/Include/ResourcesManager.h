#pragma once

#include "Engine.h"

LMNT_BEGIN

class LMNT_DLL CResourcesManager
{
	DECLARE_SINGLE(CResourcesManager)


private:
	unordered_map<string, class CMesh*>		m_mapMesh;
	unordered_map<string, class CTexture*>		m_mapTexture;
	unordered_map<string, class CSampler*>	m_mapSampler;
	unordered_map<string, PATLASDATA>			m_mapAtlasData;

public:
	void InsertAtlasData(PATLASDATA pData);
	PATLASDATA GetAtlasData(const string& strKey);

public:
	bool Initialize();

	class CMesh* CreateMesh(const string& strKey, UINT iVtxCount,
		UINT iVtxSize, D3D11_USAGE eVtxUsage, void* pVtxData,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		UINT iIdxCount = 0, UINT iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eIdxFmt = DXGI_FORMAT_R32_UINT,
		void* pIdxData = NULL);

	void LoadMesh(const string& strKey, const wchar_t* pFileName, const string& strPathKey = TEXTURE_PATH);

	class CMesh* FindMesh(const string& strKey);

	class CTexture* LoadTexture(const string& strKey, const wchar_t* pFileName, 	const string& strPathKey = TEXTURE_PATH);
	class CTexture* LoadTexture(const string& strKey, const vector<wstring>& vecFileName, const string& strPathKey = TEXTURE_PATH);
	class CTexture* LoadTextureFromFullPath(const string& strKey, const wchar_t* pFullPath);
	class CTexture* LoadTextureFromFullPath(const string& strKey, const vector<wstring>& vecFullPath);

	class CTexture* FindTexture(const string& strKey);

	class CSampler* CreateSampler(const string& strKey,
		D3D11_FILTER eFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_MODE eU = D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_MODE eV = D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_MODE eW = D3D11_TEXTURE_ADDRESS_CLAMP,
		float fMipLODBias = 0.f,
		UINT iMaxAnisotropy = 1,
		D3D11_COMPARISON_FUNC eFunc = D3D11_COMPARISON_NEVER,
		const Vector4& vBorderColor = Vector4(0.f, 0.f, 0.f, 0.f),
		float fMinLOD = -FLT_MAX,
		float fMaxLOD = FLT_MAX);
	class CSampler* FindSampler(const string& strKey);

};

LMNT_END	
