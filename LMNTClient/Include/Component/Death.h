#pragma once
#include "Component.h"

LMNT_USING

class CDeath :
	public CComponent
{
private:
	friend class CGameObject;
	CDeath();
	~CDeath();

private:
	vector<class CGameObject*> m_vecBlob;
	float m_fTimer;

public:
	void AddBlob(class CGameObject* pTarget);

public:
	bool Initialize() override;
	int Update(float fTime) override;

};

