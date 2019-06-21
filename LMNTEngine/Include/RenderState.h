#pragma once
#include "Ref.h"

LMNT_BEGIN

class LMNT_DLL CRenderState :
	public CRef
{
protected:
	CRenderState();
	virtual ~CRenderState() = 0;

protected:
	//ID3D11DeviceChild�� BlendState, DepthStencilState, RasterizerState�� ����� �����ְ� �����Ƿ�, �̰� �ϳ��� ó����.
	ID3D11DeviceChild*		m_pState;
	ID3D11DeviceChild*		m_pPrevState;
	RENDER_STATE			m_eRenderStateType;

public:
	RENDER_STATE GetRenderStateType()	const;

public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;
};

LMNT_END