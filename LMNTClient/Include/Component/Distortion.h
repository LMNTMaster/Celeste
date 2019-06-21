#pragma once
#include "Component.h"

LMNT_USING

class CDistortion :
	public CComponent
{
private:
	friend class CGameObject;
public:
	CDistortion();
	~CDistortion();


private:
	float m_fTargetTime;
	float m_fTimer;
	float m_fTargetScale;
	float m_fScale;

public:
	void SetTotalTime(float fTime);
	void SetTargetScale(float fScale);

public:
	bool Initialize() override;
	int Update(float fTime) override;

};

