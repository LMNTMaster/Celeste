#pragma once
#include "../Component.h"

LMNT_BEGIN

class LMNT_DLL CCamera :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CCamera();
	CCamera(const CCamera& component);
	~CCamera();

private:
	PMatrix		m_matView;
	PMatrix		m_matPerspectiveProjection;
	PMatrix		m_matOrthoProjection;

public:
	Matrix GetPerspectiveProjectionMatrix() const;
	Matrix GetOrthographyProjectionMatrix() const;
	Matrix GetViewMatrix() const;
	Matrix GetProjectionMatrix() const;
	void SetPerspectiveProjection(float fAspect, float fViewAngle, float fNear = 0.3f, float fFar = 5000.f);
	void SetOrthographyProjection(float fLeft, float fRight, float fTop, float fBottom,	float fNear = 0.f, float fFar = 5000.f);


public:
	bool Initialize()						override;
	int Input(float fTime)				override;
	int Update(float fTime)			override;
	int LateUpdate(float fTime)		override;
	
};

LMNT_END	