#include "RectCollider2D.h"
#include "../ShaderManager.h"
#include "../ResourcesManager.h"
#include "../Component\Camera.h"
#include "../Scene.h"
#include "../Transform.h"
#include "../Mesh.h"
#include "../CollisionVoxel2D.h"
#include "../GameObject.h"
#include "../CollisionManager.h"
#include "../Shader.h"
#include "CircleCollider2D.h"

LMNT_USING

CRectCollider2D::CRectCollider2D()
{
	SetTag("RectCollider2D");
	m_eColliderType = CT_RECT;

#ifdef _DEBUG
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh(ENGINE_MESH_FRAME_RECT_COLOR);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout(POS_COLOR_LAYOUT);
#endif // _DEBUG
}
CRectCollider2D::CRectCollider2D(const CRectCollider2D & component):
	CCollider(component)
{
	*this = component;
}
CRectCollider2D::~CRectCollider2D()
{

}



int CRectCollider2D::LateUpdate(float fTime)
{
	return 0;
}

void CRectCollider2D::Render(float fDeltaTime)
{
#ifdef _DEBUG
	TRANSFORMCBUFFER	tBuffer = {};
	
	//영벡터
	Vector3 vMin;
	//메쉬의 버텍스는 길이가 1이므로, scale이 max가 된다.
	Vector3 vMax = m_vScale;
	Vector3 vLength = vMax - vMin;
	Vector3 vRotation = m_pTransform->GetWorldRotation();

	// 피봇을 적용한 위치를 구해준다.
	Vector3	vPos = vMin + vLength * m_vPivot;
	Vector3 vLocalTransformPivotLength = (m_pTransform->GetWorldPosition() - m_pTransform->GetPivot() * m_pTransform->GetWorldScale());
	Vector3 vLocalColliderPivotLength = m_vPivot * vLength;

	vPos = m_pTransform->GetWorldPosition()  ;
	

	//m_vPoint[0] = vPos + Vector3( m_pTransform->right().x * m_vScale.x, m_pTransform->up());


	//z축의 길이가 0이면 출력 할 수 없으므로, 1로 변경해준다.
	if (m_vScale.z == 0.f)
		m_vScale.z = 1.f;

	Matrix	matScale, matPos, matWorld;
	matScale = Matrix::Scaling(m_vScale);
	matPos = Matrix::Translation(vPos);
	matWorld = matScale * m_pTransform->GetWorldRotationMatrix() * matPos;

	CCamera*	pCamera = m_pScene->GetMainCamera();

	tBuffer.matWorld = matWorld;
	tBuffer.matView = pCamera->GetViewMatrix();
	tBuffer.matProjection = pCamera->GetProjectionMatrix();
	tBuffer.matWV = matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProjection;
	tBuffer.matVP = tBuffer.matView * tBuffer.matProjection;
	tBuffer.matWP = tBuffer.matWorld * tBuffer.matProjection;
	tBuffer.vPivot = m_vPivot;
	tBuffer.vLength = m_pMesh->GetLength();

	SAFE_RELEASE(pCamera);

	tBuffer.matWorld = tBuffer.matWorld.Transpose();
	tBuffer.matView = tBuffer.matView.Transpose();
	tBuffer.matProjection = tBuffer.matProjection.Transpose();
	tBuffer.matWV = tBuffer.matWV.Transpose();
	tBuffer.matWVP = tBuffer.matWVP.Transpose();
	tBuffer.matVP = tBuffer.matVP.Transpose();
	tBuffer.matWP = tBuffer.matWP.Transpose();

	// Transform 정보를 Shader 상수버퍼로 넘겨준다.
	GET_SINGLE(CShaderManager)->UpdateConstantBuffer(TRANSFORM_CBUFFER, &tBuffer, CBT_VERTEX | CBT_PIXEL | CBT_GEOMETRY);
	COLLIDERCBUFFER tColliderBuffer = {};

	tColliderBuffer.bUI = 0;
	tColliderBuffer.vColor = Vector4::green;

	GET_SINGLE(CShaderManager)->UpdateConstantBuffer(COLLIDER_CBUFFER, &tColliderBuffer, CBT_VERTEX);
	CCollider::Render(fDeltaTime);

#endif // _DEBUG

}

CRectCollider2D * CRectCollider2D::Clone() const
{
	return new CRectCollider2D(*this);
}

