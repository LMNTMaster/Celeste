#pragma once

#include "Engine.h"

LMNT_BEGIN

class LMNT_DLL CRenderManager
{
	DECLARE_SINGLE(CRenderManager)

private:
	unordered_map<string, class CRenderState*>	m_mapRenderState;
	RENDER_MODE		m_eRenderMode;
	class CRenderState*		m_pDepthDisable;
	class CRenderState*		m_pCullNone;

public:
	RENDER_MODE GetRenderMode() const;
	void SetRenderMode(RENDER_MODE eMode);
	void SetDepth();
	void ResetDepth();
	bool Initialize();

	class CBlendState* CreateBlendState(const string& strKey, BOOL bAlphaCoverage = FALSE, BOOL bIndependent = FALSE);
	bool AddRenderTargetBlendDesc(const string& strKey, BOOL bBlendEnable = TRUE,
		D3D11_BLEND eSrc = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND eDest = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP eOption = D3D11_BLEND_OP_ADD,
		D3D11_BLEND eSrcAlpha = D3D11_BLEND_ONE,
		D3D11_BLEND eDestAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP eOptionAlpha = D3D11_BLEND_OP_ADD,
		UINT iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);

	class CDepthStencilState* CreateDepthStencilState(const string& strKey, BOOL bEnable = TRUE,
		D3D11_DEPTH_WRITE_MASK eDepthMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC eDepthFunc = D3D11_COMPARISON_LESS,
		BOOL bStencilEnable = FALSE,
		UINT8 iStencilReadMask = 0xff,
		UINT8 iStencilWriteMask = 0xff,
		const D3D11_DEPTH_STENCILOP_DESC& tFrontFace = {},
		const D3D11_DEPTH_STENCILOP_DESC& tBackFace = {});


	class CRasterizerState* CreateRasterizerState(const string& strKey,
		D3D11_FILL_MODE eFillMode = D3D11_FILL_SOLID,
		D3D11_CULL_MODE eCullMode = D3D11_CULL_BACK);


	class CRenderState* FindRenderState(const string& strKey);

};

LMNT_END

