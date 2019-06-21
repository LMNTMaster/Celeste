#pragma once
#include "../Component.h"

LMNT_BEGIN

class LMNT_DLL CAnimator :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CAnimator();
	~CAnimator();
	
private:
	class CAnimationController* m_pAnimationController;
	class CRenderer*	m_pRenderer;
	bool m_bAnimationStateTransitioned;
	bool m_bSyncScale;
	Vector3 m_vOriginScale;
	Vector3 m_vScaleFactor;
	Vector3 m_vLocalScaleFactor;


public:
	void SetTargetRenderer(class CRenderer* pRenderer);
	const Vector3& GetLocalScaleFactor() const;
	const Vector3& GetFixedScaleFactor() const;
	const Vector3& GetOriginScale() const;
	void SetFixedScaleFactor(const Vector3& vFactor);
	void SetLocalScaleFactor(const Vector3& vFactor);
	bool GetSyncScale() const;
	void SetSyncScale(bool bUseSyncScale);
	void SetAnimationStateTransitioned();
	void SetAnimationController(class CAnimationController* pController);

public:
	void SetEntryState(const string& strStateName);
	class CAnimationState* CreateAnimationStateInCurrentController(const string& strStateName);
	void CreateAnimationController();
	class CAnimationController* GetAnimationController() const;

public:
	bool Initialize() override;
	int Update(float fTime) override;

};

LMNT_END