// 충돌체가 포함되어져 있는 모튼 복셀을 추가 해준다.
void CRectCollider2D::SetCollisionVoxel()
{
	list<CCollisionVoxel2D*>::iterator iter;
	list<CCollisionVoxel2D*>::iterator iterEnd = m_CollisionVoxel2DList.end();
	for(iter = m_CollisionVoxel2DList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->Contain((CCollider*)this))
		{
			//해당 복셀이 가지고 있는 충돌체 중 이 충돌체를 날려준다.
			(*iter)->Remove((CCollider*)this);
		}
	}

	m_CollisionVoxel2DList.clear();

	Vector2 VoxelSize = GET_SINGLE(CCollisionManager)->GetVoxelSize2D();
	Vector3 LT,RB;
	LT = m_pTransform->GetWorldPosition() - m_vPivot * m_vScale;
	RB = m_pTransform->GetWorldPosition() + (Vector3::One - m_vPivot) * m_vScale;
	
	Vector2  VoxelLT = Vector2((int)LT.x / (int)VoxelSize.x, (int)LT.y / (int)VoxelSize.y);
	Vector2  VoxelRB = Vector2((int)RB.x / (int)VoxelSize.x, (int)RB.y / (int)VoxelSize.y);

	//m_CollisionVoxel2DList.clear();

	if (VoxelLT == VoxelRB)
	{
		CCollisionVoxel2D* pVoxel = GET_SINGLE(CCollisionManager)->GetVoxel2D(Vector2(VoxelLT.x, VoxelLT.y));
		pVoxel->AddGameObject(m_pGameObject);
		//pVoxel->AddGameObject((CCollider*)pVoxel);
		m_CollisionVoxel2DList.push_back(pVoxel);
	}
	else
	{
		//중복 될 경우 자동으로 걸러진다.
		for (int y = VoxelLT.y; y <= VoxelRB.y; ++y)
		{
			for (int x = VoxelLT.x; x <= VoxelRB.x; ++x)
			{
				CCollisionVoxel2D* pVoxel = GET_SINGLE(CCollisionManager)->GetVoxel2D(Vector2(x, y));
				pVoxel->AddGameObject(m_pGameObject);
				m_CollisionVoxel2DList.push_back(pVoxel);
			}
		}
	}
	//list<CCollisionVoxel2D*>::iterator iter;
	//list<CCollisionVoxel2D*>::iterator iterEnd = m_CollisionVoxel2DList.end();
	//
	//for (iter = m_CollisionVoxel2DList.begin(); iter != iterEnd; ++iter)
	//{
	//	list<CCollisionVoxel2D*>::iterator tempIter;
	//	list<CCollisionVoxel2D*>::iterator tempIterEnd = tempContainer.end();
	//
	//	for (tempIter = tempContainer.begin(); tempIter != tempIterEnd; ++tempIter)
	//	{
	//		bool PrevContained = (*iter)->Contain(m_pGameObject);
	//		bool CurrentContained = (*tempIter)->Contain(m_pGameObject);
	//
	//		//이전에도 포함되어 있었고, 현재도 포함 되어 있으면
	//		if (PrevContained && CurrentContained)
	//			continue;
	//		else if (PrevContained && !CurrentContained)
	//		{
	//			//불가
	//		}
	//		else if (!PrevContained && CurrentContained)
	//		{
	//			if ((*iter)->GetContainedColliderCount() == 1)
	//			{
	//				list<CCollider*>* colliders = (*iter)->GetContainedColliders();
	//				
	//				CGameObject* pTargetObject = (*colliders->begin())->GetGameObject();
	//				if (pTargetObject == m_pGameObject)
	//				{
	//					m_CollisionVoxel2DList.erase(iter);
	//					GET_SINGLE(CCollisionManager)->RemoveVoxel2D(*iter);
	//				}
	//				SAFE_RELEASE(pTargetObject);
	//			}
	//		}
	//		else if (!PrevContained && !CurrentContained)
	//		{
	//			//불가
	//		}
	//	}
	//}

	

}

