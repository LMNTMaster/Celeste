#include "CircleCollider2D.h"
#include "../Transform.h"
#include "../GameObject.h"
#include "../CollisionManager.h"
#include "../CollisionVoxel2D.h"
#include "../ResourcesManager.h"
#include "../ShaderManager.h"
#include "Camera.h"
#include "../Mesh.h"
#include "../Scene.h"
#include "../Layer.h"


LMNT_USING

CCircleCollider2D::CCircleCollider2D():
	m_fRadius(10.0f)
{
	SetTag("CircleCollider2D");
	m_eColliderType = CT_CIRCLE;

#ifdef _DEBUG
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(STANDARD_COLOR_SHADER);
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh(ENGINE_MESH_FRAME_CIRCLE_COLOR);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout(POS_COLOR_LAYOUT);
#endif // _DEBUG
}


CCircleCollider2D::CCircleCollider2D(const CCircleCollider2D & component) :
	CCollider(component)
{
	*this = component;
}

CCircleCollider2D::~CCircleCollider2D()
{
}

float CCircleCollider2D::GetRadius() const
{
	return m_fRadius;
}

void CCircleCollider2D::SetRadius(float fRadius)
{
}

void CCircleCollider2D::SetCollisionVoxel()
{
	list<CCollisionVoxel2D*>::iterator iter;
	list<CCollisionVoxel2D*>::iterator iterEnd = m_CollisionVoxel2DList.end();
	for (iter = m_CollisionVoxel2DList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->Contain((CCollider*)this))
		{
			//해당 복셀이 가지고 있는 충돌체 중 이 충돌체를 날려준다.
			(*iter)->Remove((CCollider*)this);
		}
	}

	m_CollisionVoxel2DList.clear();

	//각 버텍스에서 연결 되어진 버텍스 끼리 뺀 방향으로 돌면서 해당 복셀에 추가한다.
	float fScaleX = m_vScale.x / 2.0f;
	float fScaleY = m_vScale.y / 2.0f;

	Vector3 Vertex[4] = {};
	Vertex[0] = m_pTransform->GetWorldPosition() + m_pTransform->left() * m_fRadius + m_pTransform->up() * m_fRadius;
	Vertex[1] = m_pTransform->GetWorldPosition() + m_pTransform->right() * m_fRadius + m_pTransform->up() * m_fRadius;
	Vertex[2] = m_pTransform->GetWorldPosition() + m_pTransform->right() * m_fRadius + m_pTransform->down() * m_fRadius;
	Vertex[3] = m_pTransform->GetWorldPosition() + m_pTransform->left() * m_fRadius + m_pTransform->down() * m_fRadius;

	//인덱스는 Voxel의 크기에 따라 다르므로
	Vector2 VoxelSize = GET_SINGLE(CCollisionManager)->GetVoxelSize2D();

	//ABCD 의 AABB 상의 LT, RB 위치를 찾는다.
	Vector3 LT = Vertex[0];
	Vector3 RB = Vertex[0];

	for (int i = 0; i < 4; ++i)
	{
		if (LT.x < Vertex[i].x)
			LT.x = Vertex[i].x;

		if (LT.y < Vertex[i].y)
			LT.y = Vertex[i].y;

		if (RB.x > Vertex[i].x)
			RB.x = Vertex[i].x;

		if (RB.y > Vertex[i].y)
			RB.y = Vertex[i].y;
	}

	Vector2 VoxelRB = Vector2((int)LT.x / (int)VoxelSize.x, (int)LT.y / (int)VoxelSize.y);
	Vector2 VoxelLT = Vector2((int)RB.x / (int)VoxelSize.x, (int)RB.y / (int)VoxelSize.y);

	//m_CollisionVoxel2DList.clear();

	if (VoxelLT == VoxelRB)
	{
		CCollisionVoxel2D* pVoxel = GET_SINGLE(CCollisionManager)->GetVoxel2D(Vector2(VoxelLT.x, VoxelLT.y));
		pVoxel->AddGameObject(m_pGameObject);
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
}

bool CCircleCollider2D::VoxelCollisionCheck(const Vector2 & LT, const Vector2 & RB)
{
	return false;
}

bool CCircleCollider2D::CollisionCheckWithCircleCollider(CCollider * pTarget)
{
	CCircleCollider2D* pCastedTarget = (CCircleCollider2D*)pTarget;
	
	CTransform* pTargetTransform = pTarget->GetTransform();


	float fDistance = (pTargetTransform->GetWorldPosition() - m_pTransform->GetWorldPosition()).Length();

	if (fDistance > pCastedTarget->GetRadius() + m_fRadius)
	{
		SAFE_RELEASE(pTargetTransform);
		return false;
	}

	return true;
}

bool CCircleCollider2D::CollisionCheckWithRectCollider(CCollider * pTarget)
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

	vRotation.x = cos(pTargetTransform->GetWorldRotation().z);
	vRotation.y = sin(pTargetTransform->GetWorldRotation().z);

	vLocalDistance.x = vWorldDistance.x * vRotation.x - vWorldDistance.y * vRotation.y;
	vLocalDistance.y = vWorldDistance.y * vRotation.x + vWorldDistance.x * vRotation.y;

	vSign.x = vLocalDistance.x < 0.0f ? -1.0f : 1.0f;
	vSign.y = vLocalDistance.y < 0.0f ? -1.0f : 1.0f;

	vLocalDistance.x = abs(vLocalDistance.x);
	vLocalDistance.y = abs(vLocalDistance.y);

	vHalf.x = pTargetTransform->GetWorldScale().x / 2.0f;
	vHalf.y = pTargetTransform->GetWorldScale().y / 2.0f;

	SAFE_RELEASE(pTargetTransform);
	// intersection AABB - circle
	if (vLocalDistance.x > vHalf.x + m_fRadius ||
		vLocalDistance.y > vHalf.y + m_fRadius)
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

	float CornerDistSquared = pow(vLocalDistance.x - vHalf.x, 2.0f) + pow(vLocalDistance.y - vHalf.y, 2.0f);

	if (CornerDistSquared <= m_fRadius * m_fRadius)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

int CCircleCollider2D::LateUpdate(float fTime)
{
	return 0;
}

void CCircleCollider2D::Render(float fDeltaTime)
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

	vPos = m_pTransform->GetWorldPosition();


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
	tBuffer.vPivot = m_vPivot;
	tBuffer.vLength = m_pMesh->GetLength();

	SAFE_RELEASE(pCamera);

	tBuffer.matWorld = tBuffer.matWorld.Transpose();
	tBuffer.matView = tBuffer.matView.Transpose();
	tBuffer.matProjection = tBuffer.matProjection.Transpose();
	tBuffer.matWV = tBuffer.matWV.Transpose();
	tBuffer.matWVP = tBuffer.matWVP.Transpose();
	tBuffer.matVP = tBuffer.matVP.Transpose();

	// Transform 정보를 Shader 상수버퍼로 넘겨준다.
	GET_SINGLE(CShaderManager)->UpdateConstantBuffer(TRANSFORM_CBUFFER, &tBuffer, CBT_VERTEX | CBT_PIXEL | CBT_GEOMETRY);

	CCollider::Render(fDeltaTime);

#endif // _DEBUG

}

CCircleCollider2D * CCircleCollider2D::Clone() const
{
	return new CCircleCollider2D(*this);
}
