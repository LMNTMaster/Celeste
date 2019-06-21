#pragma once

#include "../Collider.h"

LMNT_BEGIN

//AABB�� OBB�� ���Խ�ų �� �ֱ� ������, Rect���� �ϳ��� ����ϵ��� �Ѵ�.

class LMNT_DLL CRectCollider2D :
	public CCollider
{
private:
	friend class CGameObject;
	
private:
	CRectCollider2D();
	CRectCollider2D(const CRectCollider2D& component);
	~CRectCollider2D();

public:
	void SetCollisionVoxel() override;

public:

	bool VoxelCollisionCheck(const Vector2& LT, const Vector2& RB);
	bool CollisionCheckWithRectCollider(CCollider* pTarget);
	bool CollisionCheckWithRectCollider(CCollider* pTarget, bool bIsAABB);
	bool CollisionCheckWithCircleCollider(CCollider* pTarget);


public:
	int LateUpdate(float fTime) override;
	void Render(float fDeltaTime) override;
	CRectCollider2D* Clone() const override;
};

LMNT_END