bool CRectCollider2D::VoxelCollisionCheck(const Vector2 & LT, const Vector2 & RB)
{
	float c[3][3];
	float absC[3][3];
	float d[3];

	float r0, r1, r;

	const float cutoff = 0.999999f;
	bool existsParallelPair = false;

	Vector3 dif = m_vPivot - Vector3(0.0f, 0.0f, 0);
	Vector3 vPosition = m_pTransform->GetWorldPosition();
	Vector3 vAxis[3] = { m_pTransform->GetWorldAxis(AX_X), m_pTransform->GetWorldAxis(AX_Y), m_pTransform->GetWorldAxis(AX_Z) };
	Vector3 vRotation = m_pTransform->GetWorldRotation();
	Vector3 vScale = m_vScale * 0.5f;
	Vector3 vTargetPosition = Vector3(LT.x, LT.y, 0);
	Vector3 vTargetAxis[3] = { Vector3::Axis[AX_X] , Vector3::Axis[AX_Y] , Vector3::Axis[AX_Z] };
	Vector3 vTargetRotation ;
	Vector3 vTargetScale = Vector3(GET_SINGLE(CCollisionManager)->GetVoxelSize2D().x, GET_SINGLE(CCollisionManager)->GetVoxelSize2D().y, 1) * 0.5f;

	//=======================투영==========================
	//X
	for (int i = 0; i < 3; ++i)
	{
		c[0][i] = vAxis[i].Dot(vTargetAxis[i]);
		absC[0][i] = abs(c[0][i]);

		if (absC[0][i] > cutoff)
			existsParallelPair = true;
	}
	d[0] = dif.Dot(vAxis[0]);
	r = abs(d[0]);
	r0 = vScale.x;
	r1 = vTargetScale.x * absC[0][0] + vTargetScale.y * absC[0][1] + vTargetScale.z * absC[0][2];

	if (r > r0 + r1)
		return false;

	//Y
	for (int i = 0; i < 3; ++i)
	{
		c[1][i] = vAxis[1].Dot(vTargetAxis[i]);
		absC[1][i] = abs(c[1][i]);
		if (absC[1][i] > cutoff)
			existsParallelPair = true;
	}

	d[1] = dif.Dot(vAxis[1]);
	r = abs(d[1]);
	r0 = vScale.y;
	r1 = vTargetScale.x * absC[1][0] + vTargetScale.y * absC[1][1] + vTargetScale.z * absC[1][2];

	if (r > r0 + r1)
		return false;

	//Z
	for (int i = 0; i < 3; ++i)
	{
		c[2][i] = vAxis[2].Dot(vTargetAxis[i]);
		absC[2][i] = abs(c[2][i]);
		if (absC[2][i] > cutoff)
			existsParallelPair = true;
	}

	d[2] = dif.Dot(vAxis[2]);
	r = abs(d[2]);
	r0 = vScale.z;
	r1 = vTargetScale.x * absC[2][0] + vTargetScale.y * absC[2][1] + vTargetScale.z * absC[2][2];

	if (r > r0 + r1)
		return false;

	//=============================================================

	// TargetX
	r = abs(dif.Dot(vTargetAxis[0]));
	r0 = vScale.x * absC[0][0] + vScale.y * absC[1][0] + vScale.z * absC[2][0];
	r1 = vTargetScale.x;

	if (r > r0 + r1)
		return false;

	//TargetY
	r = abs(dif.Dot(vTargetAxis[1]));
	r0 = vScale.x * absC[0][1] + vScale.y * absC[1][1] + vScale.z * absC[2][1];
	r1 = vTargetScale.y;

	if (r > r0 + r1)
		return false;

	//TargetZ
	r = abs(dif.Dot(vTargetAxis[2]));
	r0 = vScale.x * absC[0][2] + vScale.y * absC[1][2] + vScale.z * absC[2][2];
	r1 = vTargetScale.z;
	if (r > r0 + r1)
		return false;

	if (existsParallelPair == true)
		return true;

	//===========================================================
	//X * TargetX
	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = vScale.y * absC[2][0] + vScale.z * absC[1][0];
	r1 = vTargetScale.y * absC[0][2] + vTargetScale.z * absC[0][1];

	if (r > r0 + r1)
		return false;

	//X * TargetY
	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = vScale.y * absC[2][1] + vScale.z * absC[1][1];
	r1 = vTargetScale.x * absC[0][2] + vTargetScale.z * absC[0][0];

	if (r > r0 + r1)
		return false;

	//X * TargetZ	
	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = vScale.y * absC[2][2] + vScale.z * absC[1][2];
	r1 = vTargetScale.x * absC[0][1] + vTargetScale.y * absC[0][0];
	if (r > r0 + r1)
		return false;

	//Y * TargetX
	r = abs(d[2] * c[2][0] - d[2] * c[0][0]);
	r0 = vScale.x * absC[2][0] + vScale.z * absC[0][0];
	r1 = vTargetScale.y * absC[1][2] + vTargetScale.z * absC[1][1];
	if (r > r0 + r1)
		return false;

	//Y * TargetY
	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = vScale.x * absC[2][1] + vScale.z * absC[0][1];
	r1 = vTargetScale.x * absC[1][2] + vTargetScale.z * absC[1][0];
	if (r > r0 + r1)
		return false;

	//Y * TargetZ
	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = vScale.x * absC[2][2] + vScale.z * absC[0][2];
	r1 = vTargetScale.x * absC[1][1] + vTargetScale.y * absC[1][0];
	if (r > r0 + r1)
		return false;

	//Z * TargetX
	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = vScale.x * absC[1][0] + vScale.y * absC[0][0];
	r1 = vTargetScale.y * absC[2][2] + vTargetScale.z * absC[2][1];
	if (r > r0 + r1)
		return false;

	//Z * TargetY
	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = vScale.x * absC[1][1] + vScale.y * absC[0][1];
	r1 = vTargetScale.x * absC[2][2] + vTargetScale.z * absC[2][0];
	if (r > r0 + r1)
		return false;

	//Z * TargetZ
	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = vScale.x * absC[1][2] + vScale.y * absC[0][2];
	r1 = vTargetScale.x * absC[2][1] + vTargetScale.y * absC[2][0];
	if (r > r0 + r1)
		return false;

	return true;


	/*Vector2 vSize = RB - LT;
	Vector2 vPosition = m_pTransform->GetWorldPosition();
	Vector2 vTargetPosition = Vector2(LT.x * GET_SINGLE(CCollisionManager)->GetVoxelSize2D().x, LT.y * GET_SINGLE(CCollisionManager)->GetVoxelSize2D().y);
	Vector2 v = vPosition - vTargetPosition;

	Vector2 vLocalScale = m_vScale * 0.5f;
	Vector2 vTargetScale = GET_SINGLE(CCollisionManager)->GetVoxelSize2D() * 0.5f;

	Vector2 vUp = Vector2(m_pTransform->up().x, m_pTransform->up().y);
	Vector2 vRight = Vector2(m_pTransform->right().x, m_pTransform->right().y);
	Vector2 vTargetUp = Vector2(0.0f, 1.0f);
	Vector2 vTargetRight = Vector2(1.0f, 0.0f);

	{
		Vector2 vAxis = vUp;

		float fDistance = abs(v.Dot(vAxis));

		if (fDistance > abs(vAxis.Dot(vUp * vLocalScale.y)) +
			abs(vAxis.Dot(vRight * vLocalScale.x)) +
			abs(vAxis.Dot(vTargetUp * vTargetScale.y)) +
			abs(vAxis.Dot(vTargetRight * vTargetScale.x)))
		{
			return false;
		}
	}
	{
		Vector2 vAxis = vTargetUp;

		float fDistance = abs(v.Dot(vAxis));

		if (fDistance > abs(vAxis.Dot(vUp * vLocalScale.y)) +
			abs(vAxis.Dot(vRight * vLocalScale.x)) +
			abs(vAxis.Dot(vTargetUp * vTargetScale.y)) +
			abs(vAxis.Dot(vTargetRight * vTargetScale.x)))
		{
			return false;
		}
	}
	{
		Vector2 vAxis = vRight;

		float fDistance = abs(v.Dot(vAxis));

		if (fDistance > abs(vAxis.Dot(vUp * vLocalScale.y)) +
			abs(vAxis.Dot(vRight * vLocalScale.x)) +
			abs(vAxis.Dot(vTargetUp * vTargetScale.y)) +
			abs(vAxis.Dot(vTargetRight * vTargetScale.x)))
		{
			return false;
		}
	}
	{
		Vector2 vAxis = vTargetRight;

		float fDistance = abs(v.Dot(vAxis));

		if (fDistance > abs(vAxis.Dot(vUp * vLocalScale.y)) +
			abs(vAxis.Dot(vRight * vLocalScale.x)) +
			abs(vAxis.Dot(vTargetUp * vTargetScale.y)) +
			abs(vAxis.Dot(vTargetRight * vTargetScale.x)))
		{
			return false;
		}
	}

	return true;*/


}

