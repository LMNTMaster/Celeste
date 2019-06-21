#pragma once
#include "Component.h"

LMNT_USING

class CGridRenderer :
	public CComponent
{
	friend class CGameObject;

public:
	CGridRenderer();
	~CGridRenderer();

private:
	bool m_bInitialized;
	bool m_bEnable;
	Vector4 m_vColor;
	int m_iGridSize;


private:

	void CreateGrid();


public:
	void SetGridSize(int iGridSize);
	void SetColor(const Vector4& vColor);
	void SetEnable();
	void SetDisable();


public:

	bool Initialize() override;
	int Update(float fTime) override;
};

