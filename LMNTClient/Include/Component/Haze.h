#pragma once

#include "Component/Renderer.h"

LMNT_USING

class CHaze :
	public CRenderer
{
private:
	friend class CGameObject;

public:
	CHaze();
	~CHaze();

private:
	float m_fAmplitude;
	float m_fFrequency;
	float m_fTimer;

public:
	bool Initialize() override;
	void Render(float fTime) override final;

};

