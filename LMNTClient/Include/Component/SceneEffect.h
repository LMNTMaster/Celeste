#pragma once
#include "Component.h"
#include "../Client.h"

LMNT_USING

class CSceneEffect :
	public CComponent
{
private:
	friend class CGameObject;
	CSceneEffect();
	~CSceneEffect();

private:
	bool m_bOpen;
	bool m_bReset;
	float m_fTimer;
	float m_fTimeLimit;
	SCENE_AFTER_EFFECT m_eAfterEffect;

	class CGameObject* m_pLeft;
	class CGameObject* m_pTop;
	class CGameObject* m_pRight;
	class CGameObject* m_pBottom;

public:
	void SetOpen(bool bOpen);
	void SetAfterEffect(SCENE_AFTER_EFFECT eAfterEffect);

public:
	bool Initialize() override;
	int Update(float fTime) override;
};

