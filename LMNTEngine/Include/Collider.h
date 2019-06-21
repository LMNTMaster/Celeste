#pragma once
#include "Component.h"

LMNT_BEGIN
typedef struct LMNT_DLL _tagCollisionArea
{
	Vector3 vMin;
	Vector3 vMax;
	Vector3 vSize;
	Vector3 vRenderMin;
	Vector3 vRenderMax;

	_tagCollisionArea() :
		vMin(),
		vMax(),
		vSize(),
		vRenderMin(),
		vRenderMax()
	{
	}
}COLLISIONAREA, *PCOLLISIONAREA;



class LMNT_DLL CCollider :
	public CComponent
{
private:
	friend class CCollisionManager;

public:
	CCollider();
	CCollider(const CCollider& component);
	virtual ~CCollider();


protected:
	COLLIDER_TYPE m_eColliderType;
	Vector3 m_vPivot;
	Vector3 m_vScale;
	COLLISIONAREA m_tCollisionArea;
	
	list<class CCollisionVoxel2D*> m_CollisionVoxel2DList;
	list<class CCollisionVoxel3D*> m_CollisionVoxel3DList;
	unordered_map<class CCollider*, class CCollider*> m_mapCollision;

#ifdef _DEBUG

	class CShader*			m_pShader;
	class CMesh*				m_pMesh;
	ID3D11InputLayout*	m_pLayout;

#endif // _DEBUG

public:
	COLLIDER_TYPE GetColliderType() const;
	Vector3 GetPivot() const;
	Vector3 GetScale() const;

public:
	void Straighten();
	void SetPivot(const Vector3& vPivot);
	void SetPivot(float x, float y, float z);
	void SetScale(const Vector3& vScale);
	void SetScale(float x, float y, float z);

	virtual void SetCollisionVoxel() = 0; // 충돌을 하려면 반드시 정의되어야 한다.
	void ClearCollisionVoxel();

	size_t GetCollisionCount() const;
	void AddCollisionList(CCollider* pTarget);
	void RemoveTargetFromCollisionList(CCollider* pTarget);
	bool ContainCollision(CCollider* pTarget) const;
	void ClearCollisionList();
	void AddCollisionVoxel(class CCollisionVoxel2D* pVoxel);
	void AddCollisionVoxel(class CCollisionVoxel3D* pVoxel);
	void AddCollisionVoxel(const Vector2& vVoxelKey);
	void AddCollisionVoxel(const Vector3& vVoxelKey);

	void CalculateCollision2D(float  fTime);
	void CalculateCollision3D(float  fTime);

	bool CollisionCheck(CCollider* pTarget) const;
	CCollider* GetColliderInBegin() const;

public:
	bool Initialize() override;
	int Update(float fTime) override;
	int LateUpdate(float fTime) override;
	int OnCollisionEnter(CCollider* pThis, CCollider* pTarget, float fTime) override;
	int OnCollisionStay(CCollider* pThis, CCollider* pTarget, float fTime) override;
	int OnCollisionExit(CCollider* pThis, CCollider* pTarget, float fTime) override;
	void Render(float fTime) override;
};

LMNT_END