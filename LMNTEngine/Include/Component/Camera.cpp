#include "Camera.h"
#include "../Transform.h"

LMNT_USING


CCamera::CCamera()
{
	m_eComponentType = CT_CAMERA;
	SetTag("Camera");

	m_matView = new Matrix;
	m_matOrthoProjection = new Matrix;
	m_matPerspectiveProjection = new Matrix;

}

CCamera::CCamera(const CCamera & component)
{
	*this = component;

	m_matView = new Matrix;
	m_matOrthoProjection = new Matrix;
	m_matPerspectiveProjection = new Matrix;

	*m_matView = *component.m_matView;
	*m_matOrthoProjection = *component.m_matOrthoProjection;
	*m_matPerspectiveProjection = *component.m_matPerspectiveProjection;
}


CCamera::~CCamera()
{
	SAFE_DELETE(m_matView);
	SAFE_DELETE(m_matOrthoProjection);
	SAFE_DELETE(m_matPerspectiveProjection);
}

Matrix CCamera::GetPerspectiveProjectionMatrix() const
{
	return *m_matPerspectiveProjection;
}

Matrix CCamera::GetOrthographyProjectionMatrix() const
{
	return *m_matOrthoProjection;
}

Matrix CCamera::GetViewMatrix() const
{
	return *m_matView;
}

Matrix CCamera::GetProjectionMatrix() const
{
	return *m_matOrthoProjection;
}


void CCamera::SetPerspectiveProjection(float fAspect, float fViewAngle, float fNear, float fFar)
{
	*m_matPerspectiveProjection = XMMatrixPerspectiveFovLH(fViewAngle, fAspect, fNear, fFar);
}

void CCamera::SetOrthographyProjection(float fLeft, float fRight, float fTop, float fBottom, float fNear, float fFar)
{
	*m_matOrthoProjection = XMMatrixOrthographicOffCenterLH(fLeft, fRight, fBottom, fTop, fNear, fFar);
}

bool CCamera::Initialize()
{
	return true;
}

int CCamera::Input(float fTime)
{
	return 0;
}

int CCamera::Update(float fTime)
{
	return 0;
}

int CCamera::LateUpdate(float fTime)
{
	*m_matView = XMMatrixIdentity();

	for (int i = 0; i < 3; ++i)
	{
		Vector3	vAxis = m_pTransform->GetWorldAxis((AXIS)i);

		memcpy(&m_matView->fMat[i][0], &vAxis, sizeof(Vector3));
	}

	m_matView->Transpose();

	Vector3	vPos = m_pTransform->GetWorldPosition();

	for (int i = 0; i < 3; ++i)
	{
		Vector3	vAxis = m_pTransform->GetWorldAxis((AXIS)i);
		m_matView->fMat[3][i] = -vPos.Dot(vAxis);
	}

	return 0;
}
