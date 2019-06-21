#pragma once
#include "UI.h"

LMNT_BEGIN

class LMNT_DLL CUIButton :
	public CUI
{
private:
	friend class CGameObject;

private:
	CUIButton();
	CUIButton(const CUIButton& component);
	~CUIButton();

private:
	Vector4 m_vDiffuse;

public:
	bool Initialize() override;
	void OnMouseEnter(const Vector2& vPosition) override;
	void OnMouseStay(const Vector2& vPosition, float fTime) override;
	void OnMouseExit(const Vector2& vPosition) override;
};

LMNT_END	