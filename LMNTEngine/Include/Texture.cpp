#include "Texture.h"
#include "Device.h"
#include "PathManager.h"

LMNT_USING


CTexture::CTexture() :
	m_pRSView(NULL)
{
}

CTexture::~CTexture()
{
	SAFE_RELEASE(m_pRSView);
	Safe_Delete_VecList(m_vecScratchImage);
}

bool CTexture::LoadTexture(const string & strKey, const wchar_t * pFileName,
	const string & strPathKey)
{
	wstring	strPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	strPath += pFileName;

	if (!LoadTextureFromFullPath(strKey, strPath.c_str()))
		return false;

	return CreateShaderResourceView();
}

bool CTexture::LoadTexture(const string & strKey, const vector<wstring>& vecFileName, const string & strPathKey)
{
	wstring	strPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	for (size_t i = 0; i < vecFileName.size(); ++i)
	{
		wstring strFullPath = strPath;
		strFullPath += vecFileName[i];
		m_strKey = strKey;

		m_strFullPath = strFullPath;

		ScratchImage*	pImage = new ScratchImage;

		// 이미지 로딩을 위한 확장자 포맷을 구분한다.
		wchar_t	strExt[_MAX_EXT] = {};

		_wsplitpath_s(strFullPath.c_str(), NULL, 0, NULL, 0, NULL, 0, strExt, _MAX_EXT);

		char	strExt1[_MAX_EXT] = {};

		WideCharToMultiByte(CP_ACP, 0, strExt, -1, strExt1, lstrlenW(strExt), 0, 0);

		_strupr_s(strExt1);

		if (strcmp(strExt1, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(strFullPath.c_str(), DDS_FLAGS_NONE, NULL, *pImage)))
			{
				assert(false);
				return false;
			}
		}

		else if (strcmp(strExt1, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(strFullPath.c_str(), NULL, *pImage)))
			{
				assert(false);
				return false;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(strFullPath.c_str(), WIC_FLAGS_NONE, NULL, *pImage)))
			{
				assert(false);
				return false;
			}
		}

		m_vecScratchImage.push_back(pImage);
	}

	return CreateShaderResourceArrayView();
}

bool CTexture::LoadTextureFromFullPath(const string & strKey, const wchar_t * pFullPath)
{
	m_strKey = strKey;

	m_strFullPath = pFullPath;

	ScratchImage*	pImage = new ScratchImage;

	// 이미지 로딩을 위한 확장자 포맷을 구분한다.
	wchar_t	strExt[_MAX_EXT] = {};

	_wsplitpath_s(pFullPath, NULL, 0, NULL, 0, NULL, 0, strExt, _MAX_EXT);

	char	strExt1[_MAX_EXT] = {};

	WideCharToMultiByte(CP_ACP, 0, strExt, -1, strExt1, lstrlenW(strExt), 0, 0);

	_strupr_s(strExt1);

	if (strcmp(strExt1, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, NULL, *pImage)))
		{
			assert(false);
			return false;
		}
	}

	else if (strcmp(strExt1, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(pFullPath, NULL, *pImage)))
		{
			assert(false);
			return false;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(pFullPath, WIC_FLAGS_NONE, NULL, *pImage)))
		{
			assert(false);
			return false;
		}
	}

	m_vecScratchImage.push_back(pImage);

	return true;
}

bool CTexture::LoadTextureFromFullPath(const string & strKey, const vector<wstring>& vecFileName)
{
	for (size_t i = 0; i < vecFileName.size(); ++i)
	{
		m_strKey = strKey;

		m_strFullPath = vecFileName[i];

		ScratchImage*	pImage = new ScratchImage;

		// 이미지 로딩을 위한 확장자 포맷을 구분한다.
		wchar_t	strExt[_MAX_EXT] = {};

		_wsplitpath_s(vecFileName[i].c_str(), NULL, 0, NULL, 0, NULL, 0, strExt, _MAX_EXT);

		char	strExt1[_MAX_EXT] = {};

		WideCharToMultiByte(CP_ACP, 0, strExt, -1, strExt1, lstrlenW(strExt), 0, 0);

		_strupr_s(strExt1);

		if (strcmp(strExt1, ".DDS") == 0)
		{
			if (FAILED(LoadFromDDSFile(vecFileName[i].c_str(), DDS_FLAGS_NONE, NULL, *pImage)))
			{
				assert(false);
				return false;
			}
		}

		else if (strcmp(strExt1, ".TGA") == 0)
		{
			if (FAILED(LoadFromTGAFile(vecFileName[i].c_str(), NULL, *pImage)))
			{
				assert(false);
				return false;
			}
		}

		else
		{
			if (FAILED(LoadFromWICFile(vecFileName[i].c_str(), WIC_FLAGS_NONE, NULL, *pImage)))
			{
				assert(false);
				return false;
			}
		}

		m_vecScratchImage.push_back(pImage);
	}

	return CreateShaderResourceArrayView();
}

