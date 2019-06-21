#pragma once
#include "Component.h"
#include "../Client.h"

LMNT_USING

class CCameraMove :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CCameraMove(const CCameraMove& component);
	CCameraMove();
	~CCameraMove();
	CCameraMove* Clone();
	
private:
	bool m_bLock;
	class CGameObject* m_pPlayer;
	Vector2 m_vScreenSize;
	vector<PAREA> m_vecArea;
	PAREA m_pPrevArea;
	PAREA m_pCurrentArea;
	float m_fMoveSpeed;

	bool m_bShake;
	float m_fShakeDuration;
	float m_fShakeTimer;
	float m_fShakeAmplitude;

	vector<class CGameObject*> m_vecBackground;

	static bool Sort(class CGameObject* pSrc, class CGameObject* pDest);

public:
	void Unlock();
	void Shake(float fDuration, float fAmplitude);
	void AddBackground(class CGameObject* pTarget);
	void AddBackground(class CScene* pScene);
	void SetPlayer(class CGameObject* pPlayer);
	void SetSpeed(float fSpeed);
	float GetSpeed() const;
	void SetAreaData(vector<PAREA>* vecArea);
	PAREA GetArea() const;

public:
	bool Initialize() override;
	int Update(float fTime) override;

};

