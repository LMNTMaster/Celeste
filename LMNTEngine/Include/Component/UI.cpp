#include "UI.h"


LMNT_USING

CUI::CUI()
{
	m_eComponentType = CT_UI;
}

CUI::CUI(const CUI & ui):
	CComponent(ui)
{
}

CUI::~CUI()
{
}

UI_TYPE CUI::GetUIType() const
{
	return m_eUIType;
}

bool CUI::Initialize()
{
	return false;
}

int CUI::Update(float fTime)
{
	return 0;
}

int CUI::LateUpdate(float fTime)
{
	return 0;
}

void CUI::Render(float fTime)
{
}

CUI * CUI::Clone() const
{
	return nullptr;
}
