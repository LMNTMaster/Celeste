#pragma once
#include "Ref.h"

LMNT_BEGIN

typedef	struct LMNT_DLL _tagShaderResource
{
	class CTexture*	pDiffuseTexture;
	class CSampler*	pSampler;
	int		iTextureRegister;
	int		iSamplerRegister;

	_tagShaderResource() :
		pDiffuseTexture(NULL),
		pSampler(NULL),
		iTextureRegister(0),
		iSamplerRegister(0)
	{
	}
}SHADERRESOURCE, *PSHADERRESOURCE;

class LMNT_DLL CMaterial :
	public CRef
{
private:
	friend class CRenderer;

private:
	CMaterial();
	CMaterial(const CMaterial& mtrl);
	~CMaterial();

private:
	MATERIAL					m_tMaterial;
	SHADERRESOURCE		m_tShaderResource;

public:
	const Vector4& GetDiffuse() const;

	void SetMaterial(const Vector4& vDiffuse);

	void SetDiffuseTexture(const string& strKey);

	void SetDiffuseTexture(class CTexture* pTexture);
	
	//Atlas Animation
	void SetDiffuseTexture(const string& strKey, const wchar_t* pFileName,	const string& strPathKey = TEXTURE_PATH);
	void SetDiffuseTextureFromFullPath(const string& strKey, const wchar_t* pFullPath);
	//Frame Animation
	void SetDiffuseTexture(const string& strKey, const vector<wstring>& vecFileName, const string& strPathKey);
	void SetDiffuseTextureFromFullPath(const string& strKey, const vector<wstring>& vecFileName);

	void SetDiffuseTextureRegister(int iRegister);
	void SetSampler(const string& strKey);
	void SetSampler(class CSampler* pSampler);
	void SetSamplerRegister(int iRegister);

public:
	CMaterial* Clone()	const;

public:
	void SetMaterial();
};

LMNT_END


