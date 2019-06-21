#pragma once
#include "../Component.h"

LMNT_BEGIN

class LMNT_DLL CCursor :
	public CComponent
{
private:
	friend class CGameObject;
	
private:
	CCursor();
	~CCursor();

private:
	bool m_bVisible;

public:
	void SetCursorVisible(bool bVisible);
	bool GetCursorVisible() const;

public:
	bool Initialize() override;
	int LateUpdate(float fTime) override;
};

LMNT_END