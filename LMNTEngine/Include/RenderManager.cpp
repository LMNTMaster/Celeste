#include "RenderManager.h"
#include "ShaderManager.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "RasterizerState.h"


LMNT_USING

DEFINITION_SINGLE(CRenderManager);

CRenderManager::CRenderManager()
{
	m_pDepthDisable = NULL;
	m_pCullNone = NULL;
	m_eRenderMode = RM_3D;
}

CRenderManager::~CRenderManager()
{
	SAFE_RELEASE(m_pCullNone);
	SAFE_RELEASE(m_pDepthDisable);
	Safe_Release_Map(m_mapRenderState);
	DESTROY_SINGLE(CShaderManager);
}


RENDER_MODE CRenderManager::GetRenderMode() const
{
	return m_eRenderMode;
}

void CRenderManager::SetRenderMode(RENDER_MODE eMode)
{
	m_eRenderMode = eMode;

	if (eMode == RM_2D)
	{
		m_pDepthDisable = (CDepthStencilState*)FindRenderState(DEPTH_DISABLE);
		m_pCullNone = (CRasterizerState*)FindRenderState(CULLNONE);
	}
	else if (eMode == RM_3D)
	{
		m_pDepthDisable = (CDepthStencilState*)FindRenderState(DEPTH_ENABLE);
		m_pCullNone = (CRasterizerState*)FindRenderState(CULLBACK);
	}
	else
	{
		SAFE_RELEASE(m_pCullNone);
		SAFE_RELEASE(m_pDepthDisable);
	}
}

void CRenderManager::SetDepth()
{
	/*if (m_pDepthDisable)
	m_pDepthDisable->SetState();

	if (m_pCullNone)
	m_pCullNone->SetState();*/
}

void CRenderManager::ResetDepth()
{
	/*if (m_pDepthDisable)
	m_pDepthDisable->ResetState();

	if (m_pCullNone)
	m_pCullNone->ResetState();*/
}

bool CRenderManager::Initialize()
{
	if (!GET_SINGLE(CShaderManager)->Initialize())
		return false;

	AddRenderTargetBlendDesc(ALPHA_BLEND);
	CBlendState*	pBlendState = CreateBlendState(ALPHA_BLEND);
	SAFE_RELEASE(pBlendState);
	
	AddRenderTargetBlendDesc(ALPHA_BLEND_DISABLE, FALSE);
	pBlendState = CreateBlendState(ALPHA_BLEND_DISABLE);
	SAFE_RELEASE(pBlendState);

	CDepthStencilState*	pDepthStencilState = CreateDepthStencilState(DEPTH_DISABLE, FALSE);

	SAFE_RELEASE(pDepthStencilState);

	pDepthStencilState = CreateDepthStencilState(DEPTH_ENABLE, TRUE);

	SAFE_RELEASE(pDepthStencilState);

	CRasterizerState* pRasterizer = CreateRasterizerState(CULLNONE, D3D11_FILL_SOLID, D3D11_CULL_NONE);

	SAFE_RELEASE(pRasterizer);

	pRasterizer = CreateRasterizerState(CULLBACK, D3D11_FILL_SOLID, D3D11_CULL_BACK);

	SAFE_RELEASE(pRasterizer);

	pRasterizer = CreateRasterizerState(WIREFRAME, D3D11_FILL_WIREFRAME);

	SAFE_RELEASE(pRasterizer);

	return true;
}

CBlendState * CRenderManager::CreateBlendState(const string & strKey, BOOL bAlphaCoverage, BOOL bIndependent)
{
	CBlendState*	pBlendState = (CBlendState*)FindRenderState(strKey);
	if (!pBlendState)
	{
		assert(false);
		return NULL;
	}
	if (!pBlendState->CreateBlendState(bAlphaCoverage, bIndependent))
	{
		pBlendState->Release();
		SAFE_RELEASE(pBlendState);
		m_mapRenderState.erase(strKey);
		assert(false);
		return NULL;
	}
	return pBlendState;
}

bool CRenderManager::AddRenderTargetBlendDesc(const string & strKey, BOOL bBlendEnable, D3D11_BLEND eSrc, D3D11_BLEND eDest, D3D11_BLEND_OP eOption, D3D11_BLEND eSrcAlpha, D3D11_BLEND eDestAlpha, D3D11_BLEND_OP eOptionAlpha, UINT iWriteMask)
{
	CBlendState*	pBlendState = (CBlendState*)FindRenderState(strKey);

	if (!pBlendState)
	{
		pBlendState = new CBlendState;
		pBlendState->AddRef();

		m_mapRenderState.insert(make_pair(strKey, pBlendState));
	}

	pBlendState->AddRenderTargetBlendDesc(bBlendEnable, eSrc, eDest, eOption,
		eSrcAlpha, eDestAlpha, eOptionAlpha, iWriteMask);

	SAFE_RELEASE(pBlendState);

	return true;
}

CDepthStencilState * CRenderManager::CreateDepthStencilState(const string & strKey, BOOL bEnable, D3D11_DEPTH_WRITE_MASK eDepthMask, D3D11_COMPARISON_FUNC eDepthFunc, BOOL bStencilEnable, UINT8 iStencilReadMask, UINT8 iStencilWriteMask, const D3D11_DEPTH_STENCILOP_DESC & tFrontFace, const D3D11_DEPTH_STENCILOP_DESC & tBackFace)
{
	CDepthStencilState*	pDepthStencil = (CDepthStencilState*)FindRenderState(strKey);

	if (pDepthStencil)
		return pDepthStencil;

	pDepthStencil = new CDepthStencilState;

	if (!pDepthStencil->CreateDepthStencilState(bEnable, eDepthMask, eDepthFunc,
		bStencilEnable, iStencilReadMask, iStencilWriteMask, tFrontFace,
		tBackFace))
	{
		SAFE_RELEASE(pDepthStencil);
		assert(false);
		return NULL;
	}

	pDepthStencil->AddRef();

	m_mapRenderState.insert(make_pair(strKey, pDepthStencil));

	return pDepthStencil;
}

CRasterizerState * CRenderManager::CreateRasterizerState(const string & strKey, D3D11_FILL_MODE eFillMode, D3D11_CULL_MODE eCullMode)
{
	CRasterizerState* pRasterizer = (CRasterizerState*)FindRenderState(strKey);

	if (pRasterizer)
		return pRasterizer;

	pRasterizer = new CRasterizerState;

	if (!pRasterizer->CreateRasterizerState(eFillMode, eCullMode))
	{
		SAFE_RELEASE(pRasterizer);
		assert(false);
		
		return NULL;
	}

	pRasterizer->AddRef();

	m_mapRenderState.insert(make_pair(strKey, pRasterizer));

	return pRasterizer;
}

CRenderState * CRenderManager::FindRenderState(const string & strKey)
{
	unordered_map<string, CRenderState*>::iterator	iter = m_mapRenderState.find(strKey);

	if (iter == m_mapRenderState.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}



