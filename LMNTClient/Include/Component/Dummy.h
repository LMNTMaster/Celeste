#pragma once
#include "Component.h"
#include "Component/Renderer.h"
LMNT_USING

class CDummy :
	public CComponent
{
private:
	friend class CGameObject;
	CDummy(const CDummy& component);
	CDummy();
	~CDummy();


private:
	float fTimer;
	float fTimerMax;
	class CRenderer* m_pRenderer;

public:
	void SetRenderer(CRenderer* pRenderer);

public:
	bool Initialize() override;
	int Update(float fTime) override;
};

