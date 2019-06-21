#pragma once
#include "Component.h"
LMNT_USING
class CPivot :
	public CComponent
{
public:
	CPivot();
	~CPivot();

private:
	Vector3 m_vPivotPosition;

public:
	bool Initialize() override;
	int Update(float fTime) override;
};

