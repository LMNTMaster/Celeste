#pragma once
#include "Component.h"

LMNT_USING

class CHair :
	public CComponent
{

private:
	friend class CGameObject;

public:
	CHair();
	~CHair();
	
private:
	bool m_bIsShadow;
	Vector4 m_vColor;
	class CGameObject* m_pBody;
	Vector3 m_vOffset;
	Vector3 m_vOriginScale;
	class CGameObject* m_pBackground;
	Vector3 m_vOutlineOriginScale;
	
public:
	void UseAsShadow();
	void SetBackground(class CGameObject* pBackground);
	void SetBody(class CGameObject* pBody);

public:
	bool Initialize() override;
	int LateUpdate(float fTime) override;
	void Render(float fTime) override;

};

