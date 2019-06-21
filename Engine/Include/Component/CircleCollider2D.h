#pragma once
#include "../Collider.h"


LMNT_BEGIN

class LMNT_DLL CCircleCollider2D :
	public CCollider
{
private:
	friend class CGameObject;

private:
	CCircleCollider2D();
	CCircleCollider2D(const CCircleCollider2D& component);
	~CCircleCollider2D();

private:
	float m_fRadius;

public:
	float GetRadius() const;
	void SetRadius(float fRadius);
	void SetCollisionVoxel() override;

public:

	bool VoxelCollisionCheck(const Vector2& LT, const Vector2& RB);
	bool CollisionCheckWithCircleCollider(CCollider* pTarget);
	bool CollisionCheckWithRectCollider(CCollider* pTarget);

public:
	int LateUpdate(float fTime) override;
	void Render(float fDeltaTime) override;
	CCircleCollider2D* Clone() const override;
};

LMNT_END