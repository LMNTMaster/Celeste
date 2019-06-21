#include "SceneEffect.h"
#include "GameObject.h"
#include "Transform.h"
#include "Component/Renderer.h"
#include "Material.h"
#include "Device.h"
#include "Scene.h"
#include "../GameData.h"


CSceneEffect::CSceneEffect():
	m_bOpen(false),
	m_bReset(true),
	m_fTimer(0),
	m_fTimeLimit(1.5f),
	m_eAfterEffect(AE_STOP)
{
}


CSceneEffect::~CSceneEffect()
{
	SAFE_RELEASE(m_pLeft);
	SAFE_RELEASE(m_pTop);
	SAFE_RELEASE(m_pRight);
	SAFE_RELEASE(m_pBottom);
}

void CSceneEffect::SetOpen(bool bOpen)
{
	m_bOpen = bOpen;
}

void CSceneEffect::SetAfterEffect(SCENE_AFTER_EFFECT eAfterEffect)
{
	m_eAfterEffect = eAfterEffect;
}

bool CSceneEffect::Initialize()
{

	m_pTransform->SetPivot(0.5f, 0.5f, 0.0f);

	m_pLeft = CGameObject::CreateObject("LeftScreenBorder", m_pLayer);
	m_pTop = CGameObject::CreateObject("TopScreenBorder", m_pLayer);
	m_pRight = CGameObject::CreateObject("RightScreenBorder", m_pLayer);
	m_pBottom = CGameObject::CreateObject("BottomScreenBorder", m_pLayer);

	CTransform* pLeftTransform = m_pLeft->GetTransform();
	CTransform* pTopTransform = m_pTop->GetTransform();
	CTransform* pRightTransform = m_pRight->GetTransform();
	CTransform* pBottomTransform = m_pBottom->GetTransform();

	pLeftTransform->SetPivot(1.0f, 0.5f, 0.0f);
	pRightTransform->SetPivot(0.0f, 0.5f, 0.0f);
	pTopTransform->SetPivot(0.5f, 1.0f, 0.0f);
	pBottomTransform->SetPivot(0.5f, 0.0f, 0.0f);

	SAFE_RELEASE(pLeftTransform);
	SAFE_RELEASE(pTopTransform);
	SAFE_RELEASE(pRightTransform);
	SAFE_RELEASE(pBottomTransform);



	CRenderer* pRenderer = m_pLeft->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);

	CMaterial*	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Black", L"Celeste\\Black.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);


	pRenderer = m_pTop->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);
	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Black", L"Celeste\\Black.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);


	pRenderer = m_pLeft->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);
	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Black", L"Celeste\\Black.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);


	pRenderer = m_pRight->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);
	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Black", L"Celeste\\Black.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);


	pRenderer = m_pBottom->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh(ENGINE_MESH_ORTHORECT_TEX);
	pRenderer->SetShader(STANDARD_TEXTURE_SHADER);
	pRenderer->SetInputLayout(POS_UV_LAYOUT);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderMode(RM_2D);
	pMaterial = pRenderer->CreateMaterial();
	pMaterial->SetDiffuseTexture("Black", L"Celeste\\Black.png");
	pMaterial->SetSampler(SAMPLER_POINT);
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
	return true;
}

int CSceneEffect::Update(float fTime)
{
	//Center는 항상 pivot이 중앙에 있음.
	
	if (m_bOpen)
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / m_fTimeLimit;
		if (fProgress > 0)
		{
			float fScale = (1.0f - sinf((fProgress * PI / 2.0f) + PI / 2.0f)) * 2500.0f;
			m_pTransform->SetWorldScale(fScale, fScale, 1.0f);
			if (fProgress >= 1.0f)
			{
				m_pLeft->Destroy(true);
				m_pRight->Destroy(true);
				m_pTop->Destroy(true);
				m_pBottom->Destroy(true);
				m_pGameObject->Destroy(true);
			}
		}
		else
		{
			m_pTransform->SetWorldScale(1.0f, 1.0f, 1.0f);
		}
	}
	else
	{
		m_fTimer += fTime;
		float fProgress = m_fTimer / m_fTimeLimit;
		
		if (fProgress >= 1.0f)
		{
			fProgress = 1.0f;

			switch (m_eAfterEffect)
			{
			case AE_STOP:
				break;
			case AE_DESTROY:
				m_pLeft->Destroy(true);
				m_pRight->Destroy(true);
				m_pTop->Destroy(true);
				m_pBottom->Destroy(true);
				m_pGameObject->Destroy(true);
				break;
			case AE_RESET:
			{
				GET_SINGLE(CGameData)->ResetArea();
				CGameObject* pPlayer = GET_SINGLE(CGameData)->GetPlayer();
				CTransform* pPlayerTransform = pPlayer->GetTransform();
				m_pTransform->SetWorldPosition(pPlayerTransform->GetWorldPosition());
				m_fTimer = -1.0f;
				m_bOpen = true;
				SAFE_RELEASE(pPlayerTransform);
			}
				break;
			case AE_LOAD:
				break;
			default:
				break;
			}
		}
		float fScale = (1.0f - sinf(((1.0 - fProgress)* PI / 2.0f) + PI / 2.0f)) * 2500.0f;
		m_pTransform->SetWorldScale(fScale, fScale, 1.0f);
	}

	CTransform* pCameraTransform = m_pScene->GetMainCameraTransform();
	Vector3 vCameraPosition = pCameraTransform->GetWorldPosition();
	Vector2 vScreenSize = Vector2(DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight);
	Vector3 vPosition = m_pTransform->GetWorldPosition();
	Vector3 vScale = m_pTransform->GetWorldScale();
	SAFE_RELEASE(pCameraTransform);

	//가로는 3분할이지만, 세로는 1분할이기 때문에, 세로를 더 크게잡아준다.
	CTransform* pLeftTransform = m_pLeft->GetTransform();
	pLeftTransform->SetWorldPosition(vPosition.x - vScale.x / 2.0f, vPosition.y, 0.0f);
	pLeftTransform->SetWorldScale(vScreenSize.x * 10.0f - vScale.x / 2.0f, vScreenSize.y * 10.0f, 1.0f);
	SAFE_RELEASE(pLeftTransform);

	CTransform* pRightTransform = m_pRight->GetTransform();
	pRightTransform->SetWorldPosition(vPosition.x + vScale.x / 2.0f, vPosition.y, 0.0f);
	pRightTransform->SetWorldScale(vScreenSize.x * 10.0f - vScale.x, vScreenSize.y * 10.0f, 1.0f);
	SAFE_RELEASE(pRightTransform);

	CTransform* pTopTransform = m_pTop->GetTransform();
	pTopTransform->SetWorldPosition(vPosition.x, vPosition.y - vScale.y / 2.0f, 0.0f);
	pTopTransform->SetWorldScale( vScale.x, vScreenSize.y * 10.0f, 1.0f);
	SAFE_RELEASE(pTopTransform);

	CTransform* pBottomTransform = m_pBottom->GetTransform();
	pBottomTransform->SetWorldPosition(vPosition.x, vPosition.y + vScale.y / 2.0f, 0.0f);
	pBottomTransform->SetWorldScale(vScale.x, vScreenSize.y * 10.0f, 1.0f);
	SAFE_RELEASE(pBottomTransform);





	return 0;
}
