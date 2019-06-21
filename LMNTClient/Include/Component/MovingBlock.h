#pragma once
#include "Component.h"
#include "Component/Renderer.h"
#include "../Client.h"

LMNT_USING

class CMovingBlock :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CMovingBlock();
	~CMovingBlock();

private:
	CGameObject* m_pPlayer;
	CRenderer* m_pLight;
	MOVING_BLOCK_STATE m_eState;
	int m_iTypes;
	float m_fForce;
	float m_fAcceleration;
	Vector3 m_vBeginPosition;
	Vector3 m_vEndPosition;
	Vector3 m_vForce;
	float m_fBoostTimer;
	bool m_bBoostable;
	bool m_bEnter;
	bool m_bDashed;




public:
	const Vector3& GetForce() const;
	void SetForce(const Vector3& vForce);
	void Reset();
	void SetBlockType(int iTypes);
	void SetBeginPosition(const Vector3& position);
	void SetEndPosition(const Vector3& position);
	bool GetBoostable() const;
	
public:
	bool Initialize() override;
	int Update(float fTime) override;

	int OnCollisionEnter(class CCollider* pThis, class CCollider* pTarget, float fTime) override;
	int OnCollisionStay(class CCollider* pThis, class CCollider* pTarget, float fTime) override;
	int OnCollisionExit(class CCollider* pThis, class CCollider* pTarget, float fTime) override;
};

