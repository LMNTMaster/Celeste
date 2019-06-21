#include "Material.h"
#include "Texture.h"
#include "Sampler.h"
#include "ResourcesManager.h"
#include "Device.h"
#include "ShaderManager.h"

LMNT_USING

CMaterial::CMaterial()
{
	m_tMaterial.vDiffuse = Vector4(1.f, 1.f, 1.f, 1.f);
}

CMaterial::CMaterial(const CMaterial & mtrl)
{
	m_tMaterial = mtrl.m_tMaterial;
	m_tShaderResource = mtrl.m_tShaderResource;

	if (m_tShaderResource.pDiffuseTexture)
		m_tShaderResource.pDiffuseTexture->AddRef();

	if (m_tShaderResource.pSampler)
		m_tShaderResource.pSampler->AddRef();
}

CMaterial::~CMaterial()
{
	SAFE_RELEASE(m_tShaderResource.pDiffuseTexture);
	SAFE_RELEASE(m_tShaderResource.pSampler);
}


CMaterial * CMaterial::Clone() const
{
	return new CMaterial(*this);
}



const Vector4 & CMaterial::GetDiffuse() const
{
	return m_tMaterial.vDiffuse;
}

void CMaterial::SetMaterial(const Vector4 & vDiffuse)
{
	m_tMaterial.vDiffuse = vDiffuse;
}

void CMaterial::SetDiffuseTexture(const string & strKey)
{
	SAFE_RELEASE(m_tShaderResource.pDiffuseTexture);
	m_tShaderResource.pDiffuseTexture = GET_SINGLE(CResourcesManager)->FindTexture(strKey);
}

void CMaterial::SetDiffuseTexture(CTexture * pTexture)
{
	SAFE_RELEASE(m_tShaderResource.pDiffuseTexture);

	if (pTexture)
		pTexture->AddRef();
	m_tShaderResource.pDiffuseTexture = pTexture;
}

void CMaterial::SetDiffuseTexture(const string & strKey, const wchar_t * pFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_tShaderResource.pDiffuseTexture);
	m_tShaderResource.pDiffuseTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, pFileName, strPathKey);
}

void CMaterial::SetDiffuseTextureFromFullPath(const string & strKey, const wchar_t * pFullPath)
{
	SAFE_RELEASE(m_tShaderResource.pDiffuseTexture);
	m_tShaderResource.pDiffuseTexture = GET_SINGLE(CResourcesManager)->LoadTextureFromFullPath(strKey, pFullPath);
}

void CMaterial::SetDiffuseTexture(const string & strKey, const vector<wstring>& vecFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_tShaderResource.pDiffuseTexture);
	m_tShaderResource.pDiffuseTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, vecFileName, strPathKey);
}

void CMaterial::SetDiffuseTextureFromFullPath(const string & strKey, const vector<wstring>& vecFileName)
{
	SAFE_RELEASE(m_tShaderResource.pDiffuseTexture);
	m_tShaderResource.pDiffuseTexture = GET_SINGLE(CResourcesManager)->LoadTextureFromFullPath(strKey, vecFileName);
}

void CMaterial::SetDiffuseTextureRegister(int iRegister)
{
	m_tShaderResource.iTextureRegister = iRegister;
}

void CMaterial::SetSampler(const string & strKey)
{
	SAFE_RELEASE(m_tShaderResource.pSampler);
	m_tShaderResource.pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strKey);
}

void CMaterial::SetSampler(CSampler * pSampler)
{
	SAFE_RELEASE(m_tShaderResource.pSampler);

	if (pSampler)
		pSampler->AddRef();
	m_tShaderResource.pSampler = pSampler;
}

void CMaterial::SetSamplerRegister(int iRegister)
{
	m_tShaderResource.iSamplerRegister = iRegister;
}

void CMaterial::SetMaterial()
{
	GET_SINGLE(CShaderManager)->UpdateConstantBuffer(MATERIAL_CBUFFER, &m_tMaterial, CBT_VERTEX | CBT_PIXEL | CBT_GEOMETRY);

	m_tShaderResource.pDiffuseTexture->SetTexture(m_tShaderResource.iTextureRegister);
	m_tShaderResource.pSampler->SetSampler(m_tShaderResource.iSamplerRegister);
}
