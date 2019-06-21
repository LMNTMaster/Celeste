
#include "Engine.h"

LMNT_BEGIN

class LMNT_DLL CCollisionManager
{
	DECLARE_SINGLE(CCollisionManager)
	

	//RectCollider 에서 Collider에 Voxel 추가한 후,
	//Collider에서 충돌 검사
	//추후 레이어 간 충돌 행렬을 추가 할 수 있도록 하려면
	//이걸 Manager에서 관리해줄 필요가 있음. => CollisionManager는 모든 오브젝트에 접근이 가능해야함
	//따라서 CollisionManager는 현재 Scene에 존재하는 모든 레이어에 접근하여 Update 해주도록 함.

private:
	float m_fFixedTimeStep;
	float m_fCullingDistance;

public:
	float GetCullingDistance() const;
	void SetCullingDistance(float fDistance);
	float GetFixedTimeStep() const;
	void SetFixedTimeStep(float fTimeStep);



private:
	unordered_map<Vector2, class CCollisionVoxel2D*> m_mapCollisionVoxel2D;
	unordered_map<Vector3, class CCollisionVoxel3D*> m_mapCollisionVoxel3D;
	Vector2 m_vVoxelSize2D;
	Vector3 m_vVoxelSize3D;
	class CButtonCollider* m_pCoveredButtonCollider;

public:
	Vector2 GetVoxelSize2D() const;
	Vector3 GetVoxelSize3D() const;
	class CCollisionVoxel2D* GetVoxel2D(const Vector2& position) ;
	class CCollisionVoxel2D* GetVoxel2D(const Vector3& position) ;
	class CCollisionVoxel2D* GetVoxel2D(float x, float y) ;
	class CCollisionVoxel3D* GetVoxel3D(const Vector3& position);
	class CCollisionVoxel3D* GetVoxel3D(float x, float y, float z) ;

public:
	void SetVoxelSize2D(const Vector2& vSize);
	void SetVoxelSize2D(float fWidth, float fHeight);
	void SetVoxelSize3D(const Vector3& vSize);
	void SetVoxelSize3D(float x, float y, float z);

public:

	void RemoveVoxel2D(const Vector2& vKeyPosition);
	void RemoveVoxel2D(float fKeyX, float fKeyY);
	void RemoveVoxel2D(class CCollisionVoxel2D* pTarget);
	void RemoveVoxel3D(const Vector3& vKeyPosition);
	void RemoveVoxel3D(float fKeyX, float fKeyY, float fKeyZ);
	class CCollisionVoxel3D* CreateVoxel(const Vector3& vKeyPosition);
	class CCollisionVoxel2D* CreateVoxel(const Vector2& vKeyPosition);

	void ClearVoxel2D();
	void ClearVoxel3D();


public:
	bool Initialize();
	int UpdateCollision(float fTime);

};


LMNT_END