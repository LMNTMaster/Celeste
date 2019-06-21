#pragma once
#include "Component.h"

LMNT_USING

class CParticleManager :
	public CComponent
{
private:
	friend class CGameObject;
	
private:
	CParticleManager();
	CParticleManager(const CParticleManager& component);
	~CParticleManager();

private:
	vector<class CLayer*> m_vecTargetLayer;
	vector<Vector4> m_vecTargetColor;
	bool m_bUseSnow;
	float	m_fTimer;
	int		m_iLimitCount;
	int		m_iParticleCount;
	int		m_iSpawnCount;
	float	m_fSpawnInterval;

private:
	void CreateParticle(class CLayer* pTargetLayer);
	void CreateSnow();
	Vector4 GetRandomColor();

public:
	void SetSpawnLimit(int iCount);
	void ReduceCount();
	void UseSnow();
	void AddTargetLayer(class CLayer* pLayer);
	void AddTargetColor(const Vector4& vColor);

	void SetSpawnCount(int iCount);
	void SetSpawnInterval(float fInterval);
public:
	bool Initialize() override;
	int Update(float fTime) override;
};