bool CRectCollider2D::CollisionCheckWithRectCollider(CCollider * pTarget)
{
	CTransform* pTargetTransform = pTarget->GetTransform();

	Vector2 vPosition = m_pTransform->GetWorldPosition();
	Vector2 vTargetPosition = pTargetTransform->GetWorldPosition();
	Vector2 v = vPosition - vTargetPosition;
	
	Vector2 vLocalScale = m_vScale * 0.5f;
	Vector2 vTargetScale = pTarget->GetScale() * 0.5f;

	Vector2 vUp = Vector2(m_pTransform->up().x, m_pTransform->up().y);
	Vector2 vRight = Vector2(m_pTransform->right().x, m_pTransform->right().y);
	Vector2 vTargetUp = Vector2(pTargetTransform->up().x, pTargetTransform->up().y);
	Vector2 vTargetRight = Vector2(pTargetTransform->right().x, pTargetTransform->right().y);

	SAFE_RELEASE(pTargetTransform);
	//Local Y
	{
		Vector2 vAxis = vUp;

		float fDistance = abs(v.Dot(vAxis));

		if (fDistance > abs(vAxis.Dot(vUp * vLocalScale.y)) +
			abs(vAxis.Dot(vRight * vLocalScale.x)) +
			abs(vAxis.Dot(vTargetUp * vTargetScale.y)) +
			abs(vAxis.Dot(vTargetRight * vTargetScale.x)))
		{
			return false;
		}
	}
	{
		Vector2 vAxis = vTargetUp;

		float fDistance = abs(v.Dot(vAxis));

		if (fDistance > abs(vAxis.Dot(vUp * vLocalScale.y)) +
			abs(vAxis.Dot(vRight * vLocalScale.x)) +
			abs(vAxis.Dot(vTargetUp * vTargetScale.y)) +
			abs(vAxis.Dot(vTargetRight * vTargetScale.x)))
		{
			return false;
		}
	}
	{
		Vector2 vAxis = vRight;

		float fDistance = abs(v.Dot(vAxis));

		if (fDistance > abs(vAxis.Dot(vUp * vLocalScale.y)) +
			abs(vAxis.Dot(vRight * vLocalScale.x)) +
			abs(vAxis.Dot(vTargetUp * vTargetScale.y)) +
			abs(vAxis.Dot(vTargetRight * vTargetScale.x)))
		{
			return false;
		}
	}
	{
		Vector2 vAxis = vTargetRight;

		float fDistance = abs(v.Dot(vAxis));

		if (fDistance > abs(vAxis.Dot(vUp * vLocalScale.y)) +
			abs(vAxis.Dot(vRight * vLocalScale.x)) +
			abs(vAxis.Dot(vTargetUp * vTargetScale.y)) +
			abs(vAxis.Dot(vTargetRight * vTargetScale.x)))
		{
			return false;
		}
	}	

	return true;	
}
bool CRectCollider2D::CollisionCheckWithRectCollider(CCollider * pTarget, bool bIsAABB)
{
	CTransform* pTargetTransform = pTarget->GetTransform();	
	Vector3 LT, RB, TargetLT, TargetRB;
	Vector3 position = m_pTransform->GetWorldPosition();
	Vector3 targetPosition = pTargetTransform->GetWorldPosition();
	SAFE_RELEASE(pTargetTransform);

	LT = position - m_vPivot * m_vScale;
	RB = position + (Vector3::One - m_vPivot) * m_vScale;
	TargetLT = targetPosition - pTarget->GetPivot() * pTarget->GetScale();
	TargetRB = targetPosition + (Vector3::One - pTarget->GetPivot()) * pTarget->GetScale();
	
	if (LT.x > TargetRB.x)
		return false;

	else if (LT.y > TargetRB.y)
		return false;

	else if (RB.x < TargetLT.x)
		return false;

	else if (RB.y < TargetLT.y)
		return false;

	return true;
}
bool CRectCollider2D::CollisionCheckWithCircleCollider(CCollider * pTarget)
{
	//원과 OBB 충돌은 OBB의 각도와 상관 없이 OBB 를 AABB로 돌려준 후, AABB - CIRCLE 계산 하면 된다.

	CTransform* pTargetTransform = pTarget->GetTransform();
	Vector2 vTargetPosition = Vector2(pTargetTransform->GetWorldPosition().x, pTargetTransform->GetWorldPosition().y);

	Vector2 vWorldDistance; // difference in world coord.
	Vector2 vLocalDistance; // difference in local coord.
	Vector2 vRotation;
	Vector2 vHalf;
	Vector2 vSign;  // for restoring intersection quadrant

	vWorldDistance.x = vTargetPosition.x - m_pTransform->GetWorldPosition().x;
	vWorldDistance.y = vTargetPosition.y - m_pTransform->GetWorldPosition().y;

	vRotation.x = cos(m_pTransform->GetWorldRotation().z);
	vRotation.y = sin(m_pTransform->GetWorldRotation().z);

	vLocalDistance.x = vWorldDistance.x * vRotation.x - vWorldDistance.y * vRotation.y;
	vLocalDistance.y = vWorldDistance.y * vRotation.x + vWorldDistance.x * vRotation.y;

	vSign.x = vLocalDistance.x < 0.0f ? -1.0f : 1.0f;
	vSign.y = vLocalDistance.y < 0.0f ? -1.0f : 1.0f;

	vLocalDistance.x = abs(vLocalDistance.x);
	vLocalDistance.y = abs(vLocalDistance.y);

	vHalf.x = m_pTransform->GetWorldScale().x / 2.0f;
	vHalf.y = m_pTransform->GetWorldScale().y / 2.0f;

	SAFE_RELEASE(pTargetTransform);
	// intersection AABB - circle
	if (vLocalDistance.x > vHalf.x + ((CCircleCollider2D*)pTarget)->GetRadius() ||
		vLocalDistance.y > vHalf.y + ((CCircleCollider2D*)pTarget)->GetRadius())
	{
		return false;
	}

	if (vLocalDistance.x <= vHalf.x)
	{
		return true;
	}
	if (vLocalDistance.y <= vHalf.y)
	{
		return true;
	}

	float CornerDistSquared =
		pow(vLocalDistance.x - vHalf.x, 2.0f) +
		pow(vLocalDistance.y - vHalf.y, 2.0f);

	if (CornerDistSquared <= ((CCircleCollider2D*)pTarget)->GetRadius() * ((CCircleCollider2D*)pTarget)->GetRadius())
	{
		return true;
	}
	else
	{
		return false;
	}
}
//OBB 3D**************
//
//float c[3][3];
//float absC[3][3];
//float d[3];
//
//float r0, r1, r;
//
//const float cutoff = 0.999999f;
//bool existsParallelPair = false;
//
//
//Vector3 dif = m_vPivot - pTarget->GetPivot();
//Vector3 vPosition = m_pTransform->GetWorldPosition();
//Vector3 vAxis[3] = { m_pTransform->GetWorldAxis(AX_X), m_pTransform->GetWorldAxis(AX_Y), m_pTransform->GetWorldAxis(AX_Z) };
//Vector3 vRotation = m_pTransform->GetWorldRotation();
//Vector3 vScale = m_vScale * 10;
//Vector3 vTargetPosition;
//Vector3 vTargetAxis[3];
//Vector3 vTargetRotation;
//Vector3 vTargetScale;
//
//CTransform* pTargetTransform = pTarget->GetTransform();
//
//vTargetPosition = pTargetTransform->GetWorldPosition();
//vTargetAxis[0] = pTargetTransform->GetWorldAxis(AX_X);
//vTargetAxis[1] = pTargetTransform->GetWorldAxis(AX_Y);
//vTargetAxis[2] = pTargetTransform->GetWorldAxis(AX_Z);
//vTargetRotation = pTargetTransform->GetWorldRotation();
//vTargetScale = pTarget->GetScale() * 10;
//
//SAFE_RELEASE(pTargetTransform);
//
//
////=======================투영==========================
////X
//for (int i = 0; i < 3; ++i)
//{
//	c[0][i] = vAxis[i].Dot(vTargetAxis[i]);
//	absC[0][i] = abs(c[0][i]);
//
//	if (absC[0][i] > cutoff)
//		existsParallelPair = true;
//}
//d[0] = dif.Dot(vAxis[0]);
//r = abs(d[0]);
//r0 = vScale.x;
//r1 = vTargetScale.x * absC[0][0] + vTargetScale.y * absC[0][1] + vTargetScale.z * absC[0][2];
//
//if (r > r0 + r1)
//return false;
//
////Y
//for (int i = 0; i < 3; ++i)
//{
//	c[1][i] = vAxis[1].Dot(vTargetAxis[i]);
//	absC[1][i] = abs(c[1][i]);
//	if (absC[1][i] > cutoff)
//		existsParallelPair = true;
//}
//
//d[1] = dif.Dot(vAxis[1]);
//r = abs(d[1]);
//r0 = vScale.y;
//r1 = vTargetScale.x * absC[1][0] + vTargetScale.y * absC[1][1] + vTargetScale.z * absC[1][2];
//
//if (r > r0 + r1)
//return false;
//
////Z
//for (int i = 0; i < 3; ++i)
//{
//	c[2][i] = vAxis[2].Dot(vTargetAxis[i]);
//	absC[2][i] = abs(c[2][i]);
//	if (absC[2][i] > cutoff)
//		existsParallelPair = true;
//}
//
//d[2] = dif.Dot(vAxis[2]);
//r = abs(d[2]);
//r0 = vScale.z;
//r1 = vTargetScale.x * absC[2][0] + vTargetScale.y * absC[2][1] + vTargetScale.z * absC[2][2];
//
//if (r > r0 + r1)
//return false;
//
////=============================================================
//
//// TargetX
//r = abs(dif.Dot(vTargetAxis[0]));
//r0 = vScale.x * absC[0][0] + vScale.y * absC[1][0] + vScale.z * absC[2][0];
//r1 = vTargetScale.x;
//
//if (r > r0 + r1)
//return false;
//
////TargetY
//r = abs(dif.Dot(vTargetAxis[1]));
//r0 = vScale.x * absC[0][1] + vScale.y * absC[1][1] + vScale.z * absC[2][1];
//r1 = vTargetScale.y;
//
//if (r > r0 + r1)
//return false;
//
////TargetZ
//r = abs(dif.Dot(vTargetAxis[2]));
//r0 = vScale.x * absC[0][2] + vScale.y * absC[1][2] + vScale.z * absC[2][2];
//r1 = vTargetScale.z;
//if (r > r0 + r1)
//return false;
//
//if (existsParallelPair == true)
//return true;
//
////===========================================================
////X * TargetX
//r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
//r0 = vScale.y * absC[2][0] + vScale.z * absC[1][0];
//r1 = vTargetScale.y * absC[0][2] + vTargetScale.z * absC[0][1];
//
//if (r > r0 + r1)
//return false;
//
////X * TargetY
//r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
//r0 = vScale.y * absC[2][1] + vScale.z * absC[1][1];
//r1 = vTargetScale.x * absC[0][2] + vTargetScale.z * absC[0][0];
//
//if (r > r0 + r1)
//return false;
//
////X * TargetZ	
//r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
//r0 = vScale.y * absC[2][2] + vScale.z * absC[1][2];
//r1 = vTargetScale.x * absC[0][1] + vTargetScale.y * absC[0][0];
//if (r > r0 + r1)
//return false;
//
////Y * TargetX
//r = abs(d[2] * c[2][0] - d[2] * c[0][0]);
//r0 = vScale.x * absC[2][0] + vScale.z * absC[0][0];
//r1 = vTargetScale.y * absC[1][2] + vTargetScale.z * absC[1][1];
//if (r > r0 + r1)
//return false;
//
////Y * TargetY
//r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
//r0 = vScale.x * absC[2][1] + vScale.z * absC[0][1];
//r1 = vTargetScale.x * absC[1][2] + vTargetScale.z * absC[1][0];
//if (r > r0 + r1)
//return false;
//
////Y * TargetZ
//r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
//r0 = vScale.x * absC[2][2] + vScale.z * absC[0][2];
//r1 = vTargetScale.x * absC[1][1] + vTargetScale.y * absC[1][0];
//if (r > r0 + r1)
//return false;
//
////Z * TargetX
//r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
//r0 = vScale.x * absC[1][0] + vScale.y * absC[0][0];
//r1 = vTargetScale.y * absC[2][2] + vTargetScale.z * absC[2][1];
//if (r > r0 + r1)
//return false;
//
////Z * TargetY
//r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
//r0 = vScale.x * absC[1][1] + vScale.y * absC[0][1];
//r1 = vTargetScale.x * absC[2][2] + vTargetScale.z * absC[2][0];
//if (r > r0 + r1)
//return false;
//
////Z * TargetZ
//r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
//r0 = vScale.x * absC[1][2] + vScale.y * absC[0][2];
//r1 = vTargetScale.x * absC[2][1] + vTargetScale.y * absC[2][0];
//if (r > r0 + r1)
//return false;
//
//return true;