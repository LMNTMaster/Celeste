#include "RenderState.h"

LMNT_USING

CRenderState::~CRenderState()
{
	SAFE_RELEASE(m_pState);
}

CRenderState::CRenderState():
	m_pState(NULL),
	m_pPrevState(NULL)
{
}

RENDER_STATE CRenderState::GetRenderStateType() const
{
	return m_eRenderStateType;
}
