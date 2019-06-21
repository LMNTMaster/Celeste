#include "stdafx.h"
#include "GridRenderer.h"
#include "Component/Renderer.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Material.h"
#include "Component/Camera.h"
#include "Scene.h"

CGridRenderer::CGridRenderer():
	m_bInitialized(false),
	m_iGridSize(32)
{
}


CGridRenderer::~CGridRenderer()
{
}

void CGridRenderer::CreateGrid()
{
	CRenderer* pGridRenderer = m_pGameObject->AddComponent<CRenderer>("GridRenderer");
	pGridRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pGridRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pGridRenderer->SetInputLayout(POS_UV_LAYOUT);
	pGridRenderer->SetRenderState(ALPHA_BLEND);
	pGridRenderer->SetRenderMode(RM_2D);
	
	CMaterial*	pGridMaterial = pGridRenderer->CreateMaterial();
	//pGridMaterial->SetDiffuseTexture("Gameplay", L"Celeste\\Gameplay.png");
	pGridMaterial->SetDiffuseTexture("Grid", L"Celeste\\Grid.png");
	pGridMaterial->SetSampler(SAMPLER_POINT);
	m_pTransform->SetWorldScale(100.0f * m_iGridSize, 100.0f * m_iGridSize, 1.0f);
	m_pTransform->SetPivot(0.5f, 0.5f, 0.0f);
	SAFE_RELEASE(pGridMaterial);
	SAFE_RELEASE(pGridRenderer);
}

void CGridRenderer::SetGridSize(int iGridSize)
{
	m_iGridSize = iGridSize;
	m_pTransform->SetWorldScale(100.0f * m_iGridSize, 100.0f * m_iGridSize, 1.0f);
}

void CGridRenderer::SetColor(const Vector4 & vColor)
{
	CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->SetDiffuse(vColor);
	SAFE_RELEASE(pRenderer);
}

void CGridRenderer::SetEnable()
{
	if (!m_bInitialized)
	{
		CreateGrid();
		m_bInitialized = true;
	}
	else
	{
		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->Enable(true);
	}
}

void CGridRenderer::SetDisable()
{
	if (!m_bInitialized)
	{
		//CreateGrid();
	}
	else
	{
		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->Enable(false);
	}
}

bool CGridRenderer::Initialize()
{
	return true;
}

int CGridRenderer::Update(float fTime)
{
	CTransform* pCameraTransform = m_pScene->GetMainCameraTransform();

	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();

	Vector3 vPosition = m_pTransform->GetWorldPosition();
	// x 또는 Y 로 128씩 카메라가 이동할 경우 이동.
	Vector3 vDistance = vCameraPosition - vPosition;

	if (vDistance.x < -m_iGridSize * 4)
	{
		vPosition.x = vPosition.x - m_iGridSize * 4;
		m_pTransform->SetWorldPosition(vPosition);
	}
	if (vDistance.x > m_iGridSize * 4)
	{
		vPosition.x = vPosition.x + m_iGridSize * 4;
		m_pTransform->SetWorldPosition(vPosition);
	}
	if (vDistance.y < -m_iGridSize * 4)
	{
		vPosition.y = vPosition.y - m_iGridSize * 4;
		m_pTransform->SetWorldPosition(vPosition);
	}
	if (vDistance.y > m_iGridSize * 4)
	{
		vPosition.y = vPosition.y + m_iGridSize * 4;
		m_pTransform->SetWorldPosition(vPosition);
	}


	SAFE_RELEASE(pCameraTransform);
	return 0;
}
