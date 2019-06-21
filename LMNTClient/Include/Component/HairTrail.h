#pragma once
#include "Component.h"

LMNT_USING

class CHairTrail :
	public CComponent
{
private:
	friend class CGameObject;
	CHairTrail();
	~CHairTrail();

public:
	bool Initialize() override;
	int Update(float fTime) override;
};