bool CTexture::CreateShaderResourceView()
{
	if (FAILED(DirectX::CreateShaderResourceView(DEVICE, m_vecScratchImage[0]->GetImages(),
		m_vecScratchImage[0]->GetImageCount(),
		m_vecScratchImage[0]->GetMetadata(),
		&m_pRSView)))
		return false;

	return true;
}

bool CTexture::CreateShaderResourceArrayView()
{
	vector<ID3D11Texture2D*>	vecTexture;

	for (size_t i = 0; i < m_vecScratchImage.size(); ++i)
	{
		ID3D11Texture2D*	pTexture = NULL;

		if (FAILED(DirectX::CreateTextureEx(DEVICE, m_vecScratchImage[i]->GetImages(),
			m_vecScratchImage[i]->GetImageCount(),
			m_vecScratchImage[i]->GetMetadata(),
			D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ, 0, FALSE,
			(ID3D11Resource**)&pTexture)))
		{
			assert(false);
			return false;
		}

		vecTexture.push_back(pTexture);
	}

	D3D11_TEXTURE2D_DESC	tTexDesc = {};
	vecTexture[0]->GetDesc(&tTexDesc);

	// Array Texture를 만들기 위한 Desc 구조체를 생성한다.
	D3D11_TEXTURE2D_DESC	tArrayDesc = {};
	tArrayDesc.Width = tTexDesc.Width;
	tArrayDesc.Height = tTexDesc.Height;
	tArrayDesc.MipLevels = tTexDesc.MipLevels;
	tArrayDesc.ArraySize = vecTexture.size();
	tArrayDesc.Format = tTexDesc.Format;
	tArrayDesc.SampleDesc.Count = 1;
	tArrayDesc.SampleDesc.Quality = 0;
	tArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	tArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// 위에서 설정한 정보를 이용해서 비어있는 Array Texture를 생성한다.
	ID3D11Texture2D*	pArrayTexture = NULL;
	if (FAILED(DEVICE->CreateTexture2D(&tArrayDesc, NULL, &pArrayTexture)))
	{
		assert(false);
		return false;
	}

	// Array Texture의 픽셀정보를 불러온 텍스쳐의 픽셀정보로 채워준다.
	for (size_t i = 0; i < vecTexture.size(); ++i)
	{
		// 각각의 텍스쳐들을 밉맵 수준만큼 반복한다.
		for (size_t mip = 0; mip < tTexDesc.MipLevels; ++mip)
		{
			// 텍스처를 Map을 걸어준다.
			D3D11_MAPPED_SUBRESOURCE	tMap = {};
			DEVICE_CONTEXT->Map(vecTexture[i], mip, D3D11_MAP_READ, 0, &tMap);

			DEVICE_CONTEXT->UpdateSubresource(pArrayTexture,
				D3D11CalcSubresource(mip, i, tTexDesc.MipLevels),
				NULL, tMap.pData, tMap.RowPitch, tMap.DepthPitch);

			DEVICE_CONTEXT->Unmap(vecTexture[i], mip);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC	tViewDesc = {};
	tViewDesc.Format = tArrayDesc.Format;
	tViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	tViewDesc.Texture2DArray.MostDetailedMip = 0;
	tViewDesc.Texture2DArray.MipLevels = tArrayDesc.MipLevels;
	tViewDesc.Texture2DArray.FirstArraySlice = 0;
	tViewDesc.Texture2DArray.ArraySize = vecTexture.size();

	if (FAILED(DEVICE->CreateShaderResourceView(pArrayTexture, &tViewDesc, &m_pRSView)))
	{
		assert(false);
		return false;
	}

	SAFE_RELEASE(pArrayTexture);

	Safe_Release_VecList(vecTexture);

	return true;
}

void CTexture::SetTexture(int iRegister)
{
	DEVICE_CONTEXT->PSSetShaderResources(iRegister, 1, &m_pRSView);
}
