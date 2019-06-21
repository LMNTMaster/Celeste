#include "Sampler.h"
#include "Device.h"

LMNT_USING

CSampler::CSampler() :
	m_pSampler(NULL)
{
}


CSampler::~CSampler()
{
	SAFE_RELEASE(m_pSampler);
}

bool CSampler::CreateSampler(const string & strKey, D3D11_FILTER eFilter, D3D11_TEXTURE_ADDRESS_MODE eU, D3D11_TEXTURE_ADDRESS_MODE eV, D3D11_TEXTURE_ADDRESS_MODE eW, float fMipLODBias, UINT iMaxAnisotropy, D3D11_COMPARISON_FUNC eFunc, const Vector4 & vBorderColor, float fMinLOD, float fMaxLOD)
{
	D3D11_SAMPLER_DESC	tDesc = {};
	tDesc.Filter = eFilter;
	tDesc.AddressU = eU;
	tDesc.AddressV = eV;
	tDesc.AddressW = eW;
	tDesc.MinLOD = fMinLOD;
	tDesc.MaxLOD = fMaxLOD;
	memcpy(tDesc.BorderColor, &vBorderColor, sizeof(float) * 4);
	tDesc.ComparisonFunc = eFunc;
	tDesc.MaxAnisotropy = iMaxAnisotropy;
	tDesc.MipLODBias = fMipLODBias;

	if (FAILED(DEVICE->CreateSamplerState(&tDesc, &m_pSampler)))
	{
		assert(false);
		return false;
	}

	return true;
}

void CSampler::SetSampler(int iRegister)
{
	DEVICE_CONTEXT->PSSetSamplers(iRegister, 1, &m_pSampler);
}
