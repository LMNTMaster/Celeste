#pragma once
#include "../Component.h"

LMNT_BEGIN

class LMNT_DLL CUI :
	public CComponent
{

private:
	friend class CGameObject;

protected:
	CUI();
	CUI(const CUI& ui);
	virtual ~CUI();

protected:
	UI_TYPE m_eUIType;


public:
	UI_TYPE GetUIType()	const;
	
public:
	bool Initialize() override;
	int Update(float fTime) override;
	int LateUpdate(float fTime) override;
	void Render(float fTime) override;
	CUI* Clone()	const override;



};

LMNT_END