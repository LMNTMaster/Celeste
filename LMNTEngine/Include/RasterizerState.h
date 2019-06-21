#pragma once
#include "RenderState.h"

LMNT_BEGIN

class LMNT_DLL CRasterizerState :
	public CRenderState
{
private:
	friend class CRenderManager;

private:
	CRasterizerState();
	~CRasterizerState();

public:
	bool CreateRasterizerState(D3D11_FILL_MODE eFillMode = D3D11_FILL_SOLID,
		D3D11_CULL_MODE eCullMode = D3D11_CULL_BACK);

public:
	virtual void SetState();
	virtual void ResetState();
};

LMNT_END
