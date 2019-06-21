#pragma once
#include "Engine.h"

LMNT_BEGIN

class LMNT_DLL CCollisionVoxel3D
{
private:
	friend class CCollisionManager;

private:
	CCollisionVoxel3D();
	~CCollisionVoxel3D();

private:
	Vector3 m_vKeyPosition;
	unordered_map<class CCollider*, class CCollider*> m_mapContainedCollider;

public:
	void SetKeyPosition(const Vector3& vKeyPosition);
	Vector3 GetKeyPosition() const;
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