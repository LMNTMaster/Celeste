#pragma once
#include "Component.h"

LMNT_USING

class CCeleste :
	public CComponent
{
private:
	friend class CGameObject;
	friend class CGameData;

private:
	CCeleste(const CCeleste& component);
	CCeleste();
	~CCeleste();

private:
	class CCamera* m_pCamera;
	vector<Vector4> m_vecAreaData;
	class CPlayer* m_pPlayer;

public:
	void SetCamera(class CCamera* pCamera);
	void SetPlayer(class CPlayer* pPlayer);	

public:
	bool Initialize() override;
	int Update(float fTime) override;



};

