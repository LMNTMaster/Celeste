#pragma once

#include "Engine.h"

LMNT_BEGIN

class LMNT_DLL CCollisionVoxel2D
{
private:
	friend class CCollisionManager;

private:
	CCollisionVoxel2D();
	~CCollisionVoxel2D();
	
private:
	Vector2 m_vKeyPosition;
	unordered_map<class CCollider*, class CCollider*> m_mapContainedCollider;

public:
	void SetKeyPosition(const Vector2& vKeyPosition);
	Vector2 GetKeyPosition() const;
	unordered_map<class CCollider*, class CCollider*>* GetContainedColliders();
	int	 GetContainedColliderCount() const;
	bool Contain(class CCollider* pTarget) const;
	bool Contain(class CGameObject* pTarget) const;
	void AddGameObject(class CGameObject* pTarget);
	void AddGameObject(class CCollider* pTarget);
	void Remove(class CGameObject* pTarget);
	void Remove(class CCollider* pTarget);
	void Clear();
};

LMNT